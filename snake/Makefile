
RUN        := 1

game: main.c
	gcc -o game main.c -g -lSDL2
	@if [ $(RUN) = 1 ]; then \
		./game; \
	fi

clean:
	rm -rf game
