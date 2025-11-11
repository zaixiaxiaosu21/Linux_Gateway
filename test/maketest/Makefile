
objects = main.o hello.o 
.PHONY: clean clean2
main: $(objects)
	gcc -o main $(objects)
main.o: main.c hello.h
	gcc -c main.c -o main.o	
hello.o: hello.c hello.h
	gcc -c hello.c -o hello.o	
clean:
	-rm  $(objects) main
clean2:
	-rm main
	-rm main.o
	-rm hello.o		
	
	
	