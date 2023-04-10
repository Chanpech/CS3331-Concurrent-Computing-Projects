//------------------------------------------------------------------
//  Author:         Chanpech Hoeng
//  Course:         CS3331 R01
//  Latest update:    2/17/23
//
//  Description:    pmandel-2.c parallelize the calculation of points by calling mandelc.c in fork/exec.
//                  mandelc.c produces the Mandelbrot and stored it value using share memory allocated by an array.
//                  Creates PPM imagefile according to one of several color schemes.
//                
//  Compilation:    make, make clean, make piclean, make dump, make watch
//
//  Invocation:     pmandel-2 RealCoord ImagCoord SideLength EscapeIterations PixelsPerSide ImageFile NumberOfProcesses
//
//  See the project handout for a detailed description of the parameters and
//                  operation of the program.
//------------------------------------------------------------------
#include <complex.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include "mandel.h"
#include <sys/wait.h>
#include <errno.h>
#include <stddef.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define PRINTBUFSIZE 241

int main(int argc, char *argv[])
{
  // Professor wanted us to use int execvp(const char *file, char *const argv[]);
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
  char printBuf[PRINTBUFSIZE]; //-- Output buffer
  int *iterationMap;           //-- Maps # of iterations to # of points that escaped at that iteration count
  int *iterSpecMap;            //-- Maps iteration count to a location in the spectrum
  int total;                   //-- Total number of points

  int numberOfProcesses;

  //
  // -- Process the input arguments
  //
  if (argc != 8)
  {
    printf("pmandel RealCoord ImagCoord SideLength EscapeIterations PixelsPerSide ImageFile NumberOfProcesses\n");
    exit(1);
  }
  start = strtod(argv[1], NULL) + strtod(argv[2], NULL) * I;
  printf("Top left coordinate is: %f + %fi\n", creal(start), cimag(start));
  range = strtod(argv[3], NULL);
  if (range <= 0)
  {
    printf("Range must be greater than zero.\n");
    exit(1);
  }
  printf("Length of a side:  %f\n", range);
  maxiterations = atoi(argv[4]);
  if (maxiterations < 2)
  {
    printf("Max iterations must be at least 2.\n");
    exit(1);
  }
  pixels = atoi(argv[5]);
  printf("Pixels per side:  %d\n", pixels);
  if (pixels < 10)
  {
    printf("Pixels must be at least 10.\n");
    exit(1);
  }
  numberOfProcesses = atoi(argv[7]);

  fp = fopen(argv[6], "wb"); /* b - binary mode */
  if (fp == NULL)
  {
    printf("%s cannot be opened for write\n", argv[6]);
  }
  (void)fprintf(fp, "P6\n%d %d\n255\n", pixels, pixels);

 
  // Share memory initializations
  key_t ShmKEY;
  int ShmID;
  int **points;
  int arraysize;
  unsigned long long rowstart;
  int j;
  int cpid;
  int estatus;
  //Calculation variables
  int childRowPixels;
  double complex imaginaryRowHeight;
  double complex imaginarySideHeight;
  double complex end; 
  double complex side;

  end = start + range;
  childRowPixels = pixels / numberOfProcesses;
  imaginaryRowHeight = (cimag(start) * 2 / numberOfProcesses) *I;
  imaginarySideHeight = 1* I;
  side = range + (range/numberOfProcesses)*I;
  pwidth = pixels;
  pheight = childRowPixels;



  ShmKEY = ftok("./", 'x');
  arraysize = sizeof(int *)* pixels + sizeof(int) * pixels * pixels; // I think this is correct because we are allocating the entire pixels in the canvas.
  ShmID = shmget(ShmKEY, arraysize, IPC_CREAT | 0666); // Access the shared memory through this variable.
  points = (int **)shmat(ShmID, NULL, 0);
  //printf("Shared start: %p sizeof(int *) %d sizeof(int) %d\n", points, sizeof(int *), sizeof(int));
  // End of initialized shared memory

  // -- Start the parallization of mandelbrot set by converting its identification calculation 
  //    to mandelc.c file. The main process would then utlize fork and execlp methods to run the program
  //    in concurrency.
  int count = 0;
  while (count < numberOfProcesses)
  {
    cpid = fork(); // Parent id is nonzero. Child id is zero.
    if (cpid == 0)
    {
      // Convert to string
      char realCoord[20], imagCoord[20], sideWidth[20], sideHeight[20], maxIterat[20], pixelWidth[20], pixelHeight[20];
      
      snprintf(printBuf,PRINTBUFSIZE,"Process %d testing rectangle at %.8f + %.8f \n\twidth %.8f and height %.8f \n\tplot area width %d by height %d pixels.\n",getpid(),creal(start),cimag(start),creal(side),cimag(side),pwidth,pheight);
      write(1,printBuf,strlen(printBuf));

      snprintf(realCoord, sizeof(realCoord), "%f", creal(start));
      snprintf(imagCoord, sizeof(imagCoord), "%f", cimag(start));
      snprintf(sideWidth, sizeof(sideWidth), "%f", creal(side));
      snprintf(sideHeight, sizeof(sideHeight), "%d", count); //notice that the side height variable is not used and replaced by count for parallization
      snprintf(maxIterat, sizeof(maxIterat), "%d", maxiterations);
      snprintf(pixelWidth, sizeof(pixelWidth), "%d", pwidth);
      snprintf(pixelHeight, sizeof(pixelHeight), "%d", pheight);

      execlp("./mandelc", "./mandelc", realCoord, imagCoord, sideWidth, sideHeight, maxIterat, pixelWidth, pixelHeight, NULL);
      perror("ERROR: ");
      exit(1);
    }
    count++;
  }
  while (wait(&estatus) != -1 || errno != ECHILD)
  {
    //printf("Waited for a child to finish\n");
  }


  //Current process at row i at address is stored at current address of the pointer
   for (i=0;i<pixels;i++){
       rowstart=(unsigned long long)points+pixels*sizeof(int *)+i*pixels*sizeof(int);
       points[i]=(int *)rowstart;
       //printf("[%d]Row %d at address %p. Stored at %p\n",getpid(),i,points[i],&points[i]);
     }
  
  //Utlized given color scheme files to write the file 
  total=pixels * pixels;
  iterationMap=iterMap(points, pixels, pixels,maxiterations);
  iterSpecMap=iterSpectrumMap(iterationMap,maxiterations);  
  spectrumToRGB(points, pixels, pixels,maxiterations,iterSpecMap,fp); 

  //Close main process share memory segment
  shmdt((void *) points);
  shmctl(ShmID, IPC_RMID, NULL);
  exit(0);

}