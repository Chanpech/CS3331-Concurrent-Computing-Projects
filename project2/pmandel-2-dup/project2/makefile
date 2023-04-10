all: mandel-2

mandel-2: mandel-2.o  iterMap.o  iterSpectrumMap.o  spectrumToRGB.o 
	gcc -g -o mandel-2 mandel-2.o iterMap.o iterSpectrumMap.o spectrumToRGB.o  -lm

mandel-2.o: mandel-2.c
	gcc -g -c mandel-2.c

iterMap.o: iterMap.c
	gcc -g -c iterMap.c

iterSpectrumMap.o: iterSpectrumMap.c
	gcc -g -c iterSpectrumMap.c

spectrumToRGB.o: spectrumToRGB.c
	gcc -g -c spectrumToRGB.c

clean:
	rm mandel-2 *.o

piclean:
	rm mandel-2 *.o *.ppm
