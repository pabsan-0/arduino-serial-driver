main: main.c asd/** asd_common/**
	gcc -Wall -Wpedantic -g3 -Iasd_common/include -Iasd/include asd/src/* main.c -o main

clean:
	rm main
