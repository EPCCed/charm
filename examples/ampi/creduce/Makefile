-include ../../common.mk
CHARMC=../../../bin/ampicxx $(OPTS)

all: pgm

ampi: clean pgm

pgm: test.C
	$(CHARMC) -c test.C
	$(CHARMC) -o pgm test.o 

mpi: test.C
	mpiCC -o pgm test.C $(OPTS)

clean:
	rm -f *.o *.mod pgm *~ conv-host charmrun ampirun

test: pgm
	$(call run, ./pgm +p1 +vp1 )
	$(call run, ./pgm +p1 +vp2 )
	$(call run, ./pgm +p1 +vp4 )
	$(call run, ./pgm +p2 +vp1 )
	$(call run, ./pgm +p2 +vp2 )
	$(call run, ./pgm +p2 +vp4 )
