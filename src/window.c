#include "window.h"
#include "interface.h"

void activate (GtkApplication* app, gpointer user_data)
{
  GtkWidget *window;
  Interface * interface = (Interface *) user_data;

  window = gtk_application_window_new (app);
  gtk_window_set_title (GTK_WINDOW (window), "Window");

  GtkWidget * grid = gtk_grid_new();

  GtkWidget * category = gtk_combo_box_text_new();
  interface->category = category;
  gtk_widget_set_halign(category, GTK_ALIGN_CENTER);
  gtk_widget_set_valign(category, GTK_ALIGN_CENTER);
  gtk_widget_set_hexpand(category, FALSE);
  gtk_widget_set_vexpand(category, FALSE);
  gtk_grid_attach(GTK_GRID(grid), category, 0, 0, 1, 1);
  for (int i = 0; i < interface->nword_types; i++){
    gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (category), interface->word_types[i]);
  }

  GtkWidget * english_word = gtk_label_new("English word");
  interface->english_word = english_word;
  gtk_widget_set_halign(english_word, GTK_ALIGN_CENTER);
  gtk_widget_set_valign(english_word, GTK_ALIGN_CENTER);
  gtk_widget_set_hexpand(english_word, TRUE);
  gtk_widget_set_vexpand(english_word, TRUE);
  gtk_grid_attach(GTK_GRID(grid), english_word, 1, 0 ,1 ,1);

  GtkWidget * insert = gtk_entry_new();
  interface->insert = insert;
  gtk_widget_set_hexpand(insert, FALSE);
  gtk_widget_set_vexpand(insert, FALSE);
  gtk_widget_set_halign(insert, GTK_ALIGN_CENTER);
  gtk_widget_set_valign(insert, GTK_ALIGN_CENTER);
  gtk_grid_attach(GTK_GRID(grid), insert, 0, 1, 1, 1);

  GtkWidget * submit = gtk_button_new_with_label("Submit");
  gtk_widget_set_hexpand(submit, TRUE);
  gtk_widget_set_vexpand(submit, TRUE);
  gtk_widget_set_halign(submit, GTK_ALIGN_CENTER);
  gtk_widget_set_valign(submit, GTK_ALIGN_CENTER);
  gtk_grid_attach(GTK_GRID(grid), submit, 1, 1, 2, 1);

  GtkWidget * answer = gtk_label_new("Answer");
  interface->answer = answer;
  gtk_widget_set_hexpand(answer, TRUE);
  gtk_widget_set_vexpand(answer, TRUE);
  gtk_widget_set_halign(answer, GTK_ALIGN_CENTER);
  gtk_widget_set_valign(answer, GTK_ALIGN_CENTER);
  gtk_grid_attach(GTK_GRID(grid), answer, 0, 2, 1, 1);

  GtkWidget * next_word = gtk_button_new_with_label("Next word");
  gtk_widget_set_hexpand(next_word, TRUE);
  gtk_widget_set_vexpand(next_word, TRUE);
  gtk_widget_set_valign(next_word, GTK_ALIGN_CENTER);
  gtk_widget_set_halign(next_word, GTK_ALIGN_CENTER);
  gtk_grid_attach(GTK_GRID(grid), next_word, 1, 2, 1, 1);
  g_signal_connect(next_word, "clicked", G_CALLBACK(random_word), interface);

  gtk_container_add(GTK_CONTAINER(window), grid);
  gtk_widget_show_all (window);
}
