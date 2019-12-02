#ifndef __FULL_HEAP__
#define __FULL_HEAP__

#include <cstddef>
#include <iostream>

class Node {
 public :
  int idx;
  bool is_full;
  bool is_leaf;
  float value;
  Node* prev;
  Node* next;
 Node(int idx, bool is_leaf) : idx(idx), is_leaf(is_leaf), value(-1.0), is_full(false), prev(nullptr), next(nullptr) { }
  bool is_null () { return !is_full; }
  void print () {
    printf("%s", is_leaf ? "Leaf" : "Node");
    printf("(%d", idx);
    if (is_full) {
      if (is_leaf) {
        printf(" %f", value);
      } else {
        printf(" T");
      }
    }
    printf(")");
  }

};

int next_power_of_two (int n) {
  int k = 0;
 loop:
  int p2 = 1 << k;
  if (p2 >= n) {
    return k;
  } else {
    k = k + 1;
    goto loop;
  }
}
  
int parent_idx (int i) { return i >> 1; }
int left_child_index (int i) { return i << 1; }
int right_child_index (int i) { return (i << 1) + 1; }
int left_sibling_index (int i) { return (i >> 1) << 1; }
int right_sibling_index (int i) { return i | 1; }
bool is_left_child_index (int i) { return (i & 1) == 0; }
bool is_right_child_index (int i) { return (i & 1); }
int tot_nodes (int l) { return 2 * l - 1; }
int num_internal_nodes (int l) { return tot_nodes(l) - l; }

class Heap {
 public:
  int num_leaves;
  int pow2;
  Node *first;
  Node *last;
  Node **nodes;
  int leaf_index (int k) { return num_internal_nodes(num_leaves) + k; }
  Node* node_of (int i) { return nodes[i - 1]; }
  Node* leaf (int k) { return node_of(leaf_index(k)); }
  void set_node_of (int i, Node* n) { nodes[i - 1] = n; }
  bool has_no_children (int i) {
    return node_of(left_child_index(i))->is_null() && node_of(right_child_index(i))->is_null();
  }

  Heap (int n) {
    pow2 = next_power_of_two(n);
    num_leaves = 1 << pow2;
    int tot = tot_nodes(num_leaves);
    int internals = num_internal_nodes(num_leaves);
    nodes = (Node**)malloc(tot * sizeof(Node*));
    for (int i = 0; i < internals; i++) {
      nodes[i] = new Node(i, false);
    }
    for (int i = 1, k = internals; k < tot; k++, i++) {
      nodes[k] = new Node(i, true);
    }
    first = new Node(0, true);
    last = new Node(num_leaves + 1, true);
    first->next = last;
    last->prev = first;
  }

  Node* right_most_node (int s) {
    Node* n = node_of(s);
    if (n->is_leaf && n->is_full) {
      return n;
    } else {
      int i = s;
    loop:
      Node* rc_node = node_of(right_child_index(i));
      if (rc_node->is_leaf) {
        if (rc_node->is_full) {
          return rc_node;
        } else {
          Node* lc_node = node_of(left_child_index(i));
          if (lc_node->is_full) {
            return lc_node;
          } else {
            return last;
          }
        }
      } else {
        i = rc_node->is_full ? right_child_index(i) : left_child_index(i);
        goto loop;
      }
    }
  }

  Node* pred (int k) {
    int idx = leaf_index(k);
    int i = idx;
  loop:
    if (i <= 1) {
      return first;
    } else {
      if (is_left_child_index(i) || node_of(left_sibling_index(i))->is_null()) {
        i = parent_idx(i);
        goto loop;
      } else {
        return right_most_node(left_sibling_index(i));
      }
    }
  }

  Node* insert (int k, float v) {
    int idx = leaf_index(k);
    Node* node = node_of(idx);
    if (!node->is_full) {
      int i = parent_idx(idx);
    loop:
      Node* n = node_of(i);
      if (!n->is_full) {
        n->is_full = true;
        if (i > 1) {
          i = parent_idx(i);
          goto loop;
        }
      }
      Node* prev = pred(k);
      Node* next = prev->next;
      node->is_full = true;
      node->prev = prev;
      prev->next = node;
      node->next = next;
      next->prev = node;
    }
    node->value = v;
    return node;
  }

  Node* remove (int k) {
    int idx = leaf_index(k);
    Node* node = node_of(idx);
    if (node->is_full) {
      node->is_full = false;
      node->prev->next = node->next;
      node->next->prev = node->prev;
      node->next = nullptr;
      node->prev = nullptr;
      node->value = -1.0;
      int i = parent_idx(idx);
    loop:
      if (i >= 1 && has_no_children(i)) {
        Node* parent = node_of(i);
        parent->is_full = false;
        i = parent_idx(i);
        goto loop;
      }
    }
    return node;
  }
  void dump () {
    int j = 1;
    for (int i = 0; i <= pow2; i++) {
      for (int k = (1 << i) - 1; k < ((1 << (i + 1)) - 1); k++) {
        Node* n = nodes[k];
        printf("%d:", j);
        n->print();
        printf(" ");
        j = j + 1;
      }
      printf("\n");
    }
    for (Node* b = first; b != nullptr; b = b->next) {
      b->print();
      printf(" <=> ");
    }
    printf("\n");
  }

};

#endif
