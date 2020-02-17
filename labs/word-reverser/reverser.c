#include <stdio.h>
#include <stdlib.h>

#define SIZE 1000

int main(){
    char c;
    char word[1000];
    int count = 0;
    while((c = getchar()) != EOF){
        if(c=='\n') {
            count--;
            while(count>=0) {
                printf("%c", word[count--]);
            }
            count = 0;
            printf("\n");
        }
        else {
            word[count++]=c;
        }
    }    
    
    return 0;
}
