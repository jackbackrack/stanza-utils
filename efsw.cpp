// See license.txt for details about licensing.

#include <efsw/efsw.hpp>
#include <efsw/System.hpp>
#include <efsw/FileSystem.hpp>
#include <signal.h>
#include <iostream>
#include "safe-queue.hpp"
#include "efsw.hpp"

class WatcherEvent {
 public:
  std::string filename;
  WatcherEvent (std::string ifilename) { filename = ifilename; }
};

/// Processes a file action
class UpdateListener : public efsw::FileWatchListener {
  SafeQueue<WatcherEvent> *queue;
 public:
  UpdateListener(SafeQueue<WatcherEvent> *iqueue) {
    queue = iqueue;
  }

  std::string getActionName( efsw::Action action ) {
    switch ( action ) {
    case efsw::Actions::Add:		return "Add";
    case efsw::Actions::Modified:	return "Modified";
    case efsw::Actions::Delete:		return "Delete";
    case efsw::Actions::Moved:		return "Moved";
    default:				return "Bad Action";
    }
  }

  void handleFileAction( efsw::WatchID watchid, const std::string& dir, const std::string& filename, efsw::Action action, std::string oldFilename = ""  ) {
    if (action == efsw::Actions::Modified) {
      WatcherEvent event(filename);
      queue->enqueue(event);
    }
    // std::cout << "DIR (" << dir + ") FILE (" + ( oldFilename.empty() ? "" : "from file " + oldFilename + " to " ) + filename + ") has event " << getActionName( action ) << std::endl;
  }
};

class Watcher {
 public:
  UpdateListener *listener;
  efsw::FileWatcher *watcher;
  std::string pathname;
  SafeQueue<WatcherEvent> queue;
  std::vector< std::string > filenames;
};

Watcher* new_watcher (void) {
  Watcher* res = new Watcher();
  res->listener = new UpdateListener(&res->queue);
  efsw::FileWatcher *fw = new efsw::FileWatcher( false );
  res->watcher = fw;
  fw->followSymlinks( false );
  fw->allowOutOfScopeLinks( false );
  return res;
}

int watcher_add_dirname (Watcher* watcher, char* pathname) {
  std::string path(pathname);
  efsw::WatchID err;

  if ( ( err = watcher->watcher->addWatch( path, watcher->listener, true ) ) > 0 ) {
    watcher->watcher->watch();
    std::cout << "Watching directory: " << path.c_str() << std::endl;
  }
  return 1;
}

int watcher_add_filename (Watcher* watcher, char* filename) {
  watcher->filenames.push_back(filename);
  return watcher->filenames.size() - 1;
}

int watcher_is_empty (Watcher* watcher) {
  return watcher->queue.is_empty();
}

int watcher_next (Watcher* watcher) {
  WatcherEvent event = watcher->queue.dequeue();
  for (int i = 0; i < watcher->filenames.size(); i++) {
    if (event.filename == watcher->filenames[i])
      return i;
  }
  return -1;
}

