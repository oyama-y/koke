#ifndef koke_h
#define koke_h

#define KOKE_SUGI   0
#define KOKE_SIRAGA 1
#define KOKE_HIKARI 2

#define KOKE_TYPE 3
#define KOKE_W 17
#define KOKE_H 9

typedef double leaf_t;

typedef struct {
  leaf_t leaves[KOKE_TYPE][KOKE_W][KOKE_H];
  double water;
} koke_t;

typedef struct {
  leaf_t leaves[KOKE_W][KOKE_H];
  double water;
} koke_1_t;

typedef struct {
  leaf_t leaves[KOKE_W][KOKE_H];
} koke_0_t;

void newKoke(koke_t *koke_p);
void growKoke(koke_t *koke_p, double dms);
void waterKoke(koke_t *koke_p);
void printKoke(koke_t *koke_p);

void koke_update_0(const koke_0_t *koke_old_p, koke_1_t *koke_new_p);
void koke_update_1(const koke_1_t *koke_old_p, koke_t   *koke_new_p);

#endif
