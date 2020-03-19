#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dictionary.h"

int add(package* element, int data);
void printpackage(package *element);
size_t write (int fd, void* buf, size_t cnt); 

struct _package{
    int installed;
    int updates;
    char* installedDate;
    char* updatedDate;
    char* removalDate;
    char* name;
    package *next;
} ;


static package *dictionary[DICTIONARY_SIZE];

unsigned hash(char *s)
{
    unsigned hashval;
    for (hashval = 0; *s != '\0'; s++)
      hashval = *s + 101 * hashval;
    return hashval % DICTIONARY_SIZE;
}

package *getPackage(char *key)
{
    package *element;
    for (element = dictionary[hash(key)]; element != NULL; element = element->next)
        if (strcmp(key, element->name) == 0)
          return element; 
    return NULL; 
}

package *addElement(package *pkg)
{
    package *element;
    unsigned hashval;
    if ((element = getPackage(pkg->name)) == NULL) { 
        element = pkg;
        hashval = hash(pkg->name);
        element->next = dictionary[hashval];
        dictionary[hashval] = element;        
    }
    return element;
}

package *addPackage(char *name, char *installedDate){
    package *pkg = getPackage(name);
    if(pkg == NULL){
        pkg = malloc(sizeof(package));
        if(pkg == NULL)
            return NULL;
    }
    pkg->installed = 1;
    pkg->name = name;
    pkg->installedDate = installedDate;
    pkg->updates = 0;
    pkg->removalDate = "NOT DELETED";
    pkg->updatedDate = "NOT UPDATED";
    return addElement(pkg);
}

package *updatePackate(char *name, char *updateDate, int *counter){
    package *pkg = getPackage(name);
    if(pkg == NULL)
        return NULL;
    if(strcmp(pkg->updatedDate, "NOT UPDATED")==0){
        (*counter)++;
    }
    pkg->updatedDate = updateDate;
    pkg->updates++;
    return pkg;
}

package *removePackage(char *name, char *removeDate){
    package *pkg = getPackage(name);
    if(pkg == NULL)
        return NULL;
    pkg->removalDate = removeDate;
    pkg->installed == 0;
    return pkg;
}



void destroyDictionary(){
    package *element;
    for(int i = 0; i < DICTIONARY_SIZE; i++){
        if((element=dictionary[i])!=NULL){
            free(element);
        }
    }
}

char* printPackage(package *element, int *size){
    *size = snprintf(NULL, 0, 
        "- Package Name \t: %s\n"
        "\t- Install date \t: %s\n"
        "\t- Last update date \t: %s\n"
        "\t- How many updates \t: %d\n"
        "\t- Removal date \t: %s\n", 
        element->name, element->installedDate, element->updatedDate, element->updates, element->removalDate);
    char *buf = (char *)malloc(*size + 1);
    if(buf==NULL){
        return NULL;
    }
    snprintf(buf, *size+1, 
        "- Package Name \t: %s\n"
        "\t- Install date \t: %s\n"
        "\t- Last update date \t: %s\n"
        "\t- How many updates \t: %d\n"
        "\t- Removal date \t: %s\n",
        element->name, element->installedDate, element->updatedDate, element->updates, element->removalDate);
    return buf;
}

void printDictionary(){
    package *element;
    printf("List of packages\n----------------\n");
    for(int i = 0; i < DICTIONARY_SIZE; i++){
        if((element=dictionary[i])!=NULL){
            int size = 0;
            char *str = printPackage(element, &size);
            if(str == NULL){
                printf("Error while writing output\n");
                return;
            }
            printf(str);
        }
    }
}


void writeDictionary(int fd){
    package *element;
    int check = write(fd, "List of packages\n----------------\n", strlen("List of packages\n----------------\n"));
    if(check<0){
        printf("Error while writing output\n");
        return;
    }
    for(int i = 0; i < DICTIONARY_SIZE; i++){
        if((element=dictionary[i])!=NULL){
            int size = 0;
            char *str = printPackage(element, &size);
            if(str == NULL){
                printf("Error while writing output\n");
                return;
            }
            check = write(fd, str, strlen(str));
            if(check<0){
                printf("Error while writing output\n");
                return;
            }
        }
    }
}   
