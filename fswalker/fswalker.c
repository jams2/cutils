#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include "fswalker.h"

void
fserror(char *msg, int err, int line, int do_exit)
{
  switch (err) {
  case EACCES: print_err(msg, line,  "permission denied"); break;
  case EBADF: print_err(msg, line,  "bad file descriptor"); break;
  case EMFILE: print_err(msg, line,  "reached process fd limit"); break;
  case ENFILE: print_err(msg, line,  "reached system fd limit"); break;
  case ENOENT: print_err(msg, line,  "doesn't exist"); break;
  case ENOMEM: print_err(msg, line,  "out of memory"); break;
  case ENOTDIR: print_err(msg, line,  "not a directory"); break;
  }
  if (do_exit)
    exit(1);
}

void
count_files(char *dir_name, struct fscount *counts, int depth)
{
  char path[NAME_MAX * 16];
  DIR *dir;
  if ((dir = opendir(dir_name)) == NULL) {
    fserror(dir_name, errno, __LINE__, 0);
    return;
  }

  struct dirent *ent;
  errno = 0;

  while ((ent = readdir(dir)) != NULL) {
    if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0
	|| ent->d_name[0] == '.')
      continue;
    if (ent->d_type == DT_REG) {
      counts->files++;
    } else if (ent->d_type == DT_DIR){
      counts->dirs++;
      sprintf(path, "%s", dir_name);
      if (path[strlen(path) - 1] != '/')
	strcat(path, "/");
      strcat(path, ent->d_name);
      count_files(path, counts, depth + 2);
    }
  }

  if (errno != 0)
    fserror(dir_name, errno, __LINE__, 0);
  closedir(dir);
}

int
main(int argc, char *argv[])
{
  struct fscount counts = { 0, 0 };
  char *start = (argc == 2) ? argv[1] : "/";
  count_files(start, &counts, 0);
  printf("counted %d directories and %d regular files\n", counts.dirs, counts.files);
  return 0;
}
