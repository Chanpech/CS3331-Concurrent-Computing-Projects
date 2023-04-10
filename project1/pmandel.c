//  Author:         Chanpech Hoeng
//  Course:         CS3331 R01
//  Latest update:    1/31/23
//
//  Description:    pmandel.c parallelize the calculation of points in the Mandelbrot set (orginally the mandel.c).
//                  The application invoked: pmandel tlr tli side-length max-iterations pixels image-file nprocs.
//
#include <complex.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

#define PRINTBUFSIZE 241
#define COLORS 15
unsigned char palette[COLORS][3] = {{255, 255, 255}, {255, 0, 0}, {255, 128, 0}, {255, 255, 0}, {128, 255, 0}, {0, 255, 0}, {0, 255, 128}, {0, 255, 255}, {0, 128, 255}, {0, 0, 255}, {128, 0, 225}, {255, 0, 255}, {255, 0, 128}, {128, 128, 128}, {0, 0, 0}};

void addPoint(int scheme, int iterations, int maxiterations, FILE *fp)
{

    unsigned char color[3];
    int index;

    if (scheme == 1)
    {
        // Scheme 1 - Greyscale uniform
        color[0] = 255 - ((double)iterations / (double)maxiterations) * 255;
        color[1] = 255 - ((double)iterations / (double)maxiterations) * 255;
        color[2] = 255 - ((double)iterations / (double)maxiterations) * 255;
    }
    if ((scheme == 2) || (scheme == 3))
    {
        if (scheme == 2)
        {
            iterations = sqrt((double)iterations / (double)maxiterations) * (float)(maxiterations);
        }

        if (iterations == maxiterations)
            index = COLORS - 1; //-- Points in the set are always black, assumed last in palette
        else
            index = iterations / (maxiterations / (COLORS - 1));

        color[0] = palette[index][0];
        color[1] = palette[index][1];
        color[2] = palette[index][2];
    }

    fputc(color[0], fp);
    fputc(color[1], fp);
    fputc(color[2], fp);
}

FILE *openChildFile(FILE *fptemp, char *fileName, int colorscheme, char printBuf[PRINTBUFSIZE], int wPixels, int hPixels)
{
    //printf("Writing in writeChildField...\n");

    fptemp = fopen(fileName, "wb"); /* b - binary mode */
    if (fptemp == NULL)
    {
        printf("%s cannot be opened for write\n", fileName);
        perror("Error:");
    }
    (void)fprintf(fptemp, "P6\n%d %d\n255\n", wPixels, hPixels);

    return fptemp;
}

void concatenateFiles(FILE *fp, FILE *fpChild)
{
    char buf[81];       //-- Holds header line.  Thrown away.
    char *contents;     //-- Hold data after the header
    long fileSize;      //-- File size
    long pixelStart;    //-- Offset of pixel data
    int byte;           //-- Holds a byte of data
    long pixelDataSize; //-- Size of pixel data in bytes

    // Find th size of the file
    fseek(fpChild, 0, SEEK_END);
    fileSize = ftell(fpChild);
    //printf("File has %d bytes\n", fileSize);

    fseek(fpChild, 0, SEEK_SET);
    fgets(buf, 81, fpChild);
    //printf("Buf <%s>\n", buf);
    fgets(buf, 81, fpChild);
    //printf("Buf <%s>\n", buf);
    fgets(buf, 81, fpChild);
    //printf("Buf <%s>\n", buf);

    pixelStart = ftell(fpChild);
    pixelDataSize = fileSize - pixelStart;
    contents = malloc(pixelDataSize); // Allocate space to hold the contents
    //printf("Reading %d bytes of pixel data\n", pixelDataSize);
    fread(contents, 1, pixelDataSize, fpChild); // Get the pixel data

    fclose(fpChild);
    fwrite(contents, 1, pixelDataSize, fp);
}
char *strremove(char *str, const char *sub)
{
    size_t len = strlen(sub);
    if (len > 0)
    {
        char *p = str;
        while ((p = strstr(p, sub)) != NULL)
        {
            memmove(p, p + len, strlen(p + len) + 1);
        }
    }
    return str;
}
void createFileName(char *argv, char **childFiles, int numProcessors)
{

    char **arrayBuffer = childFiles;
    char *name = malloc(sizeof(argv));
    sprintf(name, argv);
    //printf("%s %d\n", argv, strlen(argv));

    if (strstr(name, ".ppm") != NULL)
    {
        for (int i = 0; i < numProcessors; i++)
        {
            char *temp;
            sprintf(name, argv);
            strremove(name, ".ppm");
            char increment[sizeof(argv) + 4];
            sprintf(increment, "%d.ppm", i + 1);
            strcat(name, increment);
            temp = malloc(sizeof(name));
            sprintf(temp, name);
            arrayBuffer[i] = temp;
        }
    }
}

