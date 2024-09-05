# Simple Hash Functions

This project demonstrates simple [hash functions] written in C. The first hash
function is from Kernighan and Ritchie's [_The C Programming Language_], also
known as K&R. The second hash function is a modification of K&R's function,
simply substituting 127 for 31 (both numbers are prime).

Two different implementations of each function are compared &mdash; the
original, and an optimization.

## Building and Running

```console
$ cd demo
$ gcc -o hash_demo hash_demo.c
$ ./hash_demo

The quick brown fox jumps over the lazy dog.
================================================================================
K&R Hash            : 8473644914141546766
K&R Hash (faster)   : 8473644914141546766
New Hash            : 7837367029582263214
New Hash (faster)   : 7837367029582263214

Benchmark: 10 Million Iterations
================================================================================
K&R Hash            : 1.279303 seconds
K&R Hash (faster)   : 1.016220 seconds
New Hash            : 1.271200 seconds
New Hash (faster)   : 1.014285 seconds
```

## Hash Functions

See [hash_functions.h].

Note: These hash functions are less efficient in a hash table because they
produce more collisons (see [Hash functions: An empirical comparison]). However,
they evaluate quickly and are easy to remember. For a better, secure hashing
algorithm see [SipHash].

### `kr_hash`

The hash function from K&R (see [p. 128] for the original code):

```c
unsigned long kr_hash(char *s) {
  unsigned long h;
  for (h = 0; *s != '\0'; s++) {
    h += *s + 31 * h;
  }
  return h;
}
```

This function is equivalent to the following polynomial:

```math
31^{n-1}s_0 + 31^{n-2}s_1 + ... + 31s_{n-2} + s_{n-1}
```

where $$n$$ is the length of the string, and $$s_i$$ is the character at index
$$i$$ in the string $$s$$. Following [Horner's method], we can factor the above
to:

```math
s_{n-1} + 31(s_{n-2} + ... + 31(s_1 + 31s_0))
```

for more efficient evaluation. The `for`-loop evaluates this last form from
innermost parentheses to outermost (i.e., right to left).

### `faster_kr_hash`

An optimization of `kr_hash`. Note that:

```math
31 = 32 - 1 = 2^5 - 1
```

Multiplying by $$h$$, we get:

```math
31h = (32 - 1)h = 32h - h = 2^5h - h
```

The left shift (`<<`) and right shift (`>>`) operators are very efficient ways
of multiplying or dividing by powers of two, respectively, since they work by
shifting bits. $$2^5h$$ is equivalent to `h << 5`, allowing us to rewrite
`31 * h` as `(h << 5) - h`:

```c
unsigned long faster_kr_hash(char *s) {
  unsigned long h;
  for (h = 0; *s != '\0'; s++) {
    h += *s + (h << 5) - h; // faster way of multiplying by 31
  }
  return h;
}
```

The [results] show that this optimization is about 20% faster.

### `new_hash`

The same as `kr_hash` except that we substitute 127 for 31, resulting in a
polynomial with powers of 127:

```math
127^{n-1}s_0 + 127^{n-2}s_1 + ... + 127s_{n-2} + s_{n-1}
```

### `faster_new_hash`

An optimization of `new_hash`, based on the fact that:

```math
127h = (128 - 1)h = 128h - h = 2^7h - h
```

so we can rewrite `127 * h` as `(h << 7) - h`.

## Benchmarking

This project includes a simple benchmarking system in [benchmark.h]. To use it,
first call the `benchmark_configure` macro:

```c
benchmark_configure(10000000);
```

Or for clarity:

```c
benchmark_configure(.iterations = 10000000);
```

`number_of_iterations` is the number of times each expression is to be run.
After that, use the `benchmark` macro to run individual benchmarks:

```c
benchmark("K&R Hash            : {} seconds", kr_hash(string));
benchmark("K&R Hash (faster)   : {} seconds", faster_kr_hash(string));
```

where the first argument is the benchmark description, and the second argument
is the expression to be evaluated. So this calls `kr_hash` and `faster_kr_hash`
10,000,000 times each, outputting the description &mdash; where `{}` is a
placeholder for the benchmark's time in seconds.

<!------------------------------------------------------------------------------
  Links
------------------------------------------------------------------------------->
[_The C Programming Language_]: https://archive.org/details/the-ansi-c-programming-language-by-brian-w.-kernighan-dennis-m.-ritchie.org/mode/2up
[benchmark.h]: demo/benchmark.h
[Hash functions: An empirical comparison]: https://www.strchr.com/hash_functions
[hash functions]: https://en.wikipedia.org/wiki/Hash_function
[hash_functions.h]: demo/hash_functions.h
[Horner's method]: https://en.wikipedia.org//wiki/Horner's_method
[p. 128]: https://archive.org/details/the-ansi-c-programming-language-by-brian-w.-kernighan-dennis-m.-ritchie.org/page/128/mode/1up?view=theater
[results]: #building-and-running
[SipHash]: https://github.com/veorq/SipHash
