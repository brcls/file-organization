all:
	gcc dados.h arvoreb.c auxiliares.c veiculo.c linha.c main.c -g -o programaTrab

val:
	valgrind -s --track-origins=yes ./programaTrab
run:;
	./programaTrab
