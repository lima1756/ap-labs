#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dictionary.h"

struct entry{
    struct entry *next;
    long key;
    char* path;
};

static struct entry *dictionary[DICTIONARY_SIZE];

unsigned hash(long key)
{
    return (key + 101) % DICTIONARY_SIZE;
}

char* getValue(int key)
{
    struct entry *element;
    for (element = dictionary[hash(key)]; element != NULL; element = element->next) {
        if (key == element->key) {
            return element->path; 
        }
    }
    return NULL; 
}

struct entry* getElement(int key)
{
    struct entry *element;
    for (element = dictionary[hash(key)]; element != NULL; element = element->next) {
        if (key == element->key) {
            return element; 
        }
    }
    return NULL; 
}

void addElement(int key, char *path)
{ 
    struct entry *element;
    unsigned hashval;
    if ((element = getElement(key)) == NULL) { 
        element = malloc(sizeof(*element));
        hashval = hash(key);
        element->next = dictionary[hashval];
        element->key = key;
        element->path = malloc(strlen(path)+1);
        strcpy(element->path, path);
        dictionary[hashval] = element;
    }
    else {
        free(element->path);
        element->path = malloc(strlen(path)+1);
        strcpy(element->path, path);
    }
}