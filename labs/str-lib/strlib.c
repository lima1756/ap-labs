#include "strlib.h"
#include <stdio.h>
#include <stdlib.h>

int mystrlen(char *str){
    int c = 0;
    while(str[c] != '\0') { 
        c++; 
    }
    return c;
}

char *mystradd(char *origin, char *addition){
    int originLength = mystrlen(origin);
    int additionLen = mystrlen(addition);
    char *str = malloc(originLength+additionLen);
    int c = 0;
    while(origin[c] != '\0'){
        str[c] = origin[c];
        c++;
    }
    c = 0;
    while(addition[c] != '\0'){
        str[c+originLength] = addition[c];
        c++;
    }
    str[originLength+additionLen]='\0';
    return (char *)str;
}

int mystrfind(char *origin, char *substr){
    int originLen = mystrlen(origin);
    int subStrLen = mystrlen(substr);
    printf("%i , %i\n", originLen, subStrLen);
    int i = 0, j;
    while(i < originLen){
        if(origin[i] == substr[0]){
            for(j = 1; j<subStrLen; j++){
                if(origin[i+j] != substr[j]){
                    break;
                }
            }
            if(j==subStrLen){
                return 1;
            }
        }
        i++;
    }
    return 0;
}
