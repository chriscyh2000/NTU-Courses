#include "kernel/param.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/riscv.h"
#include "kernel/fcntl.h"
#include "kernel/spinlock.h"
#include "kernel/sleeplock.h"
#include "kernel/fs.h"
#include "kernel/file.h"
#include "user/user.h"

#define fail(msg) do {printf("FAILURE: " msg "\n"); failed = 1; goto done;} while (0);
static int failed = 0;

static void public12(void);
static void cleanup(void);

int
main(int argc, char *argv[])
{
  cleanup();
  public12();
  exit(failed);
}

static void
cleanup(void)
{
  unlink("/testsymlink2/p");
  unlink("/testsymlink3/q");
  unlink("/testsymlink2");
  unlink("/testsymlink3");
}

static void
public12()
{
  int r, fd1 = -1, fd2 = -1;
  char c = 0, c2 = 0;
    
  printf("Start: test symlinks to directory\n");

  mkdir("/testsymlink2");
  mkdir("/testsymlink3");

  fd1 = open("/testsymlink2/p", O_CREATE | O_RDWR);
  if(fd1 < 0) fail("failed to open p");

  r = symlink("/testsymlink2", "/testsymlink3/q");
  if(r < 0)
    fail("symlink q -> p failed");

  fd2 = open("/testsymlink3/q/p", O_RDWR);
  if(fd2<0) fail("Failed to open /testsymlink3/q/p\n");

  printf("public testcase 1: ok\n");

  c = '#';
  r = write(fd1, &c, 1);
  if(r!=1) fail("Failed to write to /testsymlink2/p\n");
  r = read(fd2, &c2, 1);
  if(r!=1) fail("Failed to read from /testsymlink3/q/p\n");
  if(c!=c2)
    fail("Value read from /testsymlink2/p differs from value written to /testsymlink3/q/p\n");

  printf("public testcase 2: ok\n");
done:
  close(fd1);
  close(fd2);
}