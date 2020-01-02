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

void test(char str0[], char str1[]) {
  printf("%s vs %s => have %sduplicate\n", str0, str1,
         has_duplicate(str0, str1) ? "" : "no ");
}

int main(int argc, char *argv[]) {
  int const hcreate_success = hcreate(1000);
  if (!hcreate_success) {
    error(1, errno, "hcreate");
  }
  test("", "");
  test("a", "");
  test("a", "a");
  test("a", "b");
  test("abcd", "efghi");
  test("abcd", "defghi");
  hdestroy();
  return 0;
}
