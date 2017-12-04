#include <stdio.h>
#include <time.h>

// "%Y-%m-%d-%H-%M-%S"

char result[256];

char* get_time_string (char* format) {
  time_t rawtime;
  struct tm * timeinfo;
  time ( &rawtime );
  timeinfo = localtime ( &rawtime );
  strftime(result,sizeof(result),format, timeinfo);
  return result;
}
