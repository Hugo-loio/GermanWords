#include <stdbool.h>

#ifndef WORD_H
#define WORD_H

typedef struct WORD_STRUCT{
  char ** attributes;
  int nwords;
  int ntranslations;
  int nproperties;
  bool picked;
} Word;

Word * new_word(char **, int, int, int);
void destroy_word(Word*);
#endif

