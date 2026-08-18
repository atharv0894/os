#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void
tail(int fd, char *name, int nlines)
{
  int n, i;
  int total_lines = 0;
  char buf[512];

  // Pass 1: Count total lines
  while ((n = read(fd, buf, sizeof(buf))) > 0) {
    for (i = 0; i < n; i++) {
      if (buf[i] == '\n')
        total_lines++;
    }
  }
  close(fd);

  if (total_lines <= nlines) {
    // Re-open and print everything
    fd = open(name, 0);
    while ((n = read(fd, buf, sizeof(buf))) > 0) {
      write(1, buf, n);
    }
    close(fd);
    return;
  }

  // Pass 2: Skip lines and print the last nlines
  fd = open(name, 0);
  int lines_to_skip = total_lines - nlines;
  int lines_skipped = 0;

  while ((n = read(fd, buf, sizeof(buf))) > 0) {
    for (i = 0; i < n; i++) {
      if (lines_skipped >= lines_to_skip) {
        write(1, &buf[i], 1);
      } else {
        if (buf[i] == '\n')
          lines_skipped++;
      }
    }
  }
  close(fd);
}

int
main(int argc, char *argv[])
{
  int fd;
  int nlines = 10;
  char *filename = 0;

  if (argc > 1 && argv[1][0] == '-') {
    nlines = atoi(argv[1] + 1);
    if (argc > 2) filename = argv[2];
  } else if (argc > 1) {
    filename = argv[1];
  }

  if (filename == 0) {
    fprintf(2, "tail: filename required (stdin not supported in this implementation)\n");
    exit(1);
  }

  if ((fd = open(filename, 0)) < 0) {
    fprintf(2, "tail: cannot open %s\n", filename);
    exit(1);
  }

  tail(fd, filename, nlines);
  exit(0);
}
