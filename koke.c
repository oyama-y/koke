#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "koke.h"

#define LEAF_CHARS " .o+=*OSEB@"
#define GROW_STEP_MS (1000*60*60)

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
  srand(time(NULL));
  while(dms > 0) {
    double stepRatio = fmin(GROW_STEP_MS, dms)/GROW_STEP_MS;
    leaf_t leaves[KOKE_W][KOKE_H];
    memcpy(leaves, koke_p->leaves, sizeof(leaf_t)*KOKE_W*KOKE_H);
    int x, y;
    for(y = 0; y < KOKE_H; y++)
      for(x = 0; x < KOKE_W; x++) {
	leaf_t leafSum = 0.0;
	{
	  int dx, dy;
	  for(dx = -1; dx <= 1; dx++)
	    for(dy = -1; dy <= 1; dy++) {
	      if(dx == 0 && dy == 0)
		continue;
	      int lx = x+dx;
	      int ly = y+dy;
	      if(lx >= 0 && ly >= 0 && lx < KOKE_W && ly < KOKE_H)
		leafSum += leaves[lx][ly];
	    }
	}

	leaf_t presentLeaf = leaves[x][y];
	leaf_t nextLeaf = 0.0;
	if(leafSum >= 0.5 && leafSum < 3.5)
	  nextLeaf = drand48()/2.0+0.5;
	else if(leafSum < 0.5 && presentLeaf >= 0.5)
	  nextLeaf = 0.5;

	koke_p->leaves[x][y] = presentLeaf*(1.0-stepRatio/2.0) + nextLeaf*stepRatio/2.0;
      }
    dms -= GROW_STEP_MS;
  }
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
