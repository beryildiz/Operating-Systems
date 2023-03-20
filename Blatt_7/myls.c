#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <stdint.h>
#include <math.h>
#include <ctype.h>
#include <locale.h>

enum color{None, Red, Green};

typedef struct{
    char mode[11];
    long int links;
    char owner[256];
    char group[256];
    long int filesize;
    char mtime[13];
    enum color color;
    char file[PATH_MAX];
} longlist;

struct longlistList{
    longlist fileInfo;
    struct longlistList* next;
};

typedef struct{
    int linksLen;
    int ownerLen;
    int groupLen;
    int filesizeLen;
} lengthTrack;

DIR *dir;

struct stat buf;
struct dirent *dp;

char* isHidden(char* string) {
    if (string[0] == '.' && string[1] == '.')
        return string;
    else if (string[0] == '.' && string[1] == '\0')
        return string;
    else if (string[0] == '.' && string[1] != '.')
        return &string[1];
    
    return string;
}

void strmode(mode_t mode, char* buf) {
    const char chars[] = "drwxrwxrwx";

    buf[0] = (S_ISDIR(mode)) ? 'd' : '-';
    for (size_t i = 1; i < 10; i++) {
        buf[i] = ( mode & ( 1 << (9-i) ) ) ? chars[i] : '-';
    }
    buf[10] = '\0';
}

int maxval(int val1, int val2) {
    if (val1 > val2) 
        return val1;
    else
        return val2;
}

int numPlaces(int n) {
    if (n < 0) return numPlaces ((n == INT_MIN) ? INT_MAX: -n);
    if (n < 10) return 1;
    return 1 + numPlaces (n / 10);
}

struct longlistList* getElement(longlist fileInfo) {
    // allocating space
    struct longlistList* newElement = (struct longlistList*)malloc(sizeof(struct longlistList));
    // inserting the required data
    newElement->fileInfo = fileInfo;
    newElement->next = NULL;
    return newElement;
}

void insertNewElement(longlist fileInfo, struct longlistList** head) {  
    if (*head == NULL) {
        *head = getElement(fileInfo);
        return;
    } else {
        if (strcoll(isHidden(fileInfo.file), isHidden((*head)->fileInfo.file)) < 0) {
            struct longlistList* newElement = getElement(fileInfo);
            newElement->next = *head;
            *head = newElement;
            return;
        }
    }

    struct longlistList* newElement = getElement(fileInfo);
    
    struct longlistList* ptr = *head;
    while (ptr->next != NULL) {     
        if ( (strcoll(isHidden(fileInfo.file), isHidden(ptr->fileInfo.file)) > 0) && (strcoll(isHidden(fileInfo.file), isHidden(ptr->next->fileInfo.file)) < 0) )
            break;
        ptr = ptr->next;
    }
    
    newElement->next = ptr->next;
    ptr->next = newElement;
}

