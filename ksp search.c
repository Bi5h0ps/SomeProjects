#include <stdbool.h>
#substring.h
// is_substring(haystack, needle) determines if the string
//   needle is contained within the string haystack
// time: O(n + m), where n & m are the lengths of haystack & needle
bool is_substring(const char *haystack, const char *needle);

#include "substring.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// preffix(needle) produces a temporary array based on the KMP algorithm
// Every element in the arrary correspond to the element with same index
// in the string needle and tells if the charrecter is a duplicate 
// or not. If the character has already shown in the arrary, then the 
// element in the produced array indicates the index where the most 
// recent shown time of this duplicate.
// effect: allocates memory (caller must free)
// runtime: O (n) n is the length of needle;

int *preffix (const char *needle) {
  int len = strlen(needle);
  int* result = malloc( len * sizeof(int) );
  int j = 0;
  int i = 1;
  result[0] = 0;

  while (1) {
    if (i == len) break;
    if ( needle[j] == needle[i] ) {
      result[i] = j + 1;
      i++;
      j++;
    } else {
      if ( (needle[j] != needle[i]) && (j != 0) ) {
        j = result[j - 1];
      } else {
        if ( (needle[j] != needle[i]) && (j == 0) ) {
          result[i] = 0;
          i++;
        } else {
          continue;
        }
      }
    }
  }
  return result;
}


bool is_substring(const char *haystack, const char *needle) {
  int pos = 0;
  int pos0 = 0;
  int haylen = strlen(haystack);
  int needlen = strlen(needle);
  bool result = false;
  if (needlen == 0) {
    return true;
  } else {
    if (needlen > haylen) {
      return false;
    } else {
      int* info = preffix(needle);
      while (1) {
        if (pos == haylen) {
          break;
        } else {
          if (pos0 == needlen) {
            result = true;
            break;
          } else {
            if (haystack[pos] == needle[pos0]) {
              pos++;
              pos0++;
            } else {
              if (pos0 != 0) {
                pos0 = info[pos0 - 1];
              } else {
                pos++;
              }
            }
          }
        }
      }
      free(info);
      return result;
    }
  }
}



