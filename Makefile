#My graphic examples in C

godseye:
	gcc -o godseye -Lqdbmp -Iqdbmp godseye.c graphlib.c qdbmp/qdbmp.c 

gradient:
	gcc -I qdbmp gradient.c -o gradient

n-gon2:
	gcc -o ngon2 -Lqbbmp -Iqdbmp n-gon2.c graphlib.c qdbmp/qdbmp.c 