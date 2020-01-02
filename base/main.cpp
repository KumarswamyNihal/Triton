/*TODOS
 *1: initialize asio and gtk window
 *2: write callback functions
 *3: Open() to open asio at the IP
 *4: close to close that port
 *5: For open 10s, create a thread to wait and call close but not join
 * currently working on making the GUI not look like a piece of crap
 */
#include <memory>

#include <gtk/gtk.h>
#include <stdlib.h>
#include <iostream>
#include <deque>
#include <thread>
#include "asio.hpp"
#include "chat_message.hpp"

#define __STDC_FORMAT_MACROS
#define VOLTAGE_DISPLAY_UPDATE_MS 100

#include "global.h"

void ObtainGuiWidgets(GtkBuilder *p_builder)
{
  #define GuiappGET(xx) gui_app->xx=GTK_WIDGET(gtk_builder_get_object(p_builder,#xx))
  GuiappGET(window1);
  GuiappGET(entry_ip);
  GuiappGET(button_open_port);
  GuiappGET(button_close_port);
  GuiappGET(entry_p0);
  GuiappGET(entry_p0_max);
  GuiappGET(entry_p1);
  GuiappGET(entry_p1_max);
  GuiappGET(button_reset_p1);
  GuiappGET(button_reset_p0);
  GuiappGET(button_open_valve);
  GuiappGET(button_close_valve);
  GuiappGET(button_5s);
  GuiappGET(button_10s);
  GuiappGET(button_15s);
    
}

gboolean  P0_Display_Displayer(gpointer p_gptr)
{
  // change mutex 
  //P0_mutex.lock();
  g_mutex_lock(P0_mutex);
  gtk_label_set_text(GTK_LABEL(gui_app->entry_p0),P0);
  g_mutex_unlock(P0_mutex);
  //P0_mutex.unlock();
  return true;
}

gboolean  P1_Display_Displayer(gpointer p_gptr)
{
  // do not change this function
  //P1_mutex.lock();
  g_mutex_lock(P1_mutex);
  gtk_label_set_text(GTK_LABEL(gui_app->entry_p1),P1);
  g_mutex_unlock(P1_mutex);
  //P1_mutex.lock();
  return true;
}

gboolean  P0_MAX_Display_Displayer(gpointer p_gptr)
{
  // do not change this function
  //P0_max_mutex.lock();
  g_mutex_lock(P0_max_mutex);
  gtk_label_set_text(GTK_LABEL(gui_app->entry_p0_max),P0_max);
  g_mutex_unlock(P0_max_mutex);
  //P0_max_mutex.lock();
  return true;
}

gboolean  P1_MAX_Display_Displayer(gpointer p_gptr)
{
  // do not change this function
  //P1_max_mutex.lock();
  g_mutex_lock(P1_max_mutex);
  gtk_label_set_text(GTK_LABEL(gui_app->entry_p1_max),P1_max);
  g_mutex_unlock(P1_max_mutex);
  //P1_max_mutex.lock();
  return true;
}

extern "C" void button_openport_clicked(GtkWidget *p_wdgt, gpointer p_data ) 
{
  /*Open asio*/
  const char *ip;
  ip = gtk_entry_get_text(GTK_ENTRY(gui_app->entry_ip));
  c = new chat_client(ip, "5000", "data.dat");
  
}

extern "C" void button_closedevice_clicked(GtkWidget *p_wdgt, gpointer p_data ) 
{
  /*close ASIO*/
  std::cout<<"close";
  if(c != NULL)
    c->close();
    /* gtk_entry_set_text(GTK_ENTRY(gui_app->entry_p0),"0 psi");
        gtk_entry_set_text(GTK_ENTRY(gui_app->entry_p1),"0 psi");

    gtk_entry_set_text(GTK_ENTRY(gui_app->entry_p0_max),"0 psi");

    gtk_entry_set_text(GTK_ENTRY(gui_app->entry_p1_max),"0 psi"); */
  gtk_label_set_text(GTK_LABEL(gui_app->entry_p1_max),"0 psi");
  gtk_label_set_text(GTK_LABEL(gui_app->entry_p0_max),"0 psi");
  gtk_label_set_text(GTK_LABEL(gui_app->entry_p1),"0 psi");
  gtk_label_set_text(GTK_LABEL(gui_app->entry_p0),"0 psi");
    

}

int main(int argc, char **argv)
{
  GtkBuilder *builder;
  GError *err = NULL;
  gtk_init(&argc, &argv);
  
  g_assert(P0_mutex == NULL);
  P0_mutex = new GMutex;
  g_mutex_init(P0_mutex);

  g_assert(P1_mutex == NULL);
  P1_mutex = new GMutex;
  g_mutex_init(P1_mutex);


g_assert(P0_max_mutex == NULL);
  P0_max_mutex = new GMutex;
  g_mutex_init(P0_max_mutex);

g_assert(P1_max_mutex == NULL);
  P1_max_mutex = new GMutex;
  g_mutex_init(P1_max_mutex);


  //create gtk_instance for visualization
  gui_app = g_slice_new(Gui_Window_AppWidgets);

  //builder
  builder = gtk_builder_new();
  gtk_builder_add_from_file(builder, "CF.glade", &err);

    //error handling
  if(err)
    {
      g_error(err->message);
      g_error_free(err);
      g_slice_free(Gui_Window_AppWidgets, gui_app);
      exit(-1);
    }
  
  // Obtain widgets that we need
  ObtainGuiWidgets(builder);

  // Connect signals
  gtk_builder_connect_signals(builder, gui_app);

  // Destroy builder now that we created the infrastructure
  g_object_unref(G_OBJECT(builder));

  //display the gui
  gtk_widget_show(GTK_WIDGET(gui_app->window1));
  //gtk_main();
  //this is going to call the Voltage_Display_Displayer function periodically
  gdk_threads_add_timeout(VOLTAGE_DISPLAY_UPDATE_MS,P0_Display_Displayer,NULL);
  //this is going to call the Voltage_Display_Displayer function periodically
  gdk_threads_add_timeout(100,P1_Display_Displayer,NULL);
  //this is going to call the Voltage_Display_Displayer function periodically
  gdk_threads_add_timeout(100,P1_MAX_Display_Displayer,NULL);
  //this is going to call the Voltage_Display_Displayer function periodically
  gdk_threads_add_timeout(100,P0_MAX_Display_Displayer,NULL);

  gtk_main();

  if(gui_app)
    g_slice_free(Gui_Window_AppWidgets, gui_app);

  return 0;
}