#include <stdio.h>
#include <time.h>
#include "gamedata.h"

int main(int argc, char *argv[]) {
  gamedata_t sv;
  if(loadGame(&sv)) {
    printf("New game\n");
    newGame(&sv);
  }

  time_t t;
  time(&t);
  
  growKoke(&sv.koke, difftime(t, sv.lastTime)*1000.0);
  printKoke(&sv.koke);

  sv.lastTime = t;
  saveGame(&sv);
  
  return 0;
}
