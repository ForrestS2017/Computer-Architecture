all: sixth

sixth: sixth.c
	gcc -Wall -Werror -fsanitize=address sixth.c -o sixth.out

clean:
	rm -rf sixth.out