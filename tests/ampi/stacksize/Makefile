-include ../../common.mk
CHARMC=../../../bin/ampicc $(OPTS)

all: pgm

pgm: test.o
	$(CHARMC) -o pgm test.o

test.o: test.c
	$(CHARMC) -c test.c

clean:
	rm -f *.o *.mod pgm *~ conv-host charmrun charmrun.exe pgm.exe pgm.pdb pgm.ilk ampirun

test: pgm
	$(call run, ./pgm +p2 +vp4 )
