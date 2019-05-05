#include <sys/types.h>

int _read(int file, char *ptr, int len);
int _write(int file, char *buffer, unsigned int count);
int _lseek(int file, int ptr, int dir);
int _fstat(int file, struct stat *st);
int _close(int file);
int _isatty(int file);
caddr_t _sbrk(int incr);

// void _exit(int i);
// int _open(const char *name, int flags, int mode);
// int _link(char *old, char *new);
// int _unlink(char *name);
// int _stat(char *file, struct stat *st);
// int _execve(char *name, char **argv, char **env);
// int _fork();
// int _getpid();
// int _kill(int pid, int sig);
// int times(struct tm *buf);
// int _wait(int *status);
