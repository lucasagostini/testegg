CC=gcc
CFLAGS=-std=c11 -Wall
OUTDIR=bin

$(OUTDIR)/cryfs.o: cryfs.h cryfs.c
	mkdir -p $(OUTDIR) || echo ''
	$(CC) -c $(CFLAGS) cryfs.c -o $@

teste: clean $(OUTDIR)/cryfs.o simpletest.h teste.c
	$(CC) $(CFLAGS) teste.c $(OUTDIR)/cryfs.o -o $(OUTDIR)/$@
	./$(OUTDIR)/$@

teste_antigo: clean $(OUTDIR)/cryfs.o simpletest.h teste.c
	$(CC) $(CFLAGS) teste_semestre_passado.c $(OUTDIR)/cryfs.o -o $(OUTDIR)/$@
	./$(OUTDIR)/$@

clean:
	rm -f *.o $(OUTDIR)/* dc/* atest* cripto*
