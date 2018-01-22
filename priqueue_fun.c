#priqueue.h
struct priqueue;

// NOTES: All of the following functions REQUIRE:
//        pointers to a priqueue (e.g., pq) are valid (not NULL)
//
//       For times, n is the size of the priqueue
//
//       While the contracts below indicate that the order for "ties"
//       does not matter, you must follow the priqueue instructions


// priqueue_create() returns a pointer to a new (empty) priqueue
// effects: allocates memory (caller must call priqueue_destroy)
// time: O(1)
struct priqueue *priqueue_create(void);

// priqueue_destroy(pq) frees all dynamically allocated memory 
// effects: the memory at pq is invalid (freed)
// time: O(1)
void priqueue_destroy(struct priqueue *pq);

// priqueue_length(pq) determines how many items are in pq
// time: O(1)
int priqueue_length(const struct priqueue *pq);

// priqueue_add(pq, item, priority) inserts item with priority into pq
// effects: modifies pq
// time: O(logn)
void priqueue_add(struct priqueue *pq, int item, int priority);

// priqueue_front(pq) returns the item with the highest priority
//   If there are multiple items with the same priority, 
//   the order does not matter.
// requires: pq is not empty
// time : O(1)
int priqueue_front(const struct priqueue *pq);

// priqueue_remove(pq) removes and returns the item with the maximum 
//   priority in pq.
//   If there are multiple items with the same priority, 
//   the order does not matter.
// requires: pq is not empty
// effects: modifies pq
// time: O(logn)
int priqueue_remove(struct priqueue *pq);

// NOTE: priqueue_print violates the principle of information hiding
//       but is required to properly test your code

// priqueue_print(pq) prints pq
// effects: displays output
// example: It prints in the order it is stored in the array. 
//   For example, if the heap is currently
//             (1,99)
//             /    \
//         (5,20)  (2,30)
//   then it will be printed as "[(1:99),(5:20),(2:30)]\n"
//   where each node is printed as (item:priority).
//   if empty, it prints as "[empty]\n"
// time: O(n)
void priqueue_print(const struct priqueue *pq);



#include "priqueue.h"
#include <stdio.h>
#include <stdlib.h>

struct priqueue {
  int len;
  int maxlen;
  int *value;
  int *pri;
};

// my_swap(pq,pos1,pos2) swaps the two elements with index pos1 and pos2 
// in the array pq->value and pq->pri
// effect: modifies pq
// require: 0 <= pos1,pos2 < pq->len;
// time: O(1)
void my_swap(struct priqueue *pq, int pos1, int pos2) {
  int temp0 = pq->value[pos1];
  int temp1 = pq->pri[pos1];
  pq->value[pos1] = pq->value[pos2];
  pq->pri[pos1] = pq->pri[pos2];
  pq->value[pos2] = temp0;
  pq->pri[pos2] = temp1;
}





struct priqueue *priqueue_create(void) {
  struct priqueue *new = malloc( sizeof(struct priqueue) );
  new->len = 0;
  new->maxlen = 1;
  new->value = malloc( sizeof(int) * new->maxlen );
  new->pri = malloc( sizeof(int) * new->maxlen );
  return new;
}

void priqueue_destroy(struct priqueue *pq) {
  free(pq->value);
  free(pq->pri);
  free(pq);
}

int priqueue_length(const struct priqueue *pq) {
  if (pq) {
  return pq->len;
  } else {
    return 0;
  }
}

void priqueue_add(struct priqueue *pq, int item, int priority) {
  if (pq->len == pq->maxlen) {
    pq->maxlen *= 2;
    pq->value = realloc(pq->value, sizeof(int) * pq->maxlen);
    pq->pri = realloc(pq->pri, sizeof(int) * pq->maxlen);
  }
  pq->value[pq->len] = item;
  pq->pri[pq->len] = priority;

  int pos = pq->len;
  pq->len++;

  while (1) {
    if (pos == 0) break;
    if ( pq->pri[(pos - 1) / 2] < pq->pri[pos] ) {
      my_swap(pq,pos,(pos-1)/2);
      pos = (pos-1) / 2;
    } else {
      break;
    }
  }
}

int priqueue_front(const struct priqueue *pq) {
  return pq->value[0];
}

int priqueue_remove(struct priqueue *pq) {
  int backup = pq->value[0];
  pq->value[0] = pq->value[pq->len-1];
  pq->pri[0] = pq->pri[pq->len-1];
  pq->len--;

  int pos = 0;
  while(1) {
    if ( (2 * pos + 1) > pq->len-1 ) break;
    if ( (2 * pos + 2 ) > pq->len-1 ) {
      if (pq->pri[2 * pos + 1] > pq->pri[pos]) {
        my_swap(pq,pos,2*pos+1);
        break;
      } else {
        break;
      }
    } else {
      if ( pq->pri[2 * pos + 1] > pq->pri[pos] \
         && pq->pri[2 * pos + 1] >= pq->pri[2 * pos + 2]) {
        my_swap(pq,pos,2*pos+1);
        pos = 2 * pos + 1;
      } else {
        if ( pq->pri[2 * pos + 2] > pq->pri[pos] \
           && pq->pri[2 * pos + 2] > pq->pri[2 * pos + 1]) {
          my_swap(pq,pos,2*pos+2);
          pos = 2 * pos + 2;
        } else {
          break;
        }
      }
    }
  }
  return backup;
}


void priqueue_print(const struct priqueue *pq) {
  printf("[");
  if (pq->len == 0)  {
    printf("empty");
  } else {
    printf("(%d:%d)",pq->value[0],pq->pri[0]);
    for(int i = 1; i < pq->len; i++) {
      printf(",(%d:%d)",pq->value[i],pq->pri[i]);
    }
  }
  printf("]\n");
}