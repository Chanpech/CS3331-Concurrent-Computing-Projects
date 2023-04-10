//  Author:
//  Course:
//  Last update:
//
//  Description:
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
  printf("Writing in writeChildField...\n");

  fptemp = fopen(fileName, "wb+"); /* b - binary mode */
  if (fptemp == NULL)
  {
    printf("%s cannot be opened for write\n", fileName);
    perror("Error:");
  }
  (void)fprintf(fptemp, "P6\n%d %d\n255\n", wPixels, hPixels);
  printf("Child files sucessfully opened\n");
  return fptemp;
}

void concatenateFiles(FILE *parentfp, FILE *childfp)
{
  printf("Starting files concatentation!\n");
  char c;
  while ((c = fgetc(childfp) != EOF))
  {
    fputc(c, parentfp);
  }
  printf("Files concatenate!\n");
}

void createFileName(char *argv){
  
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
    printf("pmandel RealCoord ImagCoord SideLength EscapeIterations PixelsPerSide ImageFile NumberOfProcesses\n");
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

  fp = fopen(argv[6], "wb"); /* b - binary mode */
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
    //Can cap it at 16 processes
    int pid = fork(); // Parent id is nonzero. Child id is zero.
    if (pid == 0)
    {
      // printf("Child pid:%d\n", getpid());
      sprintf(fileName, "pic%d.ppm", count);

      // printf("File: %s\n", fileName);
      fpChild = openChildFile(fpChild, fileName, colorscheme, printBuf, pixels, childRowPixels);

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
      printf("Increment %f\n", increment);
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

      /*VVVV Concatenate VVVV*/

      // fseek(fpChild, 16, SEEK_SET);
      /*
          unsigned char color[3];
          fread(color, sizeof(color), 1, fpChild);
          printf("\t%s\t\n", color);
           color[0] = fgetc(fpChild);
          color[1] = fgetc(fpChild);
          color[2] = fgetc(fpChild);
          printf("\t%s\t\n", color);
          */

      printf("Concatentate and exit!\n");
      // Before return we want the child to concatenate it to
/*
      char buf[81];       //-- Holds header line.  Thrown away.
      char *contents;      //-- Hold data after the header
      long fileSize;      //-- File size
      long pixelStart;    //-- Offset of pixel data
      int byte;           //-- Holds a byte of data
      long pixelDataSize; //-- Size of pixel data in bytes

      // Find th size of the file
      fseek(fpChild, 0, SEEK_END);
      fileSize = ftell(fpChild);
      printf("File has %d bytes\n", fileSize);

      fseek(fpChild, 0, SEEK_SET);
      fgets(buf, 81, fpChild);
      printf("Buf <%s>\n", buf);
      fgets(buf, 81, fpChild);
      printf("Buf <%s>\n", buf);
      fgets(buf, 81, fpChild);
      printf("Buf <%s>\n", buf);

      pixelStart = ftell(fpChild);
      pixelDataSize = fileSize - pixelStart;
      contents = malloc(pixelDataSize); //Allocate space to hold the contents
      printf("Reading %d bytes of pixel data\n", pixelDataSize);
      fread(contents,1, pixelDataSize, fpChild); //Get the pixel data

      fclose(fpChild);
      fwrite(contents,1, pixelDataSize,fp); 
      */
      exit(0); // Child return 0 if sucessfully executed.
    }

    start = start - imaginaryRowHeight;
    count++;
  };

  while (wait(NULL) != -1 || errno != ECHILD)
  {
    printf("Waited for a child to finish\n");
  }

  printf("Done only parent is left pid: %d\n", getpid());
}