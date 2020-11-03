#include <gtk/gtk.h>
#include <stdbool.h>
#include "word.h"

#ifndef INTERFACE_H
#define INTERFACE_H

typedef struct INTERFACE_STRUCT{
  GtkWidget * category;
  GtkWidget * english_word;
  GtkWidget * insert;
  GtkWidget * answer;
  char ** word_types;
  int nword_types;
  Word ** words;
  int nwords;
  Word * selected_word;
} Interface;

void read_file(Interface *);
void random_word(GtkWidget *, gpointer);
void check_answer(GtkWidget *, gpointer);
int word_size(char *, int,int);
int word_number(char *,int);
void destroy_interface(Interface *);

#endif
