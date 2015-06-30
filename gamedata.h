#ifndef gamedata_h
#define gamedata_h

#include "koke.h"

typedef struct {
  time_t lastTime;
  koke_t koke;
} gamedata_t;

void newGame(gamedata_t *gamedata_p);
int  loadGame(gamedata_t *gamedata_p);
void saveGame(gamedata_t *gamedata_p);
void clearGame();

#endif
