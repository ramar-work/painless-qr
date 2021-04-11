#
PREFIX=/usr/local
BINDIR=$(PREFIX)/bin
CC = clang
DFLAGS = -g -O0 -fsanitize=address -fsanitize-undefined-trap-on-error -DDEBUG_H
CFLAGS = $(DFLAGS) -Wall -Werror -Wno-unused -Wno-format-security -std=c99 -Ivendor


# qr - a QR code generator
qr: vendor/qrcodegen.o
	-test -d bin/ || mkdir bin/
	$(CC) $(CFLAGS) -o bin/pqr main.c vendor/qrcodegen.o 


# install - Install scripts 
install:
	cp bin/pqr $(PREFIX)/bin/


# clean - Clean stuff
clean:
	-find -type f -name "*.o" | xargs rm



