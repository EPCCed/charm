-include ../../../common.mk
CHARMC=../../../../bin/charmc $(OPTS)

OBJS = hello.o hellof.o

all: pgm

pgm: $(OBJS) 
	$(CHARMC) -o pgm $(OBJS) -language f90charm -balancer TreeLB

hello.decl.h: hello.ci
	$(CHARMC) hello.ci -language f90charm

hello.o: hello.C hello.decl.h
	$(CHARMC) -c hello.C

hellof.o: hellof.f90
	$(CHARMC) -c hellof.f90

test: pgm
	$(call run, +p2 ./pgm +balancer RotateLB +LBDebug 1)

clean:
	rm -f charmrun conv-host pgm *.o *.mod *.decl.h *.def.h

