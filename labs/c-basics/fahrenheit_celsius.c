#include <stdio.h>
#include <stdlib.h>

#define   LOWER  0       /* lower limit of table */
#define   UPPER  300     /* upper limit */
#define   STEP   20      /* step size */

/* print Fahrenheit-Celsius table */

void printLine(double fahr){
    printf("Fahrenheit: %3.2f, Celcius: %6.2f\n", fahr, (5.0/9.0)*(fahr-32));
}

int main(int argc, char** args)
{
    double fahr;
    if(argc == 2){
        // Simple conversion
        fahr = atof(args[1]);
        printLine(fahr);
    }
    else if(argc == 4){
        // Range Conversion
        for (fahr = atof(args[1]); fahr <= atof(args[2]); fahr = fahr + atof(args[3])) {
            printLine(fahr);
        }
    }
    else {
        // Default table
        for (fahr = LOWER; fahr <= UPPER; fahr = fahr + STEP) {
            printLine(fahr);
        }
    }

    return 0;
}
