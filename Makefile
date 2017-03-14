CC=gcc
CFLAGS=-std=c11 -Wall
OUTDIR=bin

$(OUTDIR)/cryfs.o: cryfs.h cryfs.c
	$(CC) $(CFLAGS) cryfs.c -c -o $@

teste: clean $(OUTDIR)/cryfs.o simpletest.h teste.c
	$(CC) $(CFLAGS) teste.c $(OUTDIR)/cryfs.o -o $(OUTDIR)/$@
	./$(OUTDIR)/teste

clean:
	rm -f *.o $(OUTDIR)/* dc/*
