#include <stdio.h>
#include <stdlib.h>

/* month_day function's prototype*/
void month_day(int year, int yearday, int *pmonth, int *pday);
char *month_name(int n);

int main(int argc, char **argv) {
    if(argc != 3){
        printf("Please input the correct number of arguments\n");
        return 1;
    }
    int pmonth = 0;
    int pday = 0;
    int year = atoi(argv[1]);
    int yearday = atoi(argv[2]);
    month_day(year, yearday, &pmonth, &pday);
    if(pmonth == 0 && pday == 0){
        printf("Please introduce a correct number of day\n");
        return 1;
    }
    printf("%s %2d, %d\n", month_name(pmonth), pday, year);
    return 0;
}

void month_day(int year, int yearday, int *pmonth, int *pday) {
    static char daytab[2][12] = {
        {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
        {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
    };
    int leap = year%4 == 0 && year%100 != 0 || year%400 == 0;
    if(leap == 0 && yearday > 365 || yearday < 1 || leap == 1 && yearday > 366){
        (*pmonth) = 0;
        (*pday) = 0;
        return;
    }
    while(yearday > daytab[leap][*pmonth]){
        yearday -= daytab[leap][*pmonth];
        (*pmonth)++;
    }
    (*pmonth)++;
    *pday = yearday;
}

char *month_name(int n) {

   static char *name[] = {
       "Illegal month",
       "January", "February", "March",
       "April", "May", "June",
       "July", "August", "September",
       "October", "November", "December"
   };

   return (n < 1 || n > 12) ? name[0] : name[n];
}
