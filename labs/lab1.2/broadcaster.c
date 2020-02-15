#include <stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include <fcntl.h>  
#include <dirent.h>
#include <string.h>

#define ptsDirectory "/dev/pts/"
#define defaultMessage "\nBroadcast emitted\n"

int close(int fd); 
size_t write (int fd, void* buf, size_t cnt); 

int messageLength(int argc, char **args){
    int size = 0;
    for(int i = 0; i < argc; i++) {
        size+=(strlen(args[i]) + 2);  // adding two more considering a possible character 
                                    // at the start and at the end like space
    }
    return size;
}


int main(int argc, char** args) {
    int file;
    DIR * ptsDir;
    struct dirent * entry;
    int totalConnections = -2;  // -2 because we remove the . and . directories
    char directory[20] = "";

    ptsDir = opendir("/dev/pts");
    entry = readdir(ptsDir);

    //Counting the total number of files in directory
    while(entry != NULL){
        totalConnections++;
        entry = readdir(ptsDir);
    }

    // writing into the files (virtual connections)
    for(int i = 0; i < totalConnections; i++){
        snprintf(directory, sizeof directory, "%s%d", ptsDirectory,i);  // Current terminal directory
        file = open(directory, O_RDWR);
        if(argc > 1) {
            const int size = messageLength(argc, args);
            char message[size];
            message[0]='\0';
            strncat(message, "\n", 2);
            for(int j = 1; j < argc-1; j++){
                strncat(message,args[j], strlen(args[j]));
                strncat(message," ", 2);
            }            
            strncat(message,args[argc-1], strlen(args[argc-1]));
            strncat(message,"\n", 2);
            write(file, message, strlen(message));
        }
        else {
            write(file, defaultMessage, sizeof defaultMessage);
        }
        
        close(file);
    }

    return 0;
}