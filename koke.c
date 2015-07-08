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

int rgbToXtermColor(double r, double g, double b) {
  int rGrad = r*6.0;
  int gGrad = g*6.0;
  int bGrad = b*6.0;
  if(rGrad == 6) rGrad = 5;
  if(gGrad == 6) gGrad = 5;
  if(bGrad == 6) bGrad = 5;
  return 16 + rGrad*6*6 + gGrad*6 + bGrad;
}

char leafToChar(leaf_t* leaves) {
  leaf_t leafMax = 0.0;
  {
    int type;
    for(type = 0; type < KOKE_TYPE; type++) {
      leaf_t leaf = leaves[type];
      if(leaf > leafMax)
	leafMax = leaf;
    }
  }
  int len = strlen(LEAF_CHARS);
  int index = leafMax*len;
  if(index == len)
    index = len-1;
  if(index < 0 || index > len) {
    printf("Invalid koke\n");
    exit(1);
  }
  return LEAF_CHARS[index];
}

int leafToXtermColor(leaf_t* leaves) {
  leaf_t l_sugi = leaves[KOKE_SUGI];
  leaf_t l_siraga = leaves[KOKE_SIRAGA];
  leaf_t l_hikari = leaves[KOKE_HIKARI];
  double r = fmax(l_siraga, l_hikari);
  double g = fmax(l_sugi, fmax(l_siraga, l_hikari));
  double b = l_hikari;
  return rgbToXtermColor(r, g, b);
}

void newKoke(koke_t *koke_p) {
  srand(time(NULL));
  int xLeaf = rand()%KOKE_W;
  int yLeaf = rand()%KOKE_H;
  int type, x, y;
  for(type = 0; type < KOKE_TYPE; type++)
    for(y = 0; y < KOKE_H; y++)
      for(x = 0; x < KOKE_W; x++) {
	leaf_t leaf = 0.0;
	if(type == KOKE_SUGI && x == xLeaf && y == yLeaf)
	  leaf = 1.0;
	koke_p->leaves[type][x][y] = leaf;
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
    leaf_t leaves[KOKE_TYPE][KOKE_W][KOKE_H];
    memcpy(leaves, koke_p->leaves, sizeof(leaf_t)*KOKE_TYPE*KOKE_W*KOKE_H);
    int type, x, y;
    for(type = 0; type < KOKE_TYPE; type++)
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
		  leafSum += leaves[type][lx][ly];
	      }
	  }

	  leaf_t presentLeaf = leaves[type][x][y];
	  leaf_t nextLeaf = 0.0;
	  if(leafSum >= 0.5 && leafSum < 3.5)
	    nextLeaf = drand48()/2.0+0.5;
	  else if(leafSum < 0.5 && presentLeaf >= 0.5)
	    nextLeaf = 0.5;

	  nextLeaf *= avgWater;

	  koke_p->leaves[type][x][y] = presentLeaf*(1.0-stepRatio/2.0) + nextLeaf*stepRatio/2.0;
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
	leaf_t leaves[KOKE_TYPE];
	int type;
	for(type = 0; type < KOKE_TYPE; type++)
	  leaves[type] = koke_p->leaves[type][x][y];
	c = leafToChar(leaves);
	if(COLOR_KOKE) {
	  int color = leafToXtermColor(leaves);
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

void koke_update_0(const koke_0_t *koke_old_p, koke_1_t *koke_new_p) {
  memcpy(koke_new_p->leaves, koke_old_p->leaves, sizeof(leaf_t)*KOKE_W*KOKE_H);
  koke_new_p->water = 1.0;
}

void koke_update_1(const koke_1_t *koke_old_p, koke_t *koke_new_p) {
  int type, x, y;
  for(type = 0; type < KOKE_TYPE; type++)
    for(y = 0; y < KOKE_H; y++)
      for(x = 0; x < KOKE_W; x++)
	koke_new_p->leaves[type][x][y] = (type == 0 ? koke_old_p->leaves[x][y] : 0.0);
  koke_new_p->water = koke_old_p->water;
}