int main(int argc, char *argv[]){
    //setlocale(LC_ALL, "");

    char directory[PATH_MAX] = ".";
    struct longlistList* outputList = NULL;
    lengthTrack lengths; 
    long int total;

    int showHiddenFiles = 0;
    int longList = 0;
    int showOwner = 1;
    int showGroup = 1;

    int opt;
    while((opt = getopt(argc, argv, "algo")) != -1) 
    { 
        switch(opt) 
        { 
            case 'a': 
                showHiddenFiles = 1;
                break;
            case 'l':  
                longList = 1;
                break;
            case 'g': 
                longList = 1;
                showOwner = 0;
                break;
            case 'o': 
                longList = 1;
                showGroup = 0;
                break;
            default :
                break;     
        } 
    } 

   
    if( (argc > 1) && (argv[argc-1][0] != '-') ) {
        strcpy(directory, argv[argc-1]);
    }

    time_t rawtime;
    time(&rawtime);
            
    struct tm *tm_now = localtime(&rawtime);
    int dayNow = tm_now->tm_mday;
    int monthNow = tm_now->tm_mon;
    int yearNow = tm_now->tm_year;

    if ((dir = opendir (directory)) == NULL) {
        perror("Cannot open directory");
        return EXIT_FAILURE;
    }

    while ((dp = readdir (dir)) != NULL){
        if ( (dp->d_name[0] != '.') || (showHiddenFiles > 0) ) {
            char mode[11] = "";
            char path[PATH_MAX] = "";
            strcat(path, directory);
            if (directory[strlen(directory)-1] != '/') {
                strcat(path, "/");
            }
            strcat(path, dp->d_name);

            // Wir benoetigen hier den absoluten Path // REALPATH nutzen, ist besser dann braucht man keine concat!
            lstat(path, &buf);

            strmode(buf.st_mode, mode);

            struct tm *my_tm = localtime(&buf.st_mtim.tv_sec);

            // Integer rundete nicht auf sondern ab
            total += (long int)ceil( (double)buf.st_size / (double)buf.st_blksize );
            
            longlist new;
            strcpy(new.mode, mode); // s
            new.links = buf.st_nlink; // ld
            strcpy(new.owner, getpwuid(buf.st_uid)->pw_name); // s
            strcpy(new.group, getgrgid(buf.st_gid)->gr_name); // s
            new.filesize = buf.st_size; // ld

            int monthGone = 0;
            if (dayNow >= my_tm->tm_mday)
                monthGone = 1;

            if (monthNow + 11 * (yearNow - my_tm->tm_year) + monthGone - my_tm->tm_mon < 6)
                strftime(new.mtime, 13, "%b %d %H:%M", my_tm); // s
            else 
                strftime(new.mtime, 13, "%b %d  %Y", my_tm); // s

            if (mode[0] == '-' && mode[9] == 'x')
                new.color = Red;
            else if (dp->d_name[strlen(dp->d_name)-2] == '.' && dp->d_name[strlen(dp->d_name)-1] == 'c')
                new.color = Green;
            else
                new.color = None;

            strcpy(new.file, dp->d_name); // s

            insertNewElement(new, &outputList);

            lengths.linksLen = maxval(lengths.linksLen, numPlaces(buf.st_nlink));
            lengths.ownerLen = maxval(lengths.ownerLen, strlen(getpwuid(buf.st_uid)->pw_name));
            lengths.groupLen = maxval(lengths.groupLen, strlen(getgrgid(buf.st_gid)->gr_name));
            lengths.filesizeLen = maxval(lengths.filesizeLen, numPlaces(buf.st_size)); // MODES MIT Makros pruefen, in zukunft fragen und aufgabenblatt gucken :D
        }   
    }
    closedir(dir);

    if (longList > 0) {
        printf("total %ld\n", total * 4);

        struct longlistList* p = outputList;
        while(p != NULL) {     
            printf("%s ", p->fileInfo.mode);
            printf("%*ld ", lengths.linksLen, p->fileInfo.links);
            if (showOwner > 0)
                printf("%*s ", lengths.ownerLen, p->fileInfo.owner);
            if (showGroup > 0)
                printf("%*s ", lengths.groupLen, p->fileInfo.group);
            printf("%*ld ", lengths.filesizeLen, p->fileInfo.filesize);
            printf("%s ", p->fileInfo.mtime);
            if (p->fileInfo.color == None)
                printf("%s\n", p->fileInfo.file);
            else if (p->fileInfo.color == Red)
                printf("\033[1;31m%s\033[0m\n", p->fileInfo.file);
            else if (p->fileInfo.color == Green)
                printf("\033[1;32m%s\033[0m\n", p->fileInfo.file);
            //free(p);
            p = p->next;
        }
    } else {
        struct longlistList* p = outputList;
        while(p != NULL) {
            if (p->fileInfo.color == None)
                printf("%s\n", p->fileInfo.file);
            else if (p->fileInfo.color == Red)
                printf("\033[1;31m%s\033[0m\n", p->fileInfo.file);
            else if (p->fileInfo.color == Green)
                printf("\033[1;32m%s\033[0m\n", p->fileInfo.file);
            //free(p);
            p = p->next;
        }
    }
        
    return EXIT_SUCCESS;
}