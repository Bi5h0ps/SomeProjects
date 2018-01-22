 #include <stdbool.h>
# bst.h
// NOTES: All of the following functions REQUIRE:
//        pointers to a bst (e.g., t) are valid (not NULL)
//
//       For times, n is the size of the tree
//       For times, h is the height of the tree

extern const int PRE_ORDER;
extern const int IN_ORDER;
extern const int POST_ORDER;

struct bstnode {
  int item;
  struct bstnode *left;
  struct bstnode *right;
  int size;                // NEW !
};

struct bst {
    struct bstnode *root;
};

// bst_create() returns a pointer to a new (empty) bst
// effects: allocates memory (caller must call bst_destroy)
// time: O(1)
struct bst *bst_create(void);

// ino(node,a,pos) stores the item from the node in the array at index *pos
// requires: a is valid
//           pos is valid
// effect: modifies array a and modifies the value of pos
// time: O(1)
void ino(struct bstnode *node, int* a, int* pos);

// bst_destroy(pq) frees all dynamically allocated memory 
// effects: the memory at t is invalid (freed)
// time: O(n)
void bst_destroy(struct bst *t);

// bst_size(t) returns the number of nodes in the bst
// time: O(1)
int bst_size(struct bst *t);

// bst_insert(i, t) inserts the item i into the bst t
// effects: modifies t if i is not already in t
// time: O(h)
void bst_insert(int i, struct bst *t);

// bst_find(i, t) determines if i is in t
// time: O(h) where h is the height of the tree
bool bst_find(int i, struct bst *t);

// bst_select(k, t) returns the k'th element from t in sorted order
// requires: 0 <= k < bst_size(t)
// time: O(h)
int bst_select(int k, struct bst *t);

// bst_remove(i, t) removes i from bst t if it exists
// effects: modifies t if i is in t
// time: O(h)
void bst_remove (int i, struct bst *t);

// bst_range(start, end, t) returns the number of items in t that are 
//   between the values of start and end (inclusive)
// time: O(n)
int bst_range (int start, int end, struct bst *t);

// bst_print(o, t) prints the bst to the screen in order o
// example: given a bst with the following structure
//             4
//           /   \
//          2     5
//         / \
//        1   3
//   pre-order print: "[4,2,1,3,5]\n"
//   in-order print: "[1,2,3,4,5]\n"
//   post-order print: "[1,3,2,5,4]\n"
//   if the t is empty, prints "[empty]\n"
// requires: o is a valid order
// effects: displays output
// time: O(n)
void bst_print (int o, struct bst *t);

// bst_to_sorted_array(t) returns a pointer to a new array
//   which contains all of the items from t in sorted order
//   returns NULL if t is empty
// effects : allocates memory (caller must free)
// time: O(n)
int *bst_to_sorted_array(struct bst *t);

// sorted_array_to_bst(a, len) creates a new BALANCED bst that 
//   contains all of the items from a 
// note: the definition of a balanced binary tree is that, for every node 
//   in the tree, the heights of the left and right sub trees differ by 
//   at most 1
// requires: a is sorted in ascending order, len >= 1,
//           a contains no duplicates
// time: O(nlogn), or O(n) for bonus marks
struct bst *sorted_array_to_bst(int *a, int len);

// bst_rebalance(t) changes t so that it contains all of the same items,
//   but the tree is balanced
// effects: modifies t
// time: O(nlogn)
void bst_rebalance(struct bst *t);



#include "bst.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

const int PRE_ORDER = 0;
const int IN_ORDER = 1;
const int POST_ORDER = 2;

struct bst *bst_create(void) {
  struct bst *new = malloc( sizeof(struct bst) );
  new->root = NULL;
  return new;
}


// bstnode_destroy frees the node and all it's children.
// effect: the memory at node is invalid (freed)
// runtime : O(n)
static void bstnode_destroy (struct bstnode *node) {
  if ( node ) {
    bstnode_destroy (node->left);
    bstnode_destroy (node->right);
    free(node);
  }
}

void bst_destroy(struct bst *t) {
  bstnode_destroy(t->root);
  free(t);
}

int bst_size(struct bst *t) {
  if(t->root) {
    return t->root->size;
  } else {
    return 0;
  }
}


 
bool bst_find(int i, struct bst *t) {
  struct bstnode *control = t->root;

  while (control) {
    if (control -> item == i) {
      return true;
    } else {
      if (control -> item < i) {
        control = control->right;
      } else {
        control = control->left;
      }
    }
  }
  return false;
}

void bst_insert(int i, struct bst *t) {
  if (bst_find (i,t)) return;
  struct bstnode **controll = &(t->root);
  while (*controll) {
    if (i < (*controll)->item) {
      (*controll)->size++;
      controll = &((*controll)->left);
    } else {
      (*controll)->size++;
      controll = &((*controll)->right);
    }
  }
  *controll = malloc(sizeof(struct bstnode));
  (*controll)->item = i;
  (*controll)->left = NULL;
  (*controll)->right = NULL;
  (*controll)->size = 1;
}

