.PHONY: clean
build:
	gcc parallel_snake.c main.c -o parallel_snake -fopenmp -lm


clean:
	rm -f parallel_snake
