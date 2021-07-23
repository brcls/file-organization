all:
	gcc dados.h arvoreb.c juncaoEordenacao.c auxiliares.c veiculo.c linha.c main.c -g -o programaTrab

val:
	valgrind --main-stacksize= -s --track-origins=yes --main-stacksize= ./programaTrab
run:;
	./programaTrab
