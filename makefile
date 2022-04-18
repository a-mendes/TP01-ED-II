#Windows Makefile

all: main.o gerador_arquivo.o sequencial.o binaria.o b.o b_estrela.o
	@gcc ./obj/main.o ./obj/gerador_arquivo.o ./obj/sequencial.o ./obj/binaria.o ./obj/b.o ./obj/b_estrela.o -o TP1

main.o: ./src/main.c
	@gcc -c ./src/main.c -o ./obj/main.o

gerador_arquivo.o: ./src/gerador_arquivo.c
	@gcc -c ./src/gerador_arquivo.c -o ./obj/gerador_arquivo.o

sequencial.o: ./src/sequencial.c
	@gcc -c ./src/sequencial.c -o ./obj/sequencial.o

binaria.o: ./src/binaria.c
	@gcc -c ./src/binaria.c -o ./obj/binaria.o

b.o: ./src/b.c
	@gcc -c ./src/b.c -o ./obj/b.o

b_estrela.o: ./src/b_estrela.c
	@gcc -c ./src/b_estrela.c -o ./obj/b_estrela.o

run:
	@./TP1.exe

go: 
	@make all
	@make run

clean:
	@del *.exe obj data
	@cls