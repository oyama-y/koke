#include <stdio.h>
#include <string.h>
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

  if(argc >= 2) {
    char *command = argv[1];
    if(strcmp(command, "water") == 0) {
      // Water
    }
  }

  printKoke(&sv.koke);

  sv.lastTime = t;
  saveGame(&sv);
  
  return 0;
}
