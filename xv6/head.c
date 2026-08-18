#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void
head(int fd, int nlines)
{
  char buf[512];
  int n, i;
  int lines_printed = 0;

  while ((n = read(fd, buf, sizeof(buf))) > 0 && lines_printed < nlines) {
    for (i = 0; i < n && lines_printed < nlines; i++) {
      if (write(1, &buf[i], 1) != 1) {
        fprintf(2, "head: write error\n");
        exit(1);
      }
      if (buf[i] == '\n') {
        lines_printed++;
      }
    }
  }
  if (n < 0) {
    fprintf(2, "head: read error\n");
    exit(1);
  }
}

int
main(int argc, char *argv[])
{
  int fd, i;
  int nlines = 10;
  char *filename = 0;

  // Check for -n argument
  if (argc > 1 && argv[1][0] == '-') {
    nlines = atoi(argv[1] + 1);
    if (argc > 2) {
      filename = argv[2];
    }
  } else if (argc > 1) {
    filename = argv[1];
  }

  if (filename == 0) {
    head(0, nlines);
    exit(0);
  }

  if ((fd = open(filename, 0)) < 0) {
    fprintf(2, "head: cannot open %s\n", filename);
    exit(1);
  }
  head(fd, nlines);
  close(fd);
  exit(0);
}
