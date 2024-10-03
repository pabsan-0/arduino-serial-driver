main: main.c
	gcc -Wall -Wpedantic -g3 -Iasd_common/include -Iasd/include asd/src/* main.c -o main

clean:
	rm main
