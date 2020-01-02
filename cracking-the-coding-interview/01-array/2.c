#include <error.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
size_t const vector_size = 178;

void populate_vector(char str[], unsigned vector[static vector_size]) {
  for (size_t i = 0; str[i]; i++) {
    vector[(size_t)str[i]]++;
  }
}

bool check(char str0[], char str1[]) {
  if (strlen(str0) != strlen(str1)) {
    return false;
  }
  unsigned str0_vector[vector_size];
  memset(str0_vector, 0, vector_size * sizeof(unsigned));
  unsigned str1_vector[vector_size];
  memset(str1_vector, 0, vector_size * sizeof(unsigned));
  populate_vector(str0, str0_vector);
  populate_vector(str1, str1_vector);
  for (size_t i = 0; i < vector_size; i++) {
    if (str0_vector[i] != str1_vector[i]) {
      return false;
    }
  }
  return true;
}

void test(char str0[], char str1[], bool expectation) {
  if (check(str0, str1) != expectation) {
    error(1, 0, "wrong");
  }
  printf("%s vs %s => %spermutation\n", str0, str1, expectation ? "" : "NOT ");
}

int main(int argc, char *argv[]) {
  test("aaaa", "aaaa", true);
  test("aaa", "aaaa", false);
  test("aba", "aab", true);
  test("aba", "abb", false);
  return 0;
}
