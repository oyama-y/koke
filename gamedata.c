#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "gamedata.h"
#include "koke.h"
#include "config.h"

int loadGame_file(gamedata_t *gamedata_p, FILE *file_p);
int saveGame_file(gamedata_t *gamedata_p, FILE *file_p);
FILE *openGameFile(const char *mode);

void newGame(gamedata_t *gamedata_p) {
  time(&gamedata_p->lastTime);
  newKoke(&gamedata_p->koke);
}

int loadGame(gamedata_t *gamedata_p) {
  FILE *file_p = openGameFile("r");
  if(file_p == NULL) 
    return 1;

  int result = loadGame_file(gamedata_p, file_p);
  if(fclose(file_p) == EOF) {
    printf("File close error\n");
    exit(1);
  }

  return result;
}

void saveGame(gamedata_t *gamedata_p) {
  FILE *file_p = openGameFile("wb");
  if(file_p == NULL) {
    printf("File open error\n");
    exit(1);
  }

  int result = saveGame_file(gamedata_p, file_p);

  if(fclose(file_p) == EOF) {
    printf("File close error\n");
    exit(1);
  }

  if(result) {
    printf("File write error\n");
    exit(1);
  }
}

int loadGame_file(gamedata_t *gamedata_p, FILE *file_p) {
  // Magic number
  {
    char magic[MAGIC_LEN+1];
    if(fread(magic, MAGIC_LEN, 1, file_p) != 1)
      return 1;
    magic[MAGIC_LEN] = '\0';
    if(strcmp(magic, MAGIC))
      return 1;
  }

  // Version
  int version;
  if(fread(&version, sizeof(int), 1, file_p) != 1)
    return 1;
  if(version != VERSION)
    return 1;

  // Content
  int len = sizeof(gamedata_t);
  gamedata_t buffer;
  if(fread(&buffer, len, 1, file_p) != 1)
    return 1;
  memcpy(gamedata_p, &buffer, sizeof(gamedata_t));

  if(fgetc(file_p) != EOF)
    return 1;
  
  return 0;
}

int saveGame_file(gamedata_t *gamedata_p, FILE *file_p) {
  if(fputs(MAGIC, file_p) == EOF)
    return 1;
  int version = VERSION;
  if(fwrite(&version, sizeof(int), 1, file_p) == EOF)
    return 1;
  if(fwrite(gamedata_p, sizeof(gamedata_t), 1, file_p) == EOF)
    return 1;
  return 0;
}

FILE *openGameFile(const char *mode) {
  char *home = getenv("HOME");
  if(home == NULL) {
    printf("Home path error\n");
    exit(1);
  }

  char *path = (char *) calloc(strlen(home)+strlen(GAMEDATA_PATH)+2, sizeof(char));
  sprintf(path, "%s/%s", home, GAMEDATA_PATH);
  
  FILE *file_p = fopen(path, mode);
  free(path);
  return file_p;
}