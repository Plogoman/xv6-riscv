#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

extern int set_priority(int pid, int priority);
extern void set_sched_mode(int mode);

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  if(n < 0)
    n = 0;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.

uint64 sys_getppid(void) {
  struct proc *p = myproc();

  acquire(&p->lock);
  if (p->parent) {
    release(&p->lock);
    return p->parent->pid;
  } else {
    release(&p->lock);
    return 0;
  }
}

uint64 sys_getptable(void) {
  int nproc;
  uint64 addr;

  argint(0, &nproc);
  argaddr(1, &addr);

  if(addr == 0)
    return -1;

  return getptable(nproc, (char*)addr);
}

uint64
sys_set_priority(void)
{
  int pid, priority;

  argint(0, &pid);
  argint(1, &priority);

  return set_priority(pid, priority);
}

uint64
sys_set_sched_policy(void)
{
  int policy;

  argint(0, &policy);

  if (policy < 0 || policy > 2)
      return -1;

  set_sched_mode(policy);
  return 0;
}
