#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dictionary.h"

#define BUFFER_SIZE 255
#define NOICE_WORDS 12

char *noWords[NOICE_WORDS] = {"a", "and", "are", "but", "is", "of", "the", "to", "was", "were", "wich", "with" };
 

int search(char *);
int binarySearch(char *, int , int );
int saveWord(char *, int , int );
void toLower(char *);

int main(int argc, char **argv) {
    FILE* file;
    if(argc != 2 || (file = fopen(argv[1], "r"))==NULL){
        printf("Error");
        return 1;
    }
    char c;
    char word[BUFFER_SIZE];
    int size=0, line=1;
    
    while((c=fgetc(file))!=EOF){
        if(c>='a' && c<='z' || c>='A' && c<='Z'){
            word[size]=c;
            size++;
        }
        else if(size>0) {
            saveWord(word, size, line);
            size = 0;
            if(c=='\n'){
                line++;
            }
        }
        else if(size == 0 && c=='\n'){
            line++;
        }
    }
    printDictionary();
    return 0;
}

int search(char *str){
    return binarySearch(str, 0, NOICE_WORDS-1);
}

int binarySearch(char *str, int start, int end){
    if(start>end || end<start) {
        return -1;
    }
    int current = start + (end-start)/2;
    int comparison = strcmp(str, noWords[current]);
    if(comparison == 0) {
        return current;
    }
    if(comparison < 0){
        return binarySearch(str, start, current-1);
    }
    else {
        return binarySearch(str, current+1, end);
    }
    
}

int saveWord(char *str, int size, int line){
    if(size > 0){
        str[size]='\0';
        toLower(str);
        if(search(str)==-1){
            addElement(str, size, line);
        }
    }
    return 0;
}

void toLower(char *word){
    for(; *word!='\0'; word++){
        if(*word>='A' && *word<='Z'){
            *word=*word-('A'-'a');
        }
    }
}