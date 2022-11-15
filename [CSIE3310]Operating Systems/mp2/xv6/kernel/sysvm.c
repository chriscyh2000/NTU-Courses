#include "param.h"
#include "types.h"
#include "memlayout.h"
#include "riscv.h"
#include "spinlock.h"
#include "defs.h"
#include "proc.h"

/* NTU OS 2022 */
/* Entry of vmprint() syscall. */
uint64
sys_vmprint(void)
{
  struct proc *p = myproc();
  vmprint(p->pagetable);
  return 0;
}

/* NTU OS 2022 */
/* Entry of madvise() syscall. */
uint64
sys_madvise(void)
{

  uint64 addr;
  int length;
  int advise;
  
  if (argaddr(0, &addr) < 0) return -1;
  if (argint(1, &length) < 0) return -1;
  if (argint(2, &advise) < 0) return -1;

  int ret = madvise(addr, length, advise);
  return ret;
}
