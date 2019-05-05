/**
 * @file
 * Prototypes of low-level syscalls, required by newlib-nano
 * see https://sourceware.org/newlib/libc.html#Stubs
 */
#ifndef _SYSCALLS_H__
#define _SYSCALLS_H__

#include <sys/types.h>

int _read(int file, char *ptr, int len);
int _write(int file, char *buffer, unsigned int count);
int _lseek(int file, int ptr, int dir);
int _fstat(int file, struct stat *st);
int _close(int file);
int _isatty(int file);
caddr_t _sbrk(int incr);

#endif //_SYSCALLS_H__
