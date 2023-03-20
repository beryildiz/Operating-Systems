#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <iostream>
#include <unistd.h>
#include <string.h>
#include <sstream>
#include <iomanip>

typedef uint uint;

/*
  struct posix_header {         // byte offset
  char name[100];               //   0
  char mode[8];                 // 100
  char uid[8];                  // 108
  char gid[8];                  // 116
  char size[12];                // 124
  char mtime[12];               // 136
  char chksum[8];               // 148
  char typeflag;                // 156
  char linkname[100];           // 157
  char magic[6];                // 257
  char version[2];              // 263
  char uname[32];               // 265
  char gname[32];               // 297
  char devmajor[8];             // 329
  char devminor[8];             // 337
  char prefix[155];             // 345
                                // 500
};
 MetaDaten von einer File mit den Magic-Numbers bekommen wir die file extension heraus.
 .tar hat als Magic-Byte ustar.

 Quelle: https://www.gnu.org/software/tar/manual/html_node/Standard.html
 */
struct header {
    char name[100]; // file name index 0
    char mode[8]; // permissions index 100
    char size[12]; // size (octal) index 124
    char mtime[12]; // modification time (octal) index 136
    char typeflag; // file typeflag index 156
    char magic[6]; // magic indicator index 257
    char uname[32]; // owner user name index 265
    char gname[32]; // owner group name index 297
};
bool eof = false;

/*
 * Quelle: https://github.com/calccrypto/tar/blob/master/tar.c
 * Zeilen: 1040-1047
 * Die Werte werden im octal system dargestellt, die muss dementsprechend zu dezimal system umgeformt werden.
 */
uint oct2uint(char *oct, uint size) ;

/*
 * Hier werden die Zugriffsrechte der Dateien zugewiesen.
 * Quelle: https://github.com/calccrypto/tar/blob/master/tar.c
 * + Quelle Herleitung: https://jameshfisher.com/2017/02/24/what-is-mode_t/
 * Zeilen: 674 - 684
 */
void mode_str(header *entry, char *output);

//die Größe der Datei wird entnommen und an output drangehängt.
void size_str(header *entry, char *output) ;

//Quelle: https://www.cplusplus.com/reference/ctime/strftime/
//Das Datum der letzten Modufikation (Änderung) der Datei wird entnommen und an output drangehängt.
void time_str(header *entry, char *output) ;

//Zur Überprüfung ob das eine .tar Datei ist. Speicher vom Magic Array wird mit ustar verglichen.
bool isUSTAR(header *entry) ;

//Die Methode liest von der File in den Header struct ein.
void read_header(int file, header *hdr);

//Die Methode ist für die Ausgabe zuständig -> schrittwieses Ausgeben.
void print_details(header *entry);

int main(int argc, char const *argv[]) {

    //Fehler Abfangen, wenn man keine Datei mit eangibt
    if (argc != 2) {
        std::cerr << "Bitte Dateinamen als Argument uebergeben" << std::endl;
        return -1;
    }
    const char *filename = argv[1];
    int file = open(filename, O_RDONLY);
    //Fehler Abfangen, wenn die Datei nicht gefunden wird (falscher Name uebergeben)
    if (file == -1) {
        std::cerr << "Datei nicht gefunden" << std::endl;
        return -1;
    }
    int counter = 0;
    while (!eof) {
        header hdr;
        read_header(file, &hdr);
        if (eof) break;
        //Fehler Abfangen, wenn die Datei keine .tar Datei ist.
        if (!isUSTAR(&hdr)) {
            std::cerr << "Datei ist nicht im USTAR Format" << std::endl;
            return -1;
        }
        print_details(&hdr);
        int bytes = oct2uint(hdr.size, sizeof(hdr.size));
        /*
         * Daten und Meta-Daten (Header) werden in einem ustar-Archiv in Blöcken (512 Byte)
         * gespeichert. Die Header-Blöcke müssen Sie der Reihe nach auslesen und interpretieren.
         */
        if (bytes != 0) bytes += 512 - (bytes % 512);
        lseek(file, bytes, SEEK_CUR);
        counter++;
    }

    //Falls die Datei keinen Inhalt besitzt.
    if (counter == 0) {
        std::cerr << "Datei hat keinen Inhalt" << std::endl;
        return -1;
    }
    int res = close(file);
    //Wenn es zu komplikationen beim Schließen der Datei kommt wird der Fehler auch abgefangen.
    if (res == -1) {
        std::cerr << "Fehler beim Schliessen der Datei" << std::endl;
        return -1;
    }

    return 0;
}

