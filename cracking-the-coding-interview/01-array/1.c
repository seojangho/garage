#include <bsd/sys/queue.h>
#include <errno.h>
#include <error.h>
#include <search.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *to_array_of_strings_of_one_byte(char str[]) {
  size_t const size = strlen(str);
  char *array_of_strings = calloc(size * 2, 1);
  for (size_t i = 0; i < size; i++) {
    array_of_strings[i * 2] = str[i];
    array_of_strings[i * 2 + 1] = 0;
  }
  return array_of_strings;
}

bool has_duplicate(char str0[], char str1[]) {
  char *str0_d = to_array_of_strings_of_one_byte(str0);
  char *str1_d = to_array_of_strings_of_one_byte(str1);

  for (size_t i = 0; str0[i]; i++) {
    ENTRY *res = hsearch(
        (ENTRY){
            .key = &str0_d[i * 2],
            .data = 0,
        },
        ENTER);
    if (res == NULL) {
      error(1, errno, "Hashtable full");
    }
  }

  bool found = false;
  for (size_t i = 0; str1[i]; i++) {
    ENTRY *res = hsearch(
        (ENTRY){
            .key = &str1_d[i * 2],
            .data = 0,
        },
        FIND);
    if (res != NULL) {
      found = true;
      break;
    }
  }

  free(str0_d);
  free(str1_d);

  return found;
}

bool has_duplicate_nsquare(char str0[], char str1[]) {
  for (size_t i = 0; str0[i]; i++) {
    for (size_t j = 0; str1[j]; j++) {
      if (str0[i] == str1[j]) {
        return true;
      }
    }
  }
  return false;
}

void test(char str0[], char str1[], bool expectation) {
  if (has_duplicate(str0, str1) != expectation) {
    error(1, 0, "has_duplicate wrong");
  }
  if (has_duplicate_nsquare(str0, str1) != expectation) {
    error(1, 0, "has_duplicate_nsquare wrong");
  }
  printf("%s vs %s => have %sduplicate\n", str0, str1,
         expectation ? "" : "no ");
}

int main(int argc, char *argv[]) {
  int const hcreate_success = hcreate(1000);
  if (!hcreate_success) {
    error(1, errno, "hcreate");
  }
  test("", "", false);
  test("a", "", false);
  test("a", "a", true);
  test("a", "b", false);
  test("abcd", "efghi", false);
  test("abcd", "defghi", true);
  hdestroy();
  return 0;
}
