all: pmandel.c 
	gcc -DCOLORSCHEME=1 -lm -o pmandel pmandel.c 

debug: pmandel.c 
	gcc -DCOLORSCHEME=1 -DDEBUGCALC -lm -o pmandel pmandel.c 

submission: 
	gtar czvf prog1.tgz Makefile pmandel.c

clean: 
	rm pmandel
	rm -f *.ppm
