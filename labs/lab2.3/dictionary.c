#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dictionary.h"

int add(struct entry* element, int data);
void printEntry(struct entry *element);

struct entry{
    int size;
    struct entry *next;
    char* key;
    int* position;
};

static struct entry *dictionary[DICTIONARY_SIZE];

unsigned hash(char *s)
{
    unsigned hashval;
    for (hashval = 0; *s != '\0'; s++)
      hashval = *s + 101 * hashval;
    return hashval % DICTIONARY_SIZE;
}

struct entry *getElement(char *key)
{
    struct entry *element;
    for (element = dictionary[hash(key)]; element != NULL; element = element->next)
        if (strcmp(key, element->key) == 0)
          return element; 
    return NULL; 
}

struct entry *addElement(char *key, int len, int data)
{
    struct entry *element;
    unsigned hashval;
    
    if ((element = getElement(key)) == NULL) { 
        element = malloc(sizeof(*element));
        hashval = hash(key);
        element->next = dictionary[hashval];
        element->key = malloc((sizeof (char)) * (len+1));
        strcpy(element->key, key);
        element->size = 0;
        dictionary[hashval] = element;        
    }
    if(add(element, data)==-1){
        printf("Error while adding the element, out of memory");
    }  
    return element;
}

int add(struct entry* element, int data){
    int *temp = element->position;
    element->position = malloc(sizeof(int) * (element->size+1));
    if(element->position == NULL){
        return -1;
    }
    for(int i = 0; i < element->size; i++){
        element->position[i] = temp[i];
    }
    element->position[element->size]=data;
    element->size++;
    if(element->size>1){
        free(temp);
    }
    return 0;
}

void printDictionary(){
    struct entry *element;
    for(int i = 0; i < DICTIONARY_SIZE; i++){
        if((element=dictionary[i])!=NULL){
            printEntry(element);
        }
    }
}

void printEntry(struct entry *element){
    printf("%s: ", element->key);
    for(int j = 0; j < element->size-1; j++){
        printf("%d, ", element->position[j]);
    }
    printf("%d\n", element->position[element->size-1]);
}
