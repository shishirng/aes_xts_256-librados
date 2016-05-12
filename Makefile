CC=gcc
IDIR=./
CFLAGS=-I$(IDIR)
OBJ = rados_aes.c aes_xts.c
DEPS=aes_xts.h
LIBS=-lrbd -lrados -lssl -lcrypto

%.o: %c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) $(LIBS)

encrypt:$(OBJ)
	gcc -o $@ $^ $(CFLAGS) $(LIBS)
