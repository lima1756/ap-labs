#include <stdio.h>
#include <stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include "dictionary.h"

#define REPORT_FILE "packages_report.txt"
#define BUFFER_SIZE 255

size_t read (int fd, void* buf, size_t cnt);  
size_t write (int fd, void* buf, size_t cnt); 
int close(int fd); 
void analizeLog(char *logFile, char *report);
char* getWord(char *str, int start);
char* substr(char *str, int start, int end);
char* nextLine(int fd);
int readLog(char *logFile);
int writeReport(char *report);

int installed = 0;
int removed = 0;
int updated = 0;

int main(int argc, char **argv) {

    if (argc < 2) {
	printf("Usage:./pacman-analizer.o pacman.log\n");
	return 1;
    }

    analizeLog(argv[1], REPORT_FILE);

    return 0;
}

void analizeLog(char *logFile, char *report) {

    printf("Generating Report from: [%s] log file\n", logFile);

    if(readLog(logFile)!= 0){
        return;
    }

    if(writeReport(report) != 0) {
        return;
    }
    destroyDictionary();
    printf("Report is generated at: [%s]\n", report);
    
}

int readLog(char *logFile){
    int log_fd = open(logFile, O_RDONLY);
    if(log_fd < 0){
        printf("Error while opening the log file\n");
        return -1;
    }
    
    char* line;
    while((line = nextLine(log_fd))!= NULL && strcmp("\0", line)!=0){
        char *date = substr(line, 1, 17);
        if(date == NULL){
            printf("Error while reading\n");
            return -1;
        }
        char *instruction = substr(line, 20, 24);
        if(instruction == NULL){
            printf("Error while reading\n");
            return -1;
        }
        if(strcmp(instruction, "ALPM")!=0){
            continue;
        }
        char *action;
        char* name;
        switch(line[26]){
            case 'i':
                action = substr(line, 26, 35);
                if(action == NULL){
                    printf("Error while reading\n");
                    return -1;
                }
                if(strcmp(action, "installed")!=0)
                    continue;
                name = getWord(line, 36);
                if(name == NULL){
                    printf("Error while reading\n");
                    return -1;
                }
                if(addPackage(name, date)==NULL){
                    printf("Error Out of memory\n");
                    return -1;
                }
                installed++;
                break;
            case 'r':
                action = substr(line, 26, 33);
                if(action == NULL){
                    printf("Error while reading\n");
                    return -1;
                }
                if(strcmp(action, "removed")!=0)
                    continue;
                name = getWord(line, 34);
                if(name == NULL){
                    printf("Error while reading\n");
                    return -1;
                }
                if(removePackage(name, date)==NULL){
                    printf("Error package removed without being installed\n");
                    return -1;
                }
                removed++;
                break;
            case 'u':
                action = substr(line, 26, 34);
                if(action == NULL){
                    printf("Error while reading\n");
                    return -1;
                }
                if(strcmp(action, "upgraded")!=0)
                    continue;
                name = getWord(line, 35);
                if(name == NULL){
                    printf("Error while reading\n");
                    return -1;
                }
                if(updatePackate(name, date, &updated)==NULL){
                    printf("Error package upgraded without being installed\n");
                    return -1;
                }
                break;
            default:
                continue;
        }
    }
    if(line==NULL){
        printf("Error while reading the file\n");
        destroyDictionary();
        return -1;
    }


    if(close(log_fd)<0){
        printf("Error while closing the file!");
        return -1;
    }
    return 0;
}

int writeReport(char *report){
    int report_fd = open(report,  O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH);

    if(report_fd < 0){
        printf("Error while opening the report file\n");
        return -1;
    }

    write(report_fd, "Pacman Packages Report\n----------------------\n", strlen("Pacman Packages Report\n----------------------\n"));


    int size = snprintf(NULL, 0, 
        "- Installed packages \t: %d\n"
        "- Removed packages   \t: %d\n"
        "- Upgraded packages  \t: %d\n"
        "- Current installed  \t: %d\n\n", 
        installed, removed, updated, installed-removed);
    char *buf = (char *)malloc(size + 1);
    if(buf==NULL){
        printf("Error while writing, out of memory");
        return -1 ;
    }
    snprintf(buf, size+1, 
        "- Installed packages \t: %d\n"
        "- Removed packages   \t: %d\n"
        "- Upgraded packages  \t: %d\n"
        "- Current installed  \t: %d\n\n", 
        installed, removed, updated, installed-removed);
    write(report_fd, buf, size);

    writeDictionary(report_fd);

    

    if(close(report_fd)<0){
        printf("Error while closing the report file!");
        return -1;
    }
    return 0;
}

char* nextLine(int fd){
    int bytesRead;
    char buffer[2];
    char *line = malloc(1);
    strcpy(line, "");
    if(line==NULL){
        return NULL;
    }
    int size = 1;
    while((bytesRead = read(fd, buffer, 1))>0){
        buffer[1] = '\0';
        if(buffer[0]!='\n' && buffer[0]!='\r')
        {
            char *tmp = malloc(size + 1);
            strcpy(tmp, line);
            free(line);
            line = tmp;
            strcat(line, buffer);
            size++;
        }
        else{
            strcat(line, "\0");
            return line;
        }
    }
    if(bytesRead==-1){
        return NULL;
    }
    return "\0";
}


char* substr(char *str, int start, int end){
    char *data = malloc(end-start+1);
    if(data == NULL){
        return NULL;
    }
    for(int i = 0; i < end-start; i++){
        data[i] = str[i+start];
    }
    data[end-start]='\0';
    return data;
}

char* getWord(char *str, int start){
    int c = 0;
    while(*(str+start+c) != ' '){
        c++;
    }
    return substr(str, start, start+c);
}