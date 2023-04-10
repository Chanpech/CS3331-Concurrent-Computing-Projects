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



#define PRINTBUFSIZE 241
#define COLORS 15 
unsigned char palette[COLORS][3] = {{255,255,255},{255,0,0},{255,128,0},{255,255,0},{128,255,0},{0,255,0},{0,255,128},{0,255,255},{0,128,255},{0,0,255},{128,0,225},{255,0,255},{255,0,128},{128,128,128},{0,0,0}};

void addPoint(int scheme, int iterations, int maxiterations, FILE *fp){

  unsigned char color[3];
  int index;
  
  if (scheme==1){
      // Scheme 1 - Greyscale uniform 
      color[0]=255 - ((double)iterations/(double)maxiterations)*255;
      color[1]=255 - ((double)iterations/(double)maxiterations)*255;
      color[2]=255 - ((double)iterations/(double)maxiterations)*255;
  }
  if ((scheme==2)||(scheme==3)){
       if (scheme==2){
            iterations=sqrt( (double)iterations/(double)maxiterations)*(float)(maxiterations);
       }
       
       if (iterations==maxiterations)
	        index=COLORS-1; //-- Points in the set are always black, assumed last in palette
       else
            index=iterations/(maxiterations/(COLORS-1));

       color[0]=palette[index][0];
       color[1]=palette[index][1];
       color[2]=palette[index][2];
  }            
	 
  fputc(color[0],fp);
  fputc(color[1],fp);
  fputc(color[2],fp);
}
int main(int argc, char *argv[]){
  
  double complex start;  //-- Top left corner
  double range;          //-- Length of a side in complex plane
  int pixels;            //-- Pixels per side
  double increment;      //-- Length between pixels in complex plane
  double complex z;      //-- Temporary used in mandelbrot recurrence computation
  double complex c;      //-- Temporary used in mandelbrot recurrence computation
  double size;           //-- Temporary used in mandelbrot recurrence computation
  int maxiterations;     //-- Number of iterations use to identify points in the set
  int iterations;        //-- Recurrence iteration counters
  int pheight;           //-- Counter for loop over pixels down imaginary axis
  int pwidth;            //-- Counter for loop over pixels across real axis
  int i;                 //-- Loop index for recurrence relation
  FILE *fp;              //-- Descriptor for file to hold image
  int colorscheme;       //-- Color scheme for the plot
  char printBuf[PRINTBUFSIZE];    //-- Output buffer  
  //
  // -- Process the input arguments
  //
  if (argc!=7){
    printf("mandel RealCoord ImagCoord SideLength EscapeIterations PixelsPerSide ImageFile\n");
    exit(1);
  } 
  start=strtod(argv[1],NULL)+strtod(argv[2],NULL)*I;
  printf("Top left coordinate is: %f + %fi\n", creal(start), cimag(start));  
  range=strtod(argv[3],NULL);
  if (range<=0){printf("Range must be greater than zero.\n");exit(1);}
  printf("Length of a side:  %f\n",range);
  maxiterations=atoi(argv[4]);
  if (maxiterations<2){printf("Max iterations must be at least 2.\n");exit(1);}
  pixels=atoi(argv[5]);
  printf("Pixels per side:  %d\n",pixels);
  if (pixels<48){printf("Pixels must be at least 48.\n");exit(1);}

  fp=fopen(argv[6], "wb"); /* b - binary mode */
  if (fp==NULL){printf("%s cannot be opened for write\n",argv[6]);}
  (void) fprintf(fp, "P6\n%d %d\n255\n", pixels, pixels);  

#ifndef COLORSCHEME
    colorscheme=1;
#else
    colorscheme=COLORSCHEME;
#endif

    snprintf(printBuf,PRINTBUFSIZE,"Process %d testing rectangle at %.8f + %.8f \n\twidth %.8f and height %.8f \n\tplot area width %d by height %d pixels.\n",getpid(),creal(start),cimag(start),range,range,pixels,pixels);
    write(1,printBuf,strlen(printBuf));
  //
  // -- Start the mandelbrot set identification calculation 
  //    One to one correspondance between pixels ad points
  //    in the plane to be evaluated.  Divde the range by
  //    the number of pixels on a side to identify points. 
  //    Start at te left corner and work down the imaginary
  //    axis by rwo. Only square spaces in the real-imaginary
  //    plane.
  //
  z=0;
  size=0;
  increment=range/((double)pixels-1);  //-- How far we move at each step
  printf("Increment %f\n", increment);
  for (pheight=0;pheight<pixels;pheight++){
      c=start-(double)pheight*increment*I;
      for (pwidth=0;pwidth<pixels;pwidth++){
#ifdef DEBUGCALC	
	       printf("\nc = %f + %f\n", creal(c), cimag(c));
#endif
           z=0;
	       iterations=0;
           for (i=1;i<=maxiterations;i++){
                z=cpow(z,2)+c;
#ifdef DEBUGCALC	  
	            printf("z^2 = %f + %f\n", creal(z), cimag(z));
#endif	  
	            size=cabs(z);
#ifdef DEBUGCALC
	            printf("size of z is %f\n",size);
#endif	  
	            if (size>2.0){
	                 iterations=i;
	                 break;
	            }
           }
	       if (i>maxiterations)iterations=maxiterations;
#ifndef DEBUGCALC	
	       addPoint(colorscheme,iterations,maxiterations,fp);
#endif	
           c=c+increment+0*I;
      }
  }
  snprintf(printBuf,PRINTBUFSIZE,"Process %d done.\n",getpid());
  write(1,printBuf,strlen(printBuf));
}
