#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "koke.h"

#define LEAF_CHARS " .o+=*OSEB@"

char leafToChar(leaf_t leaf) {
  int len = strlen(LEAF_CHARS);
  int index = leaf*len;
  if(index == len)
    index = len-1;
  if(index < 0 || index > len) {
    printf("Invalid koke\n");
    exit(1);
  }
  return LEAF_CHARS[index];
}

void newKoke(koke_t *koke_p) {
  int x, y;
  for(y = 0; y < KOKE_H; y++) 
    for(x = 0; x < KOKE_W; x++) {
      leaf_t leaf = 0.0;
      if(x == KOKE_W/2 && y == KOKE_H/2)
	leaf = 1.0;
      koke_p->leaves[x][y] = leaf;
    }
}

void growKoke(koke_t *koke_p, double dms) {
}

void printKoke(koke_t *koke_p) {
  int x, y;
  for(y = -1; y <= KOKE_H; y++) {
    for(x = -1; x <= KOKE_W; x++) {
      int xFrame = (x < 0) || (x >= KOKE_W);
      int yFrame = (y < 0) || (y >= KOKE_H);
      char c;
      if(xFrame && yFrame)
	c = '+';
      else if(xFrame)
	c = '|';
      else if(yFrame)
	c = '-';
      else {
	leaf_t leaf = koke_p->leaves[x][y];
	c = leafToChar(leaf);
      }
      printf("%c", c);
    }
    printf("\n");
  }
}
