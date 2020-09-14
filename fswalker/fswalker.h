#ifndef fswalker_
#define fswalker_

#include <dirent.h>

#define print_err(msg, line, why)					\
  fprintf(stderr, "[%s:%d] \"%s\": %s\n", __FILE__, (line), (msg), (why))

#define print_indent(n) for (int i = 0; i < (n); printf(" "), ++i)

struct fscount {
  int dirs;
  int files;
};

void fserror(char *msg, int err, int line, int do_exit);
void count_files(char *dir_name, struct fscount *counts, int depth);

#endif
