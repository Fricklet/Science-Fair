CFLAGS = -lm

all: noise pfactorization sieve

pfactorization: Factorize_C Factorize_CPP

sieve: Sieve_C Sieve_CPP

noise: Perlin_C Perlin_CPP
	gcc -o gen_permutation gen_permutation.c -Wall
	./gen_permutation 256

extra:
	gcc -o noise-viewer noise-viewer.c -lSDL2

%_C: %.c
	gcc -o $@ $< $(CFLAGS)
%_CPP: %.cpp
	g++ -o $@ $< $(CFLAGS)

clean:
	rm *_C *_CPP gen_permutation noise-viewer Noise Permutations
