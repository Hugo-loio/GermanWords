#include "word.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Members are the char pointers, they are 4, but last one can be NULL

Word * new_word(char ** members, int nw, int nt, int np){
  Word * word = (Word *) malloc(sizeof(Word));
  word->attributes = members;
  word->nwords=nw;
  word->ntranslations=nt;
  word->nproperties=np;
  word->picked = false;
  return word;
}

void destroy_word(Word* self){
  int N = self->nwords + self->ntranslations + self->nproperties;
  for(int i = 0; i < N; i++){
    free(self->attributes[i]);
  }
  free(self);
}

