// See license.txt for details about licensing.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

struct DirEntry {
  int file_number;
  int record_length;
  char type;
  char name_length;
  char* name;
};

int readdirent (DIR* dir, struct DirEntry* res) {
  struct dirent* ent = readdir(dir);
  if (ent == NULL) {
    return 0;
  } else {
    res->file_number = ent->d_ino;
    res->record_length = ent->d_reclen;
    res->type = ent->d_type;
    res->name_length = strlen(ent->d_name);
    res->name = ent->d_name;
    return 1;
  }
}
