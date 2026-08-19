object := main.o		
object += hello.o
.PHONY:clean

main:$(object)
	gcc $(object) -o main



hello.o:hello.c
	gcc -c hello.c -o hello.o


main.o:main.c hello.h
	gcc -c main.c -o main.o

clean:
	rm main $(object)

clean2:
	-rm main.o
	-rm hello.o
	-rm main

