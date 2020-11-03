#include "interface.h"
#include <errno.h>

void read_file(Interface * interface){
  int n = 0;
  FILE * file = fopen("data/words.txt", "r");
  if (file == NULL){
    printf("Error: prepositions file not read\n");
    printf("Error : errno='%s'.\n", strerror(errno));
  }
  else{
    char * line = NULL;
    size_t len = 0;
    int nlines = 0;
    ssize_t nchars;
    char c;
    //Get number of lines (equals number of words);
    for(c = getc(file); c != EOF; c = getc(file)){
      if(c == '\n'){
	nlines++;
      }
    }
    interface->nwords = nlines;
    interface->words = (Word **) malloc(nlines*sizeof(Word *));
    //Scan lines for words
    int nw = 1; //word amount
    int nt = 0; //Translation amount
    int np = 0; //Property amount
    int size = 0; //member size
    int number = 0; //member number
    int position = 0; //position in line
    int line_count = 0; 
    fseek(file,0,SEEK_SET);
    while((nchars = getline(&line, &len, file)) != -1){
      //Number of attributes in the line
      number = word_number(line, nchars);
      char ** word = (char **) malloc(number*sizeof(char *)); 
      for(int i = 0; i < number; i++){
	//Number of characters in the attribute (which might be more than on word)
	size = word_size(line, position, nchars);
	word[i] = (char *) malloc((size+1)*sizeof(char));
	for(int e = 0; e < nchars - position; e++){
	  if(line[e + position] == ','){
	    if(nt == 0){
	      nw++;
	    }
	    else if(np == 0){
	      nt++;
	    }
	    else{
	      np++;
	    }
	    position = position + e + 1;
	    word[i][e] = '\0';
	    break;
	  }
	  else if(line[e + position] == '|'){
	    if(nt == 0){
	      nt++;
	    }
	    else{
	      np++;
	    }
	    position = position + e + 1;
	    word[i][e] = '\0';
	    break;
	  }
	  else if(line[e + position] == '\n'){
	    word[i][e] = '\0';
	    break;
	  }
	  else{
	    word[i][e] = line[e + position];
	  }
	}
      }

      interface->words[line_count] = new_word(word, nw, nt, np);
      nw = 1;
      nt = 0;
      np = 0;
      position = 0;
      line_count++;
      free(line);
      len = 0;
    }
  }
  //Get all available word types
  interface->word_types = (char **) malloc(sizeof(char*));
  interface->nword_types = 1;
  Word * word_ptr = interface->words[0];
  char * type;
  interface->word_types[0] = word_ptr->attributes[word_ptr->nwords + word_ptr->ntranslations];
  for(int i = 1; i < interface->nwords; i++){
    word_ptr = interface->words[i];
    type = word_ptr->attributes[word_ptr->nwords + word_ptr->ntranslations];
    int e = 0;
    for (e; e < interface->nword_types; e++){
      if (strcmp(interface->word_types[e],type) == 0){
	break;
      }
    }
    if (e == interface->nword_types){
      interface->nword_types++;
      interface->word_types = (char**) realloc(interface->word_types, interface->nword_types*sizeof(char*));
      interface->word_types[interface->nword_types - 1] = type;
    }
  }
  fclose(file);
  return;
}

int word_size(char * line, int start,int nchars){
  int size = 0;
  for(int i = start; i < nchars; i++){
    if((line[i] != ',') && (line[i] != '|') && (line[i] != '\n')){
      size ++;
    }
    else{
      break;
    }
  }
  return size;
}

int word_number(char * line, int nchars){
  int n = 1;
  for(int i = 0; i < nchars; i++){
    if((line[i] == ',') || (line[i] == '|')){
      n++;
    }
  }
  return n;
}

void random_word(GtkWidget * next_word, gpointer user_data){
  Interface * interface = (Interface *) user_data;
  char * category = (char *) gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(interface->category)); 
  gtk_entry_set_text(GTK_ENTRY(interface->insert), "");
  //Get random word from the ones that haven't been picked
  Word * word = interface->words[0];
  int elegible = 0;
  for (int i = 0; i < interface->nwords; i++){
    word = interface->words[i];
    if(word->picked == false){
      if ((category == NULL) || (strcmp(word->attributes[word->nwords + word->ntranslations], category) == 0)){
	elegible++;
      }
    }
  }
  if (elegible != 0){
    int rand_index = rand()%elegible;
    int count = 0;
    for(int i = 0; i < interface->nwords; i++){
      word = interface->words[i];
      if(word->picked == false){
	if ((category == NULL) || (strcmp(word->attributes[word->nwords + word->ntranslations], category) == 0)){
	  if (count == rand_index){
	    word->picked = true;
	    break;
	  }
	  count++;
	}
      }
    }
  }
  else{
    for(int i = 0; i < interface->nwords; i++){
      interface->words[i]->picked = false;
    }
    random_word(next_word, user_data);
    free(category);
    return;
  }
  //Combine translations into one string
  char translation[100] = "";
  strcat(translation, word->attributes[word->nwords]);
  for(int i = 1; i < word->ntranslations; i++){
    strcat(translation, ", ");
    strcat(translation, word->attributes[word->nwords + i]);
  }
  //Update english word label
  gtk_label_set_text(GTK_LABEL(interface->english_word), (char *) &translation);
  interface->selected_word = word;
  free(category);
  return;
}

void check_answer(GtkWidget * submit, gpointer user_data){
  Interface * interface = (Interface *) user_data;
  const char * answer = gtk_entry_get_text(GTK_ENTRY(interface->insert));
  char * correct_answer;
  bool correct = false;
  bool firstletter = false;
  bool rest = false;
  if(answer[0] != '\0'){ //answer isn't empty
    for (int i = 0; i < interface->selected_word->nwords ; i++){
      correct_answer = interface->selected_word->attributes[i];
      //first letter is case insensitive
      if(answer[0] > 96 && answer[0] < 123){ //lower case
	if(answer[0] == correct_answer[0] || answer[0]-32 == correct_answer[0]){
	  firstletter = true;
	}
      }
      else if (answer[0] > 64 && answer[0] < 91){ //upper case
	if(answer[0] == correct_answer[0] || answer[0]+32 == correct_answer[0]){
	  firstletter = true;
	}
      }
      else if (answer[0] == correct_answer[0]){ //just in case first letter is a symbol
	firstletter = true;
      }
      if(answer[1] == '\0'){
	if(correct_answer[1] == '\0'){ //word has just one letter
	  rest = true;
	}
      }
      else if(correct_answer[1] != '\0'){ //word and answer have more than one letter
	if(strcmp(&answer[1],&correct_answer[1]) == 0){
	  rest = true;
	}
      }
      if (rest && firstletter){
	correct= true;
	break;
      }
    }
  }
  char answer_label[200] = "";
  if (correct){
    strcat(answer_label, "Correct!\n");
  }
  else{
    strcat(answer_label, "Wrong!\n");
  }
  strcat(answer_label, interface->selected_word->attributes[0]);
  for(int i = 1; i < interface->selected_word->nwords; i++){
    strcat(answer_label, "/");
    strcat(answer_label, interface->selected_word->attributes[i]);
  }
  for(int i = 0; i < interface->selected_word->nproperties; i++){
    strcat(answer_label, ", ");
    strcat(answer_label, interface->selected_word->attributes[interface->selected_word->nwords + interface->selected_word->ntranslations + i]);
  }
  gtk_label_set_text(GTK_LABEL(interface->answer), answer_label);
}

void destroy_interface(Interface * words){
  printf("TODO: destroy interface\n");
}


