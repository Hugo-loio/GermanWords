#include "window.h"
#include "interface.h"

void activate (GtkApplication* app, gpointer user_data)
{
  int outer_padding = 10;
  int iner_padding = 10;
  GtkWidget *window;
  Interface * interface = (Interface *) user_data;

  window = gtk_application_window_new (app);
  gtk_window_set_title (GTK_WINDOW (window), "GermanWords");

  GtkWidget * grid = gtk_grid_new();
  gtk_grid_set_row_spacing(GTK_GRID(grid), iner_padding);
  gtk_grid_set_column_spacing(GTK_GRID(grid), iner_padding);

  GtkWidget * category = gtk_combo_box_text_new();
  interface->category = category;
  g_object_set(category, "margin-top", outer_padding, NULL);
  g_object_set(category, "margin-left", outer_padding, NULL);
  gtk_widget_set_hexpand(category, TRUE);
  gtk_grid_attach(GTK_GRID(grid), category, 0, 0, 1, 1);
  for (int i = 0; i < interface->nword_types; i++){
    gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (category), interface->word_types[i]);
  }

  GtkWidget * english_word = gtk_label_new("English word");
  g_object_set(english_word, "margin-top", outer_padding, NULL);
  g_object_set(english_word, "margin-right", outer_padding, NULL);
  interface->english_word = english_word;
  gtk_widget_set_hexpand(english_word, TRUE);
  gtk_widget_set_halign(english_word, GTK_ALIGN_CENTER);
  gtk_grid_attach(GTK_GRID(grid), english_word, 1, 0 ,1 ,1);

  GtkWidget * insert = gtk_entry_new();
  interface->insert = insert;
  g_object_set(insert, "margin-left", outer_padding, NULL);
  gtk_widget_set_hexpand(insert, TRUE);
  gtk_grid_attach(GTK_GRID(grid), insert, 0, 1, 1, 1);

  GtkWidget * submit = gtk_button_new_with_label("Submit");
  g_object_set(submit, "margin-right", outer_padding, NULL);
  gtk_widget_set_hexpand(submit, TRUE);
  gtk_grid_attach(GTK_GRID(grid), submit, 1, 1, 1, 1);
  g_signal_connect(submit, "clicked", G_CALLBACK(check_answer), interface);

  GtkWidget * answer = gtk_label_new("Answer");
  g_object_set(answer, "margin-bottom", outer_padding, NULL);
  g_object_set(answer, "margin-left", outer_padding, NULL);
  interface->answer = answer;
  gtk_widget_set_hexpand(answer, TRUE);
  gtk_widget_set_halign(answer, GTK_ALIGN_CENTER);
  gtk_grid_attach(GTK_GRID(grid), answer, 0, 2, 1, 1);

  GtkWidget * next_word = gtk_button_new_with_label("Next word");
  g_object_set(next_word, "margin-bottom", outer_padding, NULL);
  g_object_set(next_word, "margin-right", outer_padding, NULL);
  gtk_widget_set_hexpand(next_word, TRUE);
  gtk_grid_attach(GTK_GRID(grid), next_word, 1, 2, 1, 1);
  g_signal_connect(next_word, "clicked", G_CALLBACK(random_word), interface);

  gtk_container_add(GTK_CONTAINER(window), grid);
  gtk_widget_show_all (window);
}
