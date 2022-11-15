#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define PGSIZE 4096

int main(int argc, char *argv[]) {
  printf("# Before sbrk(PGSIZE * 2)\n");
  vmprint();
  printf("\n");

  char *orig = sbrk(PGSIZE * 2);

  printf("# After sbrk(PGSIZE * 2)\n");
  vmprint();
  printf("\n");

  char *new = sbrk(-PGSIZE * 2);

  printf("# After sbrk(-PGSIZE * 2)\n");
  vmprint();
  printf("\n");

  if (orig + PGSIZE * 2 != new) {
    printf("error: return value of sbrk() is incorrect.\n");
    exit(1);
  }

  orig = sbrk(PGSIZE * 2);

  printf("# After sbrk(PGSIZE * 2) again\n");
  vmprint();
  printf("\n");

  *(orig + PGSIZE) = 1;

  printf("# After page fault at %p\n", orig + PGSIZE);
  vmprint();
  printf("\n");

  new = sbrk(-PGSIZE * 2);

  printf("# After sbrk(-PGSIZE * 2) again\n");
  vmprint();

  if (orig + PGSIZE * 2 != new) {
    printf("error: return value of sbrk() is incorrect.\n");
    exit(1);
  }

  exit(0);
}
