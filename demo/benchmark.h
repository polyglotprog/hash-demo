#ifndef BENCHMARK_H
#define BENCHMARK_H

/*
 * A couple macros and a function for benchmarking.
 */

#include <assert.h>
#include <stdlib.h>
#include <string.h>

struct BenchmarkConfiguration {
  int iterations;
};

#define benchmark_configure(...) \
  const struct BenchmarkConfiguration _benchmark_configuration_ = { \
    __VA_ARGS__ \
  }; \
  clock_t _start_, _end_; \
  double _elapsed_time_

#define benchmark(description, expression) \
  _start_ = clock(); \
  { \
    int iterations = _benchmark_configuration_.iterations; \
    for (int _i_ = 0; _i_ < iterations; _i_++) { \
      expression; \
    } \
  } \
  _end_ = clock(); \
  _elapsed_time_ = (double)(_end_ - _start_) / CLOCKS_PER_SEC; \
  benchmark_display(description, _elapsed_time_)

void benchmark_display(char *description, double elapsed_time) {
  int description_length = strlen(description);
  char *output = malloc(description_length + 1);
  assert(output);
  int i;
  for (i = 0; i < description_length; i++) {
    if (description[i] == '{' && description[i + 1] == '}') {
      output[i] = '%';
      output[i + 1] = 'f';
      i += 2;
    }
    output[i] = description[i];
  }
  output[i] = '\n';
  output[++i] = '\0';
  printf(output, elapsed_time);
}

#endif
