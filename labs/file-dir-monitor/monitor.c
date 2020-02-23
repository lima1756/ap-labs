#include <stdio.h>
#include <stdlib.h>
#include <ftw.h>
#include <sys/inotify.h>
#include <string.h>
#include "logger.h"
#include "dictionary.h"

#define EVENT_SIZE  ( sizeof (struct inotify_event) )
#define EVENT_BUF_LEN     ( 1024 * ( EVENT_SIZE + 16 ) )

int inotify;
char* lastMoved[2];

int addFile(const char *fpath, const struct stat *sb, int typeflag);
int monitor();
int handleEvent(struct inotify_event *event);

int main(int argc, char **argv){
    if(argc != 2){
        warnf("Please introduce only the directory to monitor\n");
        return 1;
    }
    inotify = inotify_init();
    if(ftw(argv[1], addFile, 20 )== -1){
        errorf("There was a error trying to monitor the directory provided.\n");
        return 1;
    }
    monitor();
    return 0;
}

int addFile(const char *fpath, const struct stat *sb, int typeflag){
    if(typeflag == FTW_D){
        int wd = inotify_add_watch( inotify, fpath, IN_CREATE | IN_DELETE | IN_MOVE);
        addElement(wd, fpath);
    }
    return 0; 
}

int monitor(){
    char buffer[EVENT_BUF_LEN];
    while(1){
        int i = 0;
        int len = read(inotify, buffer, EVENT_BUF_LEN);
        while(i < len){
            struct inotify_event *event = ( struct inotify_event * ) &buffer[ i ];     
            handleEvent(event);
            i += EVENT_SIZE + event->len;;
        }
    }
    return 0;
}

int handleEvent(struct inotify_event *event){
    char *path = getValue(event->wd); 
    if(event->mask & IN_CREATE && event->mask & IN_ISDIR) {
        char directory[strlen(path) + strlen(event->name) + 1];
        infof("DIR CREATION: %s/%s\n", path, event->name);
        strcpy(directory, path);
        strcat(directory, "/");
        strcat(directory, event->name);
        addFile(directory, NULL, FTW_D);
    }
    else if (event->mask & IN_CREATE) {
        infof("FILE CREATION: %s/%s\n", path, event->name);
    }
    else if (event->mask & IN_DELETE && event->mask & IN_ISDIR) {
        infof("DIR DELETION: %s/%s\n", path, event->name);
        inotify_rm_watch(inotify, event->wd);
    }
    else if (event->mask & IN_DELETE) {
        infof("FILE DELETION: %s/%s\n", path, event->name);
    }
    else if (event->mask & IN_MOVED_FROM) {
        lastMoved[0] = event->name;
        lastMoved[1] = path;
    }
    else if (event->mask & IN_MOVED_TO) {
        if(strcmp(lastMoved[1], path)==0){
            infof("FILE %s/%s RENAMED TO: %s/%s\n", path, lastMoved[0] , path, event->name);
        }
        lastMoved[0] = NULL;
        lastMoved[1] = NULL;
    }
    return 0;
}