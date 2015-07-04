#ifndef koke_h
#define koke_h

#define KOKE_W 17
#define KOKE_H 9

typedef double leaf_t;

typedef struct {
  leaf_t leaves[KOKE_W][KOKE_H];
} koke_t;

void newKoke(koke_t *koke_p);
void growKoke(koke_t *koke_p, double dms);
void waterKoke(koke_t *koke_p);
void printKoke(koke_t *koke_p);

#endif
