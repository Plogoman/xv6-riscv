#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "spinlock.h"
#include "proc.h"
#include "defs.h"

static uint64 rand_seed = 0;
extern int kbd_intr_count;

struct datetime {
  int year;
  int month;
  int day;
  int hour;
  int minute;
  int second;
};

static inline uint64 read_mtime(void) {
  return (uint64)CLINT_MTIME;
}

void rand_init(void) {
  rand_seed = read_mtime();
}

uint64 sys_kbdint(void) {
  return kbd_intr_count;
}

uint64 sys_countsyscall(void) {
    int n;
    argint(0, &n);
    struct proc *p = myproc();
    if(n < 0 || n >= 32)
        return -1;
    return p->syscall_count[n];
}

uint64 sys_datetime(void) {
  uint64 addr;
  struct datetime dt = {0};

  argaddr(0, &addr);
  if (addr == 0) return -1;

  // Read mtime register (10MHz clock on QEMU virt)
  uint64 current_ticks = *(volatile uint64*)CLINT_MTIME;
  uint64 boot_epoch = BOOT_EPOCH;
  uint64 seconds_since_boot = current_ticks / 10000000;  // 10MHz = 10M ticks/sec
  uint64 unix_time = boot_epoch + seconds_since_boot;

  uint64 days_since_epoch = unix_time / 86400;
  uint64 seconds_today = unix_time % 86400;

  dt.hour = seconds_today / 3600;
  dt.minute = (seconds_today % 3600) / 60;
  dt.second = seconds_today % 60;

  int year = 1970;
  uint64 days_in_year;

  while (1) {
    days_in_year = ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) ? 366 : 365;
    if (days_since_epoch < days_in_year) break;
    days_since_epoch -= days_in_year;
    year++;
  }

  dt.year = year;

  int days_in_month[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
    days_in_month[1] = 29;

  int month = 0;
  while (month < 12 && days_since_epoch >= days_in_month[month]) {
    days_since_epoch -= days_in_month[month];
    month++;
  }

  dt.month = month + 1;
  dt.day = days_since_epoch + 1;

  if(copyout(myproc()->pagetable, addr, (char*)&dt, sizeof(dt)) < 0) {
    return -1;
  }
  return 0;
}

uint64 sys_myrand(void) {
  rand_seed = (1664525ULL * rand_seed + 1013904223ULL) & 0xFFFFFFFFULL;

  rand_seed ^= (read_mtime() & 0xFFFF);
  return (int)(rand_seed & 0x7FFFFFFF);
}

uint64 sys_uptime(void) {
  uint64 xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);

  return xticks;
}

uint64 sys_shutdown(void) {
  // Write magic value to QEMU test device
  volatile uint32 *test_dev = (uint32*)QEMU_TEST_DEV;
  *test_dev = 0x5555;  // Magic shutdown value for QEMU

  // If QEMU doesn't exit, halt the CPU
  while(1) {
    asm volatile ("wfi");  // Wait for interrupt
  }

  return 0;  // Never reached
}
