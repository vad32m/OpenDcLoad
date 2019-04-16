#include "FreeRTOSConfig.h"

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <time.h>
#include <sys/stat.h>

#include <libopencm3/cm3/itm.h>
#include <libopencm3/cm3/scb.h>

// // Function declaration.
// void _exit(int i);
// int _open(const char *name, int flags, int mode);
 int _read(int file, char *ptr, int len);
 int _write(int file, char *buffer, unsigned int count);
 int _lseek(int file, int ptr, int dir);
 int _fstat(int file, struct stat *st);
// int _link(char *old, char *new);
// int _unlink(char *name);
// int _stat(char *file, struct stat *st);
int _close(int file);
// int _execve(char *name, char **argv, char **env);
// int _fork();
// int _getpid();
 int _isatty(int file);
// int _kill(int pid, int sig);
 caddr_t _sbrk(int incr);
// int times(struct tm *buf);
// int _wait(int *status);

#undef errno
extern int errno;
char *__env[1] = {0};
char **__environ = __env;
static uint8_t heap_mem[configTOTAL_HEAP_SIZE];
uint32_t allocated = 0;

// Function definition.


int _write(int file, char *data, unsigned int len)
{
  for (uint16_t i = 0u; i < len; i++)
  {
      while (!(ITM_STIM8(0) & ITM_STIM_FIFOREADY)) {}

	    ITM_STIM8(0) = data[i];
  }

  return len;
}

int _close(int file)
{
  return -1;
}

int _fstat(int file, struct stat *st)
{
  st->st_mode = S_IFCHR;
  return 0;
}

int _isatty(int file)
{
  return 1;
}

int _lseek(int file, int ptr, int dir)
{
  return 0;
}

int _read(int file, char *ptr, int len)
{
  return 0;
}

caddr_t _sbrk(int incr)
{
  caddr_t prev_break = heap_mem + allocated;
  
  allocated += incr;

  if (allocated >= configTOTAL_HEAP_SIZE)
  {
    errno = ENOMEM;
    allocated -= incr;
    return (void *)(-1);
  }

  return prev_break;
}



