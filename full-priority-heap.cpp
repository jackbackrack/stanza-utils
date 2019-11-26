#include "full-priority-heap.h"


int main (int argc, char** argv) {
  int n = 1 << 13;
  Heap* t = new Heap(n);
  for (int i = 0; i < (n * n); i++) {
    int k = (rand() % n) + 1;
    if (t->leaf(k)->is_null()) {
      t->insert(k, (float)k);
    } else {
      t->remove(k);
    }
  }
  return 0;
}
