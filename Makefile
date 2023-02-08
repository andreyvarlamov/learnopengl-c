a.out: main.o glad/glad.o
	gcc -o a.out main.o glad/glad.o -I include -lglfw

main.o: src/main.c include/glad/glad.h
	gcc -o main.o -c src/main.c -I include

glad/glad.o: glad/src/glad.c include/glad/glad.h
	gcc -o glad/glad.o -c glad/src/glad.c -I include

.PHONY: run
run: a.out
	@./a.out

.PHONY: clean
clean:
	@rm a.out main.o glad/glad.o
