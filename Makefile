#My graphic examples in C

godseye:
	gcc -o $@ -Lqdbmp -Iqdbmp godseye.c graphlib.c qdbmp/qdbmp.c 

gradient:
	gcc -o $@ -Lqdbmp -Iqdbmp gradient.c qdbmp/qdbmp.c 

n-gon2:
	gcc -o $@ -Lqbbmp -Iqdbmp n-gon2.c graphlib.c qdbmp/qdbmp.c 

clean:
	rm -f gradient ngon2 godseye output.bmp

all: godseye gradient n-gon2