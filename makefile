CC = gcc

run: main.c
	$(CC) -o main main.c
	sudo ./main