int main(int argc, char *argv[])
{
    int pid = getpid();
    // Declare variables
    double complex start;        //-- Top left corner
    double range;                //-- Length of a side in complex plane
    int pixels;                  //-- Pixels per side
    double increment;            //-- Length between pixels in complex plane
    double complex z;            //-- Temporary used in mandelbrot recurrence computation
    double complex c;            //-- Temporary used in mandelbrot recurrence computation
    double size;                 //-- Temporary used in mandelbrot recurrence computation
    int maxiterations;           //-- Number of iterations use to identify points in the set
    int iterations;              //-- Recurrence iteration counters
    int pheight;                 //-- Counter for loop over pixels down imaginary axis
    int pwidth;                  //-- Counter for loop over pixels across real axis
    int i;                       //-- Loop index for recurrence relation
    FILE *fp;                    //-- Descriptor for file to hold image
    int colorscheme;             //-- Color scheme for the plot
    char printBuf[PRINTBUFSIZE]; //-- Output buffer
    double complex end;
    //
    // -- Process the input arguments
    //
    if (argc != 8)
    {
        printf("pmandel tlr tli side-length max-iterations pixels image-file nprocs\n");
        exit(1);
    }
    start = strtod(argv[1], NULL) + strtod(argv[2], NULL) * I; // I is the imaginary value
    printf("Top left coordinate is: %f + %fi\n", creal(start), cimag(start));
    range = strtod(argv[3], NULL);
    if (range <= 0)
    {
        printf("Range must be greater than zero.\n");
        exit(1);
    }
    printf("Length of a side: %f\n", range);
    maxiterations = atoi(argv[4]);
    if (maxiterations < 2)
    {
        printf("Max iterations must be at least 2.\n");
        exit(1);
    }
    pixels = atoi(argv[5]);
    printf("Pixels per side: %d\n", pixels);
    if (pixels < 48)
    {
        printf("Pixels must be at least 48.\n");
        exit(1);
    }

    int numberOfProcesses = atoi(argv[7]);
    int childRowPixels;
    double complex imaginaryRowHeight;
    double horizontalLength;
    FILE *fpChild;
    char fileName[10];
    end = start + range;
    childRowPixels = pixels / numberOfProcesses;
    imaginaryRowHeight = (cimag(start) * 2 / numberOfProcesses) * I;
    horizontalLength = range;

    char *childFiles[numberOfProcesses];
    createFileName(argv[6], childFiles, numberOfProcesses);

    fp = fopen(argv[6], "w"); /* b - binary mode */
    if (fp == NULL)
    {
        printf("%s cannot be opened for write\n", argv[6]);
    }
    (void)fprintf(fp, "P6\n%d %d\n255\n", pixels, pixels);
    fflush(fp);

    // printf("End: %f, Child row pixels %d, Imaginary Row Height: %f, horizontalLength: %d\n", creal(end), childRowPixels, imaginaryRowHeight, horizontalLength);
    int count = 0;

    while (count < numberOfProcesses)
    {
        // Can cap it at 16 processes
        int pid = fork(); // Parent id is nonzero. Child id is zero.
        if (pid == 0)
        {
            fpChild = openChildFile(fpChild, childFiles[count], colorscheme, printBuf, pixels, childRowPixels);

#ifndef COLORSCHEME
            colorscheme = 1;
#else
            colorscheme = COLORSCHEME;
#endif

            // vv Copied vv
            snprintf(printBuf, PRINTBUFSIZE, "Process %d testing rectangle at %.8f + %.8f \n\twidth %.8f and height %.8f \n\tplot area width %d by height %d pixels.\n", getpid(), creal(start), cimag(start), range, cimag(imaginaryRowHeight), pixels, childRowPixels);
            write(1, printBuf, strlen(printBuf));
            //
            // -- Start the mandelbrot set identification calculation
            //    One to one correspondance between pixels ad points
            //    in the plane to be evaluated.  Divde the range by
            //    the number of pixels on a side to identify points.
            //    Start at te left corner and work down the imaginary
            //    axis by rwo. Only square spaces in the real-imaginary
            //    plane.
            //

            z = 0;
            size = 0;
            increment = range / ((double)pixels - 1); //-- How far we move at each step
            for (pheight = 0; pheight < childRowPixels; pheight++)
            {
                c = start - (double)pheight * increment * I;
                for (pwidth = 0; pwidth < pixels; pwidth++)
                {
#ifdef DEBUGCALC
                    printf("\nc = %f + %f\n", creal(c), cimag(c));
#endif
                    z = 0;
                    iterations = 0;
                    for (i = 1; i <= maxiterations; i++)
                    {
                        z = cpow(z, 2) + c;
#ifdef DEBUGCALC
                        printf("z^2 = %f + %f\n", creal(z), cimag(z));
#endif
                        size = cabs(z);
#ifdef DEBUGCALC
                        printf("size of z is %f\n", size);
#endif
                        if (size > 2.0)
                        {
                            iterations = i;
                            break;
                        }
                    }
                    if (i > maxiterations)
                        iterations = maxiterations;
#ifndef DEBUGCALC
                    addPoint(colorscheme, iterations, maxiterations, fpChild);
#endif
                    c = c + increment + 0 * I;
                }
            }
            snprintf(printBuf, PRINTBUFSIZE, "Process %d done.\n", getpid());
            write(1, printBuf, strlen(printBuf));
            exit(0); // Child return 0 if sucessfully executed.
        }

        start = start - imaginaryRowHeight;
        count++;
    };

    while (wait(NULL) != -1 || errno != ECHILD)
    {
        printf("Waited for a child to finish\n");
    }

    for (int i = 0; i < numberOfProcesses; i++)
    {
        fpChild = fopen(childFiles[i], "r");
        concatenateFiles(fp, fpChild);
    }
    fclose(fp);
}