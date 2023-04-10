
//-----------------------------------------------------------------
//  Author:         Chanpech Hoeng
//  Course:         CS3331 R01
//  Latest update:    2/17/23
//
// Computes members of mandelbrot for specified square region
// of the real/imaginary plane. Creates PPM image file
// according to one of several color schemes.
//-----------------------------------------------------------------
//
// Compilation: gcc -o mandel mandel.c -lm
//
//-----------------------------------------------------------------
//
// Invocation: mandel RealCoord ImagCoord SideLength EscapeIterations PixelsPerSide ImageFile
//
// See the project handout for a detailed description of the parameters and
// operation of the program.
//------------------------------------------------------------------
#include <complex.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include "mandel.h"
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <stddef.h>

#define PRINTBUFSIZE 1000

int main(int argc, char *argv[])
{
  double complex start;        //-- Top left corner
  float wSide;                //-- Length of a side in complex plane
  float hSide;
  int wPixels;                 //--Width pixels per side
  int hPixels;                 //--Height pixels per side
  double increment;            //-- Length between pixels in complex plane
  double complex z;            //-- Temporary used in mandelbrot recurrence computation
  double complex c;            //-- Temporary used in mandelbrot recurrence computation
  double size;                 //-- Temporary used in mandelbrot recurrence computation
  int maxiterations;           //-- Number of iterations use to identify points in the set
  int iterations;              //-- Recurrence iteration counters
  int pheight;                 //-- Counter for loop over pixels down imaginary axis
  int pwidth;                  //-- Counter for loop over pixels across real axis
  int i, j;                    //-- Loop index for recurrence relation
  FILE *fp;                    //-- Descriptor for file to hold image
  char printBuf[PRINTBUFSIZE]; //-- Output buffer
  int *iterationMap;           //-- Maps # of iterations to # of points that escaped at that iteration count
  int **pointCounts;           //-- Escape iterations for each point
  int *iterSpecMap;            //-- Maps iteration count to a location in the spectrum
  int total;                   //-- Total number of points

  //
  // -- Process the input arguments
  //
  if (argc != 8)
  {
    printf("mandelc RealCoordUpperLeft ImagCoordUpperLeft SideWidthRealAxis SideHeightImaginaryAxis MaxIterations PixelsWidth PixelsHeigh\n");
    exit(1);
  }
  start = strtod(argv[1], NULL) + strtod(argv[2], NULL) * I;
  //printf("Top left coordinate is: %f + %fi\n", creal(start), cimag(start));
  wSide = strtod(argv[3], NULL);
  hSide = strtod(argv[4], NULL);
  //printf("Side real width and imaginary height is: %f + %fi\n", creal(side), cimag(side));
  //printf("Side real width and imaginary height is: %f + %fi\n", wSide, hSide);
  maxiterations = atoi(argv[5]);
  if (maxiterations < 2)
  {
    printf("Max iterations must be at least 2.\n");
    exit(1);
  }
  wPixels = atoi(argv[6]);
  //printf("Width pixels:  %d\n", wPixels);
  hPixels = atoi(argv[7]);
  //printf("Height pixels:  %d\n", hPixels);

  // Intialize key to access the shared memory space
  key_t ShmKEY;
  int ShmID;
  int **points; // The same as pointcount
  int arraysize;
  unsigned long long rowstart; // Will likely need to change the type
  char *pointprintbuf;
  int phtemp = hSide * hPixels;
  int lastRowPixel = phtemp + hPixels;

  //printf("phtemp: %d lastrow: %d\n", phtemp, lastRowPixel);
  //printf("Top left coordinate is: %f + %fi\n", creal(start), cimag(start));
  
  //-- Shared memory block contains row pointers and rows of integers.
  //-- Row pointers at lowest address.
  //          Pointers occupy pixels*sizeof(int *)
  //-- Row values next.
  //          Each row occupies pixels*sizeof(int) bytes
  //-- So row 0 values start at address points+pixels*sizeof(int *)
  //      row X values at (points+pixels*sizeof(int *)+ X*pixels(sizeof(int))
  //

  ShmKEY = ftok("./", 'x');
  arraysize = sizeof(int *) * wPixels + sizeof(int) * wPixels * wPixels; // This child process will be able to access every elements within the shared array.
  ShmID = shmget(ShmKEY, arraysize, 0666);
  points = (int **)shmat(ShmID, NULL, 0);

  // End of memory initialization

#ifdef SLEEPINT
  sleep(8);
#endif

  for (i = phtemp; i < lastRowPixel; i++)
  {
    rowstart = (unsigned long long)points + wPixels * sizeof(int *) + i * wPixels * sizeof(int);
    points[i] = (int *)rowstart;
    // printf("[%d]Row %d at address %p. Stored at %p\n", getpid(), i, points[i], &points[i]);
  }
  //printf("realNum %f, phtemp %d, last row Pixel %d\n", realNum, phtemp, lastRowPixel);
 
   //
  // -- Fill in the array by the mandelbrot set identification calculation 
  //    One to one correspondance between pixels ad points
  //    in the plane to be evaluated.  Divde the range by
  //    the number of pixels on a side to identify points. 
  //    Start at te left corner and work down the imaginary
  //    axis by rwo. Only square spaces in the real-imaginary
  //    plane.
  //
  z = 0;
  size = 0;
  increment = wSide / ((double)wPixels -1); //-- How far we move at each step
  for (pheight = phtemp; pheight < lastRowPixel; pheight++)
  {
#ifdef DUMPPOINTS
    snprintf(printBuf +  strlen(printBuf), sizeof(printBuf),"\nRow %d", pheight);
#endif

    c = start - (double)(pheight) * increment * I;
    for (pwidth = 0; pwidth < wPixels; pwidth++)
    {
      z = 0;
      iterations = 0;
      for (i = 1; i <= maxiterations; i++)
      {
        z = cpow(z, 2) + c;
        size = cabs(z);
        if (size > 2.0)
        {
          iterations = i;
          break;
        }
      }

      if (i > maxiterations)
        iterations = maxiterations;
      //-- NEW
      points[pheight][pwidth] = iterations; //-- Filling in pointCounts
                                            //-- In the previous project just computed color
                                            //-- for the point and addded point to PPM file
      c = c + increment + 0 * I;

#ifdef DUMPPOINTS
      snprintf(printBuf +  strlen(printBuf), sizeof(printBuf), "[%d] %d ", pwidth, points[pheight][pwidth]);
#endif
    }
  }

#ifdef DUMPPOINTS
    write(1, printBuf, strlen(printBuf));
    write(1, "\n", 2);
#endif

  //close child process share memory segment
  shmdt((void *)points);
  exit(0);
}
