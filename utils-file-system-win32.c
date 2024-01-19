#include <windows.h>
#include <stanza.h>

// Return a directory path suitable for creating temporary files in, by
// allocating a buffer of size `MAX_PATH + 1`, and then calling the Windows API
// func `GetTempPath()` to populate it. If this succeeds, we return the buffer,
// otherwise NULL. Responsibility falls on the caller to `free()` the returned
// buffer, except when an error occurs.
stz_byte* get_temp_path() {
  TCHAR* buffer = malloc(sizeof(TCHAR) * (MAX_PATH + 1));
  if (GetTempPath(MAX_PATH + 1, buffer) == 0) {
    free(buffer);
    return NULL;
  }
  return STZ_STR(buffer);
}

// Retrieve the file permissions of the given path. If this succeeds, the
// permissions are casted to a Stanza long and written to `out`, and this
// function returns 0. Otherwise, nothing is written to `out` and this function
// returns -1.
stz_int get_file_permissions(const stz_byte* path, stz_long* out) {
  DWORD attributes = GetFileAttributes(C_CSTR(path));
  if (attributes == INVALID_FILE_ATTRIBUTES) {
    return -1;
  }
  *out = (stz_long) attributes;
  return 0;
}

// Sets the file permissions of the given path. If this succeeds, return 0,
// otherwise return -1.
stz_int set_file_permissions(const stz_byte* path, stz_long permissions) {
  if (!SetFileAttributes(C_CSTR(path), (DWORD)permissions)) {
    return -1;
  }
  return 0;
}
