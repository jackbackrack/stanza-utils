// See license.txt for details about licensing.

#include <efsw/efsw.hpp>
#include <efsw/System.hpp>
#include <efsw/FileSystem.hpp>
#include <signal.h>
#include <iostream>
#include "safe-queue.hpp"
#include "efsw.hpp"
#include <stdio.h>

int main () {
  Watcher *watcher = new_watcher();
  std::string dir("/home/jrb/jitpcb-dualview/src");
  watcher_add_dirname(watcher, (char*)(dir.c_str()));
  std::string filename("efsw-main.cpp");
  watcher_add_filename(watcher, (char*)(filename.c_str()));
  for (;;) {
    int fid = watcher_next(watcher);
    printf("FID %d\n", fid);
  }
}
