#include <gtk/gtk.h>
#include "window.h"
#include "interface.h"

int main (int argc, char **argv)
{
  srand(time(NULL));
  GtkApplication *app;
  int status;
  Interface * interface = (Interface *) malloc(sizeof(Interface));
  read_file(interface);

  //TODO ler o ficheiro e passar como argumento para as funções activate
  app = gtk_application_new ("org.gtk.example", G_APPLICATION_FLAGS_NONE);
  g_signal_connect (app, "activate", G_CALLBACK(activate), interface);
  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);

  destroy_interface(interface);
  printf("Status %d\n", status);
  return status;
}
