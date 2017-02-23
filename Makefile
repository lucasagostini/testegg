CC=gcc
CFLAGS=-std=c11 -Wall
OUTDIR=bin

$(OUTDIR)/cryfs.o: cryfs.h cryfs.c
	$(CC) $(CFLAGS) cryfs.c -c -o $@

teste: $(OUTDIR)/cryfs.o simpletest.h teste.c
	$(CC) $(CFLAGS) teste.c -o $(OUTDIR)/$@
	./$(OUTDIR)/teste

clean:
	rm -f *.o $(OUTDIR)/*
