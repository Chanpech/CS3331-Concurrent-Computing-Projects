//------------------------------------------------
//
//    Demonstrates how to capture pixel data portion of PPM file and write it to another file
//    Assumes a three line PPM file header.  No error checking.
//
//    findEnd file.ppm
//------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
int main(int argc, char *argv[])
{
  FILE *fp;        //-- Will hold just pixel data.  Approach 1
  FILE *fp2;       //-- Will hold just pixel data.  Approach 2
  char buf[81];    //-- Holds header line.  Thrown away.
  char *contents;  //-- Holds data after the header -- just pixel info
  long fileSize;   //-- File size in bytes
  long pixelStart; //-- Offset of pixel data; from start of file
  int byte;        //-- Holds a byte of data
  long pixelDataSize;  //-- Size of pixel data in bytes
  
  //-- Find the size of the file.
  fp=fopen(argv[1],"r");
  fseek(fp,0,SEEK_END);
  fileSize=ftell(fp);
  printf("File has %d bytes\n",fileSize);
  
  //-- Move past the header.  Know its three lines.
  fseek(fp,0,SEEK_SET);
  fgets(buf,81,fp);  printf("Buf <%s>\n",buf);
  fgets(buf,81,fp);  printf("Buf <%s>\n",buf);
  fgets(buf,81,fp);  printf("Buf <%s>\n",buf);

  //-- Remainder of file is pixel data. Determine the size so we can read it all at once. 
  pixelStart=ftell(fp); //-- Start of pixel data.  Immediately after the header.
  pixelDataSize=fileSize-pixelStart;  
  contents=malloc(pixelDataSize); //-- Allocate space to hold the contents
  printf("Reading %d bytes of pixel data\n",pixelDataSize);
  fread(contents,1,pixelDataSize,fp); //-- Get the pixel data

  fclose(fp);
  fp=fopen("pixelData","w");  //-- This has only the pixel data.
  fwrite(contents,1,pixelDataSize,fp);
  fclose(fp);
  
  //-------------------------------------------------------------------------
  // Approach 2.  Higher overhead but less knowledge of FS interface required.
  //-------------------------------------------------------------------------

  fp=fopen(argv[1],"r");
  fp2=fopen("pixelDataSlow","w");
  
  //-- Move past the header.  Know its three lines
  fgets(buf,81,fp);  printf("Buf <%s>\n",buf);
  fgets(buf,81,fp);  printf("Buf <%s>\n",buf);
  fgets(buf,81,fp);  printf("Buf <%s>\n",buf);

  byte=fgetc(fp);
  while (byte!=EOF){fputc(byte,fp2);byte=fgetc(fp);
    }
  
  fclose(fp);
  fclose(fp2);  
      
}
