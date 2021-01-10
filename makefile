CC = gcc

run: main.c
	$(CC) -o main main.c -lwiringPi
	sudo ./main