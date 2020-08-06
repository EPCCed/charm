-include ../../common.mk
CHARMC=../../../bin/charmc $(OPTS) $(MOPTS)

OBJS = jacobi3d.o

all: jacobi3d

jacobi3d: $(OBJS)
	$(CHARMC) -language charm++ -lpmem -module TreeLB -o jacobi3d $(OBJS)

projections: $(OBJS)
	$(CHARMC) -language charm++ -tracemode projections -lz -o jacobi3d.prj $(OBJS)

summary: $(OBJS)
	$(CHARMC) -language charm++ -tracemode summary -lz -o jacobi3d.sum $(OBJS)

jacobi3d.decl.h: jacobi3d.ci
	$(CHARMC)  jacobi3d.ci

fttest: memtest pmemfsdaxtest pmempmdktest diskdirecttest  disktest

memtest: jacobi3d
	$(call run, ./jacobi3d 512 256 100 10 +p7 +balancer DummyLB )
	#$(call run, ./jacobi3d 256 128 +p7 +balancer DummyLB +killFile kill_01.txt )
	#$(call run, ./jacobi3d 256 256 256 64 64 32 +p7 +balancer DummyLB +killFile kill_02.txt )
	#$(call run, ./jacobi3d 256 256 256 64 64 32 +p7 +balancer DummyLB +killFile kill_03.txt )

pmemfsdaxtest: jacobi3d
	$(call run, ./jacobi3d 512 256 100 10 +p7 +balancer DummyLB +ftc_pmem_fsdax )
	#$(call run, ./jacobi3d 256 128 +p7 +balancer DummyLB +killFile kill_01.txt )
	#$(call run, ./jacobi3d 256 256 256 64 64 32 +p7 +balancer DummyLB +killFile kill_02.txt )
	#$(call run, ./jacobi3d 256 256 256 64 64 32 +p7 +balancer DummyLB +killFile kill_03.txt )

pmempmdktest: jacobi3d
	$(call run, ./jacobi3d 512 256 100 10 +p7 +balancer DummyLB +ftc_pmem_pmdk )
	#$(call run, ./jacobi3d 256 128 +p7 +balancer DummyLB +killFile kill_01.txt )
	#$(call run, ./jacobi3d 256 256 256 64 64 32 +p7 +balancer DummyLB +killFile kill_02.txt )
	#$(call run, ./jacobi3d 256 256 256 64 64 32 +p7 +balancer DummyLB +killFile kill_03.txt )

disktest: jacobi3d
	$(call run, ./jacobi3d 512 256 100 10 +p7 +balancer DummyLB +ftc_disk )
	#$(call run, ./jacobi3d 256 128 +p7 +balancer DummyLB +killFile kill_01.txt )
	#$(call run, ./jacobi3d 256 256 256 64 64 32 +p7 +balancer DummyLB +killFile kill_02.txt )
	#$(call run, ./jacobi3d 256 256 256 64 64 32 +p7 +balancer DummyLB +killFile kill_03.txt )

diskdirecttest: jacobi3d
	$(call run, ./jacobi3d 512 256 100 10 +p7 +balancer DummyLB +ftc_disk_direct )
	#$(call run, ./jacobi3d 256 128 +p7 +balancer DummyLB +killFile kill_01.txt )
	#$(call run, ./jacobi3d 256 256 256 64 64 32 +p7 +balancer DummyLB +killFile kill_02.txt )
	#$(call run, ./jacobi3d 256 256 256 64 64 32 +p7 +balancer DummyLB +killFile kill_03.txt )


mpisyncfttest: jacobi3d
	$(call run, ./jacobi3d 256 128 150 50 +p10 +wp8 +balancer DummyLB )
	$(call run, ./jacobi3d 256 128 150 50 +p10 +wp8 +balancer DummyLB +killFile kill_04.txt )
	$(call run, ./jacobi3d 256 256 256 64 64 32 150 50 +p10 +wp8 +balancer DummyLB +killFile kill_05.txt )

clean:
	rm -f *.decl.h *.def.h conv-host *.o jacobi3d jacobi3d.prj charmrun *~

jacobi3d.o: jacobi3d.C jacobi3d.decl.h
	$(CHARMC) -c jacobi3d.C
