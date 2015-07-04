#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "koke.h"
#include "config.h"

#define LEAF_CHARS " .o+=*OSEB@"
#define GROW_STEP_MS (1000*60*60)
#define WATER_MS     (1000*60*60*24*2)

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

int leafToXtermColor(leaf_t leaf) {
  int len = 5;
  int grad = leaf*len;
  if(grad == len)
    grad = len-1;
  return 22+grad*6;
}

void newKoke(koke_t *koke_p) {
  srand(time(NULL));
  int xLeaf = rand()%KOKE_W;
  int yLeaf = rand()%KOKE_H;
  int x, y;
  for(y = 0; y < KOKE_H; y++)
    for(x = 0; x < KOKE_W; x++) {
      leaf_t leaf = 0.0;
      if(x == xLeaf && y == yLeaf)
        leaf = 1.0;
      koke_p->leaves[x][y] = leaf;
    }
}

void growKoke(koke_t *koke_p, double dms) {
  double newWater = koke_p->water - dms/WATER_MS;
  if(newWater < 0)
    newWater = 0;
  double avgWater = (koke_p->water+newWater)/2;
  koke_p->water = newWater;

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

	nextLeaf *= avgWater;

	koke_p->leaves[x][y] = presentLeaf*(1.0-stepRatio/2.0) + nextLeaf*stepRatio/2.0;
      }
    dms -= GROW_STEP_MS;
  }
}

void waterKoke(koke_t *koke_p) {
  koke_p->water = 1.0;
}

void printKoke(koke_t *koke_p) {
  char tag[KOKE_W+1];
  sprintf(tag, "[%d%%]", (int) (koke_p->water*100));
  int tagLen = strlen(tag);

  int x, y;
  for(y = -1; y <= KOKE_H; y++) {
    for(x = -1; x <= KOKE_W; x++) {
      int xFrame = (x < 0) || (x >= KOKE_W);
      int yFrame = (y < 0) || (y >= KOKE_H);
      char c;
      int inTag = 0;
      if(xFrame && yFrame)
	c = '+';
      else if(xFrame)
	c = '|';
      else if(yFrame) {
	int tagOffset = 1+KOKE_W/2-(tagLen+1)/2;
	if(x >= tagOffset && x < tagOffset+tagLen && y == -1) {
	  c = tag[x-tagOffset];
	  inTag = 1;
	} else
	  c = '-';
      } else {
	leaf_t leaf = koke_p->leaves[x][y];
	c = leafToChar(leaf);
	if(COLOR_KOKE) {
	  int color = leafToXtermColor(leaf);
	  printf("\033[38;05;%dm", color);
	}
      }
      if(COLOR_KOKE && inTag)
	printf("\033[1;38;05;27m");
      printf("%c", c);
      if(COLOR_KOKE)
	printf("\033[0m");
    }
    printf("\n");
  }
}

void koke_update_0(const koke_0_t *koke_old_p, koke_t *koke_new_p) {
  memcpy(koke_new_p->leaves, koke_old_p->leaves, sizeof(leaf_t)*KOKE_W*KOKE_H);
  koke_new_p->water = 1.0;
}
