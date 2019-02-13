// See license.txt for details about licensing.

class Watcher;
extern "C" {
Watcher* new_watcher (void);
int watcher_add_dirname (Watcher* watcher, char* pathname);
int watcher_add_filename (Watcher* watcher, char* filename);
int watcher_is_empty (Watcher* watcher);
int watcher_next (Watcher* watcher);
};