/*
 * Quelle: https://github.com/calccrypto/tar/blob/master/tar.c
 * Zeilen: 1040-1047
 * Die Werte werden im octal system dargestellt, die muss dementsprechend zu dezimal system umgeformt werden.
 */
uint oct2uint(char *oct, uint size) {
    uint out = 0;
    int i = 0;
    while (((uint)i < size) && oct[i]) {
        out = (out << 3) | (uint) (oct[i++] - '0');
    }
    return out;
}

/*
 * Hier werden die Zugriffsrechte der Dateien zugewiesen.
 * Quelle: https://github.com/calccrypto/tar/blob/master/tar.c
 * + Quelle Herleitung: https://jameshfisher.com/2017/02/24/what-is-mode_t/
 * Zeilen: 674 - 684
 */
void mode_str(header *entry, char *output) {
    //mit modet bekommen wir unsere Bits und die werden Bitwise mit Flags verglichen.
    const mode_t modet = oct2uint(entry->mode, 7);
    /* Für die Zeile mit "-hlcbdp-":
        Values used in typeflag field.
        #define REGTYPE  '0'            / regular file /
        #define AREGTYPE '\0'           / regular file /
        #define LNKTYPE  '1'            / link /
        #define SYMTYPE  '2'            / reserved /
        #define CHRTYPE  '3'            / character special /
        #define BLKTYPE  '4'            / block special /
        #define DIRTYPE  '5'            / directory /
        #define FIFOTYPE '6'            / FIFO special /
        #define CONTTYPE '7'            / reserved
     */
    const char mode_str[11] = {"-hlcbdp-"[entry->typeflag ? entry->typeflag - '0' : 0],
                               modet & S_IRUSR ? 'r' : '-',
                               modet & S_IWUSR ? 'w' : '-',
                               modet & S_IXUSR ? 'x' : '-',
                               modet & S_IRGRP ? 'r' : '-',
                               modet & S_IWGRP ? 'w' : '-',
                               modet & S_IXGRP ? 'x' : '-',
                               modet & S_IROTH ? 'r' : '-',
                               modet & S_IWOTH ? 'w' : '-',
                               modet & S_IXOTH ? 'x' : '-',
                               0};
    //informationen aus mode_str wird an output drangehängt.
    strcat(output, mode_str);
}

//die Größe der Datei wird entnommen und an output drangehängt.
void size_str(header *entry, char *output) {
    int bytes = oct2uint(entry->size, sizeof(entry->size));
    std::stringstream ss;
    ss << std::setw(10) << bytes;
    strcat(output, ss.str().c_str());
}

//Quelle: https://www.cplusplus.com/reference/ctime/strftime/
//Das Datum der letzten Modufikation (Änderung) der Datei wird entnommen und an output drangehängt.
void time_str(header *entry, char *output) {
    time_t mtime = oct2uint(entry->mtime, 12);
    char mtime_str[32];
    strftime(mtime_str, sizeof(mtime_str), "%Y-%m-%d %H:%M", localtime(&mtime));
    strcat(output, mtime_str);
}

//Zur Überprüfung ob das eine .tar Datei ist. Speicher vom Magic Array wird mit ustar verglichen.
bool isUSTAR(header *entry) {
    return (memcmp("ustar", entry->magic, 5) == 0);
}

//Die Methode liest von der File in den Header struct ein.
void read_header(int file, header *hdr) {
    if (read(file, hdr->name, 100) < 1 || oct2uint(hdr->name, 100) == 0) {
        eof = true;
        return;
    };
    read(file, hdr->mode, 8);
    lseek(file, 16, SEEK_CUR);
    read(file, hdr->size, 12);
    read(file, hdr->mtime, 12);
    lseek(file, 8, SEEK_CUR);
    read(file, &(hdr->typeflag), 1);
    lseek(file, 100, SEEK_CUR);
    read(file, hdr->magic, 6);
    lseek(file, 2, SEEK_CUR);
    read(file, hdr->uname, 32);
    read(file, hdr->gname, 32);
    lseek(file, 155 + 16 + 12, SEEK_CUR);
}

//Die Methode ist für die Ausgabe zuständig -> schrittwieses Ausgeben.
void print_details(header *entry) {
    char output[256] = "";
    mode_str(entry, output);
    strcat(output, " ");
    strcat(output, entry->uname);
    strcat(output, "/");
    strcat(output, entry->gname);
    size_str(entry, output);
    strcat(output, " ");
    time_str(entry, output);
    strcat(output, " ");
    strcat(output, entry->name);
    std::cout << output << std::endl;
}