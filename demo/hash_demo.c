#include <stdio.h>
#include <time.h>
#include "benchmark.h"
#include "hash_functions.h"

int main() {
  /* A string to test our hashing functions against. */
  char *string = "The quick brown fox jumps over the lazy dog.";

  /*
   * Calculate the hash using our different functions.
   */
  printf("\n%s\n", string);
  printf("================================================================================\n");
  printf("K&R Hash            : %lu\n", kr_hash(string));
  printf("K&R Hash (faster)   : %lu\n", faster_kr_hash(string));
  printf("New Hash            : %lu\n", new_hash(string));
  printf("New Hash (faster)   : %lu\n", faster_new_hash(string));

  /*
   * Now let's benchmark which function is fastest.
   */
  printf("\nBenchmark: 10 Million Iterations\n");
  printf("================================================================================\n");

  benchmark_configure(.iterations = 10000000);

  /* Let the hashing begin! */
  benchmark("K&R Hash            : {} seconds", kr_hash(string));
  benchmark("K&R Hash (faster)   : {} seconds", faster_kr_hash(string));
  benchmark("New Hash            : {} seconds", new_hash(string));
  benchmark("New Hash (faster)   : {} seconds", faster_new_hash(string));

  return 0;
}
