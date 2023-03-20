#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h> /* fuer Kontrollfluss */
#include <limits.h>
#include <unistd.h>

using namespace std;

struct posix_header {                // byte offset
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

int main(int argc, char *argv[]) {   
    struct posix_header header;
    
    int fd;
    fd = open("dirStruct.tar.gz" , O_RDONLY);
    
    if (fd < 0) {
        printf("open call fail");
        return EXIT_FAILURE;
    }
    
    int bytes_read = read(fd, &header, sizeof(header));

    cout << bytes_read << "\n";
    cout << header.name << "\n"; // Kein sinnvoller Output

    close(fd);

    return EXIT_SUCCESS;
}





