#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h"

int
main(int argc, char *argv[])
{
  char buf[512];
  int fd_src, fd_dest, n;

  if (argc != 3) {
    fprintf(2, "Usage: cp source destination\n");
    exit(1);
  }

  if ((fd_src = open(argv[1], O_RDONLY)) < 0) {
    fprintf(2, "cp: cannot open %s\n", argv[1]);
    exit(1);
  }

  if ((fd_dest = open(argv[2], O_CREATE | O_WRONLY)) < 0) {
    fprintf(2, "cp: cannot open/create %s\n", argv[2]);
    close(fd_src);
    exit(1);
  }

  while ((n = read(fd_src, buf, sizeof(buf))) > 0) {
    if (write(fd_dest, buf, n) != n) {
      fprintf(2, "cp: write error\n");
      close(fd_src);
      close(fd_dest);
      exit(1);
    }
  }

  if (n < 0) {
    fprintf(2, "cp: read error\n");
  }

  close(fd_src);
  close(fd_dest);
  exit(0);
}
