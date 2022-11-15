#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/vm.h"
#include "user/user.h"

#define PGSIZE 4096

int main(int argc, char *argv[]) {
  int result = 0;

  char *lower = sbrk(PGSIZE * 2);
  char *upper = lower + PGSIZE;

  result = madvise(lower, 0, MADV_NORMAL);
  if (result != 0) {
    printf("error: return value of madvise() is incorrect.\n");
    exit(1);
  }

  result = madvise(lower, PGSIZE, MADV_NORMAL);
  if (result != 0) {
    printf("error: return value of madvise() is incorrect.\n");
    exit(1);
  }

  result = madvise(lower, PGSIZE * 2, MADV_NORMAL);
  if (result != 0) {
    printf("error: return value of madvise() is incorrect.\n");
    exit(1);
  }

  result = madvise(lower, PGSIZE * 3, MADV_NORMAL);
  if (result == 0) {
    printf("error: return value of madvise() is incorrect.\n");
    exit(1);
  }

  /* Trigger page fault */
  *upper = 1;

  printf("# Before madvise()\n");
  vmprint();
  printf("\n");

  result = madvise(lower, PGSIZE * 2, MADV_DONTNEED);
  if (result != 0) {
    printf("error: return value of madvise() is incorrect.\n");
    exit(-1);
  }

  printf("# After madvise()\n");
  vmprint();

  exit(0);
}
