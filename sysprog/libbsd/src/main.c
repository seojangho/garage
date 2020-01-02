#include <bsd/sys/queue.h>
#include <errno.h>
#include <error.h>
#include <stdio.h>
#include <stdlib.h>

struct Person {
  unsigned id;
  LIST_ENTRY(Person) people;
};

struct People {
  LIST_HEAD(people_list, Person) people;
};

void add_person(unsigned id, struct People *people) {
  struct Person *person = malloc(sizeof(struct Person));
  if (person == 0) {
    error(1, errno, "Memory allocation failure");
  }
  person->id = id;
  LIST_INSERT_HEAD(&people->people, person, people);
}

int main(int argc, char **argv) {
  struct People people;
  LIST_INIT(&people.people);
  add_person(1, &people);
  add_person(2, &people);
  add_person(3, &people);
  printf("hello there\n");

  struct Person *p0 = LIST_FIRST(&people.people);
  struct Person *p1 = 0;
  while (p0 != 0) {
    printf("id=%u\n", p0->id);
    p1 = LIST_NEXT(p0, people);
    free(p0);
    p0 = p1;
  }

  return 0;
}