// select(k,node) produces the k-th smallest element from node and it's children
// requires: 0 <= k < bst_size(t)
// time:O(h)
static int select (int k, struct bstnode *node) {
  assert(node && 0 <= k && k < node->size);
  int lsize = 0;
  if (node->left) lsize = node->left->size;
  if (k < lsize) return select(k, node->left);
  if (k == lsize) return node->item;
  return select(k - lsize - 1, node->right);
}

int bst_select(int k, struct bst *t) {
  return select(k,t->root);
}

// node_remove(i,node) removes the item i from the nodes if it exists
// effects: modifies node if i is in t
// time: O(h)
struct bstnode *node_remove (int i, struct bstnode *node) {
  if(node == NULL) return NULL;
  if (i < node->item) {
    node->size--;
    node->left = node_remove(i,node->left);
  } else {
    if (i > node->item) {
      node->size--;
      node->right = node_remove(i,node->right);
    } else {
      if (node -> left == NULL) {
        struct bstnode *new = node->right;
        free(node);
        return new;
      } else {
        if (node -> right == NULL) {
          struct bstnode *new = node->left;
          free(node);
          return new;
        } else {
          struct bstnode *next = node->right;
          while (next->left) {
            next = next->left;
          }
          node->item = next->item;
          node->size--;
          node->right = node_remove(next->item, node->right);
        }
      }
    }
  }
  return node;
}

void bst_remove (int i, struct bst *t) {
  if ( bst_find(i,t) ) t->root = node_remove(i,t->root);
}

// compare(start,end,node) produces the number of items among node and 
// it's children that is bigger than start and less than end, inclusive.
// time: O(n)
int compare(int start, int end, struct bstnode *node) {
  if (node) {
    int left = compare(start,end,node->left);
    int right = compare(start,end,node->right);
    if ( node->item >= start && node->item <= end ) {
      return left + right + 1;
    } else {
      return left + right;
    }
  }
  return 0;
}

int bst_range (int start, int end, struct bst *t) {
  return compare(start,end,t->root);
}


// binary_build (a,start,end) produces a balanced bst that contains items 
// bewteen index start and end from the array a.
// requires: a is sorted in ascending order, len >= 1,
//           a contains no duplicates
// time: O(n)
struct bstnode *binary_build( int*a, int start, int end ) {
  assert(a);
  if (start > end) return NULL;
  int mid = (start + end) / 2;
  struct bstnode *new = malloc(sizeof(struct bstnode));
  new->item = a[mid];
  new->size = end - start + 1;
  new->left = binary_build(a,start,mid-1);
  new->right = binary_build(a,mid+1,end);
  return new;
}

int *bst_to_sorted_array(struct bst *t) {
  if (t->root) {
    int pos = 0;
    int *result = malloc(sizeof(int) * bst_size(t));
    ino(t->root,result,&pos);
    return result;
  } else {
    return NULL;
  }
}

struct bst *sorted_array_to_bst(int *a, int len) {
  struct bst *result = bst_create();
  result->root = binary_build(a, 0, len-1);
  return result;
}

void bst_rebalance(struct bst *t) {
  if (t->root) {
    int *sa = bst_to_sorted_array(t);
    struct bstnode *result = binary_build(sa,0,t->root->size-1);
    bstnode_destroy(t->root);
    t->root = result;
    free(sa);
  }
}


// preo(node,a,pos) stores the item from the node in the array at index *pos
// requires: a is valid
//           pos is valid
// effect: modifies array a and modifies the value of pos
// time: O(1)
void preo(struct bstnode *node, int* a, int* pos) {
  if (node) {
    a[*pos] = node->item;
    (*pos)++;
    preo(node->left,a,pos);
    preo(node->right,a,pos);
  }
}

// ino(node,a,pos) stores the item from the node in the array at index *pos
// requires: a is valid
//           pos is valid
// effect: modifies array a and modifies the value of pos
// time: O(1)
void ino(struct bstnode *node, int* a, int* pos) {
  if (node) {
    ino(node->left,a,pos);
    a[*pos] = node->item;
    (*pos)++;
    ino(node->right,a,pos);
  }
}

// posto(node,a,pos) stores the item from the node in the array at index *pos
// requires: a is valid
//           pos is valid
// effect: modifies array a and modifies the value of pos
// time: O(1)
void posto(struct bstnode *node, int* a, int* pos) {
  if (node) {
    posto(node->left,a,pos);
    posto(node->right,a,pos);
    a[*pos] = node->item;
    (*pos)++;
  }
}


void bst_print (int o, struct bst *t) {
  if (t->root) {
    int pos = 0;
    int *result = malloc(sizeof(int) * t->root->size);
    if ( o == PRE_ORDER ) { 
      preo(t->root,result,&pos);
    } else {
      if ( o == IN_ORDER ) {
        ino(t->root,result,&pos);
      } else {
        if ( o == POST_ORDER ) {
          posto(t->root,result,&pos);
        }
      }
    }
    printf("[");
    printf("%d",result[0]);
    for(int i = 1; i < t->root->size; i++) {
      printf(",%d",result[i]);
    }
    printf("]\n");
    free(result);
  } else {
    printf("[empty]\n");
  }
}























