#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <math.h>

#define MAX 1000

char* input[MAX];

int numcmp(char *, char* );
void mergeSort(int, void*[], int (*)(void *, void *));
void merge(int, int, void*[], int, int, int (*)(void *, void *));
void printAll(int, void *[]);
int readAll(char *[]);
int getLine(char []);

int main(int argc, char** argv)
{
    int size = readAll(input);
    char numeric = 0;
    if(argc > 1 && strcmp(argv[1], "-n")==0){
        numeric = 1;
    }
    mergeSort(size, input, (int (*)(void *, void *))numeric?numcmp:strcmp);
    printAll(size, input);
    return 0;
}

int numcmp(char *s1, char *s2) {
    double val1, val2;
    val1 = atof(s1);
    val2 = atof(s2);

    if(val1 < val2) {
        return -1;
    }
    else if(val1 > val2){
        return 1;
    }
    return 0;
}

void mergeSort(int size, void *input[], int (*comp)(void *, void *)){
    double iterations;
    iterations = log2(size);
    // logical split
    for(int n = 0; n < iterations; n++){
        int jump = pow(2, n);
        for(int iStart = 0, jStart = jump; iStart < size; iStart+=jump*2, jStart+=jump*2) {
            merge(size, jump*2, input, iStart, jStart, comp);
        }
    }
}

void merge(int size, int chunkSize, void *input[], int iStart, int jStart, int (*comp)(void *, void *)){
    int i = iStart;
    int j = jStart;
    void *output[chunkSize];
    int k = 0;
    while(i<jStart && j < iStart + chunkSize && j < size){
        
        if((*comp)(input[i], input[j])<0) {
            output[k++] = input[i++];
        }
        else {
            output[k++] = input[j++];
        }
    }
    while(i<jStart){
        output[k++] = input[i++];
    }
    while(j < chunkSize + iStart && j < size){
        output[k++] = input[j++];
    }
    for(int l = iStart, m = 0; m < chunkSize; l++, m++){
        input[l] = output[m];
    }
}

void printAll(int size, void *data[]){
    for(int i = 0; i < size; i++){
        printf("%s, ", (char*) data[i]);
    }
    printf("\n");
}

int readAll(char* data[]){
    int total = 0;
    int len = 0;
    char *p, line[MAX];
    while((len = getLine(line)) > 0 && total < MAX && (p = (char*) malloc(len)) != NULL) {
        strcpy(p, line);
        data[total++] = p;
    }
    return total; 
}

int getLine(char line[]){
    char c;
    int len = 0;
    while((c=getchar())!=EOF && c != '\n' && len < MAX-1){
        line[len++] = c;
    }
    if(c=='\n'){
        line[len] = 0;
    }
    return len;
}