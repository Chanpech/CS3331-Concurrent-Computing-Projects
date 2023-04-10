#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <complex.h>

int main(int argc, char *argv[]){
    int pheight = 200;
    double complex side = 4 + 4 * I;
    float realNum = cimag(side);
    int phtemp = realNum* pheight + pheight;
    printf("realNum %f, phtemp %d\n", realNum, phtemp);
    char *printBuf;
    
    snprintf(printBuf, size, " [%d] %d", 1, points[pheight][pwidth]);
    write(1, printBuf, strlen(printBuf));
    return 0;
}