/*
 * The hash function from K&R:
 *
 *     https://archive.org/details/the-ansi-c-programming-language-by-brian-w.-kernighan-dennis-m.-ritchie.org/page/128/mode/1up?view=theater
 *
 * It uses Horner's method to efficiently evaluate the following polynomial:
 *
 *    31^(n-1) * s[0] + 31^(n-2) * s[1] + ... + 31 * s[n-2] + s[n-1]
 *
 * where n is the length of the string s, and s[i] is the character at position
 * i in the string s.
 */
unsigned long kr_hash(char *s) {
  unsigned long h;
  for (h = 0; *s != '\0'; s++) {
    h += *s + 31 * h;
  }
  return h;
}

/*
 * Optimization of K&R's hash function.
 */
unsigned long faster_kr_hash(char *s) {
  unsigned long h;
  for (h = 0; *s != '\0'; s++) {
    h += *s + (h << 5) - h; // faster way of multiplying by 31
  }
  return h;
}

/*
 * New hash function using 127 instead of 31.
 */
unsigned long new_hash(char *s) {
  unsigned long h;
  for (h = 0; *s != '\0'; s++) {
    h += *s + 127 * h;
  }
  return h;
}

/*
 * Optimization of our new hash function.
 */
unsigned long faster_new_hash(char *s) {
  unsigned long h;
  for (h = 0; *s != '\0'; s++) {
    h += *s + (h << 7) - h; // faster way of multiplying by 127
  }
  return h;
}
