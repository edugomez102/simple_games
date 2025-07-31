
RUN := 1
FLAGS := -g -std=c11
UNAME_S := $(shell uname -s)


ifeq ($(UNAME_S),Linux)
        FLAGS += -lSDL2
    endif
    ifeq ($(UNAME_S),Darwin)
        FLAGS += -I/opt/homebrew/include -L/opt/homebrew/lib -lSDL2
    endif

game: main.c
	gcc -o game main.c $(FLAGS)
	@if [ $(RUN) = 1 ]; then \
		./game; \
	fi

clean:
	rm -rf game


# snake: src/main.c
# 	gcc -o snake src/main.c utils.c 


# draft: draft.c utils.c
# 	gcc -o draft draft.c utils.c
# 	./draft

# clean:
# 	rm snake
