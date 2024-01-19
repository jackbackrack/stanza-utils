#include <sys/stat.h>
#include <stanza/types.h>

// Retrieve the file permissions of the given path. If this succeeds, the
// permissions are casted to a Stanza long and written to `out`, and this
// function returns 0. Otherwise, nothing is written to `out` and this function
// returns -1.
stz_int get_file_permissions(const stz_byte* path, stz_long* out) {
  struct stat st;
  if (stat(C_CSTR(path), &st) == -1) {
    return -1;
  }

  *out = (stz_long)st.st_mode;
  return 0;
}

// Sets the file permissions of the given path. If this succeeds, return 0,
// otherwise return -1.
stz_int set_file_permissions(const stz_byte* path, stz_long permissions) {
  if (chmod(C_CSTR(path), (mode_t)permissions) == -1) {
    return -1;
  }
  return 0;
}
