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

static void public1(void);
static void public2(void);
static void cleanup(void);

int
main(int argc, char *argv[])
{
  cleanup();
  public1();
  cleanup();
  public2();
  cleanup();
  exit(failed);
}

static void
cleanup(void)
{
  unlink("/testsymlink/a");
  unlink("/testsymlink/b");
  unlink("/testsymlink/c");
  unlink("/testsymlink/1");
  unlink("/testsymlink/2");
  unlink("/testsymlink/3");
  unlink("/testsymlink/4");
  unlink("/testsymlink2/1");
  unlink("/testsymlink2/3");
  unlink("/testsymlink/z");
  unlink("/testsymlink/y");
  unlink("/testsymlink");
  unlink("/testsymlink2");
}

// stat a symbolic link using O_NOFOLLOW
static int
stat_slink(char *pn, struct stat *st)
{
  int fd = open(pn, O_RDONLY | O_NOFOLLOW);

  if(fd < 0)
    return -1;
  if(fstat(fd, st) != 0)
    return -1;
  return 0;
}

static void
public1(void)
{
  int r, fd1 = -1, fd2 = -1;
  char buf[4] = {'a', 'b', 'c', 'd'};
  char c = 0;
  struct stat st;
    
  printf("public testcase 1:\n");

  mkdir("/testsymlink");

  // 1. Create file a
  fd1 = open("/testsymlink/a", O_CREATE | O_RDWR);
  if(fd1 < 0) fail("failed to open a");

  // 2. Create symlink b -> a
  r = symlink("/testsymlink/a", "/testsymlink/b");
  if(r < 0)
    fail("symlink b -> a failed");

  // 3. check b file type
  if (stat_slink("/testsymlink/b", &st) != 0)
    fail("failed to stat b");
  if(st.type != T_SYMLINK)
    fail("b isn't a symlink");

  // 4. Write a
  if(write(fd1, buf, sizeof(buf)) != 4)
    fail("failed to write to a");

  // 5. Read b and check if it is identical to a.
  fd2 = open("/testsymlink/b", O_RDWR);
  if(fd2 < 0)
    fail("failed to open b");
  read(fd2, &c, 1);
  if (c != 'a')
    fail("b and a are not the same");

  printf("public testcase 1: ok\n");

done:
  close(fd1);
  close(fd2);
}

static void
public2(void)
{
  int r, fd1 = -1, fd2 = -1;
  char c = 0, c2 = 0;
    
  printf("public testcase 2:\n");

  mkdir("/testsymlink");

  // 1. Create a symlink chain 1 -> 2 -> 3 -> 4
  r = symlink("/testsymlink/2", "/testsymlink/1");
  if(r) fail("Failed to link 1->2");
  r = symlink("/testsymlink/3", "/testsymlink/2");
  if(r) fail("Failed to link 2->3");
  r = symlink("/testsymlink/4", "/testsymlink/3");
  if(r) fail("Failed to link 3->4");

  // 2. Open 1 and 4.
  fd1 = open("/testsymlink/4", O_CREATE | O_RDWR);
  if(fd1<0) fail("Failed to create 4\n");
  fd2 = open("/testsymlink/1", O_RDWR);
  if(fd2<0) fail("Failed to open 1\n");

  // Write 1 and read 4 to check they are the same file.
  c = '#';
  r = write(fd2, &c, 1);
  if(r!=1) fail("Failed to write to 1\n");
  r = read(fd1, &c2, 1);
  if(r!=1) fail("Failed to read from 4\n");
  if(c!=c2)
    fail("Value read from 4 differed from value written to 1\n");

  printf("public testcase 2: ok\n");

done:
  close(fd1);
  close(fd2);
}
