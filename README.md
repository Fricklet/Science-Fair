# Science Fair Project
Take a look at my mediocre and possibly buggy code.

## WARNING!
If you are going to use this code for anything other than comparing results, for example in your own program, please use the [Patched](https://github.com/Fricklet/Science-Fair/tree/Patched) branch. It has optimizitations that the main branch doesn't, in the interest of archiving the original code used in the experiment.

## Platform
This code was compiled, tested, and written for Linux. While the code may compile and run fine on different operating systems (or at least with a little bit of tweaking), I'm _probably_ not going to put in direct effort to port it.

## What's in it
- Perlin.*: An implementation of Perlin in a language. _Generates a 512x512 Perlin Noise file with 8 octaves_
- Sieve.*: An implementation of the Sieve of Eratosthenes in a language. _Requires a number argument to count up to._
- Factorize.*: An implementation of prime factorization in a language. _Requires a number argument to factorize._
- noise-viewer.c: A way to view the Perlin noise files that are generated. _Requires a width, height, and file to read from. Uses SDL._
- gen_permutation.c: A way to generate a permutation table for Perlin Noise. _Requires a magnitude, Use 256 for it to work with these Perlin programs._

