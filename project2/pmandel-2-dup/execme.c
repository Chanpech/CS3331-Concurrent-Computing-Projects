
//-----------------------------------------------------------------
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

#define PRINTBUFSIZE 241

int main(int argc, char *argv[])
{
  printf("Start variables initialization.\n");
  double complex start;        //-- Top left corner
  double complex side;         //-- Length of a side in complex plane
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
    printf("Start inputs initialization.\n");
    printf("argc is %d\n", argc);
  
  if (argc != 8)
  {
    printf("mandelc RealCoordUpperLeft ImagCoordUpperLeft SideWidthRealAxis SideHeightImaginaryAxis MaxIterations PixelsWidth PixelsHeigh\n");
    exit(1);
  }
  start = strtod(argv[1], NULL) + strtod(argv[2], NULL) * I;
  printf("Top left coordinate is: %f + %fi\n", creal(start), cimag(start));
  side = strtod(argv[3], NULL) + strtod(argv[4], NULL) * I;
  printf("Side real width and imaginary height is: %f + %fi\n", creal(side), cimag(side));
  maxiterations = atoi(argv[5]);
  if (maxiterations < 2)
  {
    printf("Max iterations must be at least 2.\n");
    exit(1);
  }
  printf("MaxIteration: %d\n", maxiterations);
  wPixels = atoi(argv[6]);
  printf("Width pixels:  %d\n", wPixels);
  hPixels = atoi(argv[7]);
  printf("Height pixels:  %d\n", hPixels);
  if (wPixels < 10 || hPixels < 10)
  {
    printf("Width pixels and height pixels must be at least 10.\n");
    exit(1);
  }

  //-- NEW allocating space for the point counts
  //   Access is regular indexing: pointCounts[x][y]
  //
  printf("Start key initialization.\n");
  // Intialize key to access the shared memory space
  key_t ShmKEY;
  int ShmID;
  int **points; // The same as pointcount;
  int arraysize;
  unsigned long long rowstart; // Will likely need to change the type
  char *pointprintbuf;
 //-- Shared memory block contains row pointers and rows of integers.
   //-- Row pointers at lowest address.                                     
   //          Pointers occupy pixels*sizeof(int *)                         
   //-- Row values next.                                                    
   //          Each row occupies pixels*sizeof(int) bytes                   
   //-- So row 0 values start at address points+pixels*sizeof(int *)
   //      row X values at (points+pixels*sizeof(int *)+ X*pixels(sizeof(int))
   //
  ShmKEY = ftok("./", 'x');
  arraysize = sizeof(int *)* wPixels + sizeof(int)*wPixels*wPixels; // This child process will be able to access every elements within the shared array.
  ShmID = shmget(ShmKEY, arraysize, 0666);
  points = (int **)shmat(ShmID, NULL, 0);

  float realNum = cimag(side);
  int phtemp = realNum* hPixels;
  int lastRowPixel = phtemp + hPixels;

  for (i = phtemp; i < lastRowPixel; i++)
  {
    rowstart = (unsigned long long)points + wPixels*sizeof(int *) + i * wPixels * sizeof(int);
    points[i] = (int *)rowstart;
    //printf("[%d]Row %d at address %p. Stored at %p\n", getpid(), i, points[i], &points[i]);
  }
  //-- Fill in the array
  printf("realNum %f, phtemp %d, last row Pixel %d\n", realNum, phtemp, lastRowPixel);
  z=0;
  size=0;
  increment= creal(side)/((double)wPixels-1);  //-- How far we move at each step
  for (pheight=phtemp;pheight<phtemp + hPixels;pheight++){ 
      c=start-(double)(pheight-phtemp)*increment*I;
      for (pwidth=0;pwidth<wPixels;pwidth++){
           z=0;
	       iterations=0;
           for (i=1;i<=maxiterations;i++){
                z=cpow(z,2)+c;
	            size=cabs(z);
	            if (size>2.0){
	                 iterations=i;
	                 break;
	            }
           }
	   //After calculation we want to just add pheight by number of pixel.
     //This mean we keep width pixel the same.
     //
     //phtemp = sideHeight * pheight + pheight;

     //printf("realNum %f, phtemp %d", realNum, phtemp);


	      if (i>maxiterations)iterations=maxiterations;
	      //-- NEW
	      points[pheight][pwidth]=iterations; //-- Filling in pointCounts
	                                                //-- In the previous project just computed color
	                                                //-- for the point and addded point to PPM file
               c=c+increment+0*I;
        //snprintf(buf,size, "%d", argn); 
        //To write after
        //snprintf(buff.strlen(buf), size, "%d", 2000);
        printf("[%d]pixels[%d][%d]=%d at address <%p>\n",getpid(),pheight,pwidth,points[pheight][pwidth],&points[pheight][pwidth]);
      #ifdef DUMPPOINTS
        //sprintf(name, argv);
        write(1,pointprintbuf,strlen(pointprintbuf));
      #endif
        }      
  } 

 #ifdef SLEEPINT
        //sprintf(name, argv);
       sleep(8);
#endif

  shmdt((void *)points);
  exit(0);
}
