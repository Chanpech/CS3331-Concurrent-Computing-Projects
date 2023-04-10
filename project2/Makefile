all: pmandel-2 mandelc

pmandel-2: pmandel-2.o iterMap.o  iterSpectrumMap.o  spectrumToRGB.o 
	gcc -g -o pmandel-2 pmandel-2.o iterMap.o iterSpectrumMap.o spectrumToRGB.o  -lm

mandelc: mandelc.o
	gcc -g -o mandelc mandelc.o -lm

pmandel-2.o: pmandel-2.c
	gcc -g -c pmandel-2.c

mandelc.o: mandelc.c
	gcc -g -c mandelc.c

iterMap.o: iterMap.c
	gcc -g -c iterMap.c

iterSpectrumMap.o: iterSpectrumMap.c
	gcc -g -c iterSpectrumMap.c

spectrumToRGB.o: spectrumToRGB.c
	gcc -g -c spectrumToRGB.c

clean:
	rm pmandel-2 mandelc *.

piclean:
	rm pmandel-2 mandelc *.o *.ppm

submission: 
	gtar czvf prog2.tgz Makefile pmandel-2.c mandelc.c iterMap.c iterSpectrumMap.c spectrumToRGB.c mandel.h

dump: pmandel-2 mandelc_dump

mandelc_dump: mandelc_dump.o
	gcc -g -o mandelc mandelc.o -lm
mandelc_dump.o: mandelc.c
	gcc -g -c  -DDUMPPOINTS mandelc.c

watch: pmandel-2 mandelc_watch

mandelc_watch: mandelc_watch.o
	gcc -g -o mandelc mandelc.o -lm
mandelc_watch.o: mandelc.c
	gcc -g -c  -DSLEEPINT mandelc.c