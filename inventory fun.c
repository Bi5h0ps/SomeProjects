#inventory.h
// A module for an inventory ADT with string items and int qtys

struct inventory;

// NOTE: All of the following functions REQUIRE:
//       pointers to an inventory (e.g., inv) are valid (not NULL)
//       item strings are valid
//       for time, n == total number of items in the inventory
//                 m == length of the item (string)

// inventory_create() returns a new empty inventory
// effects: allocates memory (caller must call inventory_destroy)
// time: O(1)
struct inventory *inventory_create(void);

// inventory_destroy(inv) frees all dynamically allocated memory 
// effects: the memory at inv is invalid (freed)
// time : O(n)
void inventory_destroy(struct inventory *inv);

// inventory_lookup(inv, item) determines the quantity of items in inv
//   returns -1 if item is not in the inventory
//   note: an item with quantity 0 returns 0 (not -1)
// time: O(mlogn)
int inventory_lookup(const struct inventory *inv, const char *item);

// inventory_add(inv, item, qty) adds qty items to inv
// requires: qty >= 0
// effects: inv is modified
//          makes a copy of the item string for use in the inventory
// time: O(mlogn) if item is already in the inventory
//       O(mn) if item does not exist in the inventory
void inventory_add(struct inventory *inv, const char *item, int qty);

// inventory_remove(inv, item, qty) removes qty items from inv
// requires: 0 < qty <= inventory_lookup(inv, item)
// effects: inv is modified
// time: O(mlogn)
void inventory_remove(struct inventory *inv, const char *item, int qty);




#include "inventory.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

struct inventory {
  int len;
  struct item **storage;
};

struct item {
  char *name;
  int qtty;
};

struct inventory *inventory_create(void) {
  struct inventory *new = malloc( sizeof(struct inventory) );
  new -> len = 0;
  new -> storage = malloc ( sizeof(struct item) );
  return new;
}

void inventory_destroy(struct inventory *inv) {

  for (int i = 0; i < inv->len; i++) {
    free(inv -> storage[i] -> name);
  }

  for (int i = 0; i < inv->len; i++) {
    free(inv -> storage[i]);
  }

  free( inv-> storage );
  free( inv );
}

// dup(str) produces the duplicate of the string str.
// require: str is valid (str != NULL).
// effect: allocates memory, caller must free.
// run time: O(n), n is length of str.
char *dup (const char* str) {
  int len = strlen(str);
  char *result = malloc( (len+1) * sizeof(char) );
  strcpy(result,str);
  return result;
}

// find(inv,item,left,right) returns the position of target item in inv->data
// if item exists in it, otherwise, -1 is returned. left and right indicates 
// the begining and ending position of the search.
//       for time, n == total number of items in the inventory
//                 m == length of the item (string)
// run time: O(mlogn);
int find (const struct inventory *inv,
          const char *item,int left,int right) {
  while (left <= right) {
    int mid = (right + left) / 2;
    if ( strcmp( item, inv->storage[mid]->name) < 0 ) {
      right = mid - 1;
    } else {
      if ( strcmp( item, inv->storage[mid]->name) > 0 ) {
        left = mid + 1;
      } else {
        return mid;
      }
    }
  }
  return -1;
}





void inventory_add (struct inventory *inv, const char *item, int qty) {
  int pos = find(inv,item,0,inv->len-1);
  if (pos >= 0) {
    inv->storage[pos]->qtty += qty;
  } else {
    struct item *new = malloc(sizeof(struct item));
    new->name = dup(item);
    new->qtty = qty;

    if (inv->len == 0) {
      inv->storage[0] = new;
    } else {
      inv->storage = realloc(inv->storage, (inv->len + 1) * 
                             sizeof(char*));
      int pos = inv->len - 1;
      while (1) {
        if (pos < 0) {
          inv->storage[1] = inv->storage[0];
          inv->storage[0] = new;
          break;
        } else {
          if (strcmp(item,inv->storage[pos]->name) < 0) {
            inv->storage[pos + 1] = inv->storage[pos];
            pos--;
          } else {
            inv->storage[pos + 1] = new;
            break;
          }
        }
      }
    }
    inv->len++;
  }
}





int inventory_lookup(const struct inventory *inv, const char *item) {
  int pos = find(inv,item,0,inv->len-1);
  if ( pos >= 0 ) {
    return inv->storage[pos]->qtty;
  } else {
    return -1;
  }
}

void inventory_remove(struct inventory *inv, const char *item, int qty) {
  int pos = find(inv,item,0,inv->len-1);
  if ( pos < 0 ) return;
  inv->storage[pos]->qtty -= qty;
}










