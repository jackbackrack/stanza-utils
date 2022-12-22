#ifdef PLATFORM_WINDOWS
#include "utils-file-system-win32.c"
#endif

#if defined(PLATFORM_OS_X) || defined(PLATFORM_LINUX)
#include "utils-file-system-posix.c"
#endif
