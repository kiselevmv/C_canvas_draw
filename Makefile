#My graphic examples in C
CC = gcc # Flag for implicit rules
CFLAGS = -g # Flag for implicit rules. Turn on debug info
FULL_WARNINGS =  \
        -std=gnu89 \
        -fsigned-char \
        -fno-builtin \
        -fno-unroll-loops \
        -pedantic \
        -Wcast-qual \
        -Wall \
        -Wwrite-strings \
        -Winline  \
        -Wmissing-prototypes \
        -Wmissing-declarations
DEBUGOPTS = -g
CFLAGS = -x c -O2 $(FULL_WARNINGS) $(DEBUGOPTS)

VPATH = .:qdbmp

CODESOURCE = qdbmp/qdbmp.c graphlib.c


godseye:
	$(CC) $(CFLAGS) $^ -Iqdbmp godseye.c $(CODESOURCE) -o $@ 

gradient:
	$(CC) $(CFLAGS) $^ -Iqdbmp gradient.c $(CODESOURCE) -o $@ 

n-gon2:
	$(CC) $(CFLAGS) $^ -Iqdbmp n-gon2.c $(CODESOURCE) -o $@

clean:
	rm -f gradient ngon2 godseye output.bmp

all: godseye gradient n-gon2