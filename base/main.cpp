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
  GuiappGET(entry_valve);
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

gboolean  valve_Display_Displayer(gpointer p_gptr)
{
   char tr[40] = "OPEN", fa[40] = "CLOSED";
  g_mutex_lock(valve_mutex);
   if(valve_state == true)
    gtk_label_set_text(GTK_LABEL(gui_app->entry_valve),tr);
  else if(valve_state == false)
    gtk_label_set_text(GTK_LABEL(gui_app->entry_valve),fa); 
  g_mutex_unlock(valve_mutex); 
  
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

  gtk_widget_set_sensitive (gui_app->button_close_port,TRUE);
  gtk_widget_set_sensitive (gui_app->button_open_port,FALSE);
  
}

extern "C" void button_reset_p0_clicked(GtkWidget *p_wdgt, gpointer p_data)
{
  g_mutex_lock(P0_max_mutex);
  sprintf(P0_max,"%3.2f","0");
  g_mutex_unlock(P0_max_mutex);
}
extern "C" void button_reset_p1_clicked(GtkWidget *p_wdgt, gpointer p_data)
{
  g_mutex_lock(P1_max_mutex);
  sprintf(P1_max,"%3.2f","0");
  g_mutex_unlock(P1_max_mutex);
}

extern "C" void button_closedevice_clicked(GtkWidget *p_wdgt, gpointer p_data ) 
{
  /*close ASIO*/
  std::cout<<"close";
  if(c != NULL)
  {
    c->close();
    free(c);
  }
  gtk_widget_set_sensitive (gui_app->button_close_port,FALSE);
  gtk_widget_set_sensitive (gui_app->button_open_port,TRUE);

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

  g_assert(valve_mutex == NULL);
  valve_mutex = new GMutex;
  g_mutex_init(valve_mutex);


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

  gtk_widget_set_sensitive (gui_app->button_close_port,FALSE);
  
  //display the gui
  gtk_widget_show(GTK_WIDGET(gui_app->window1));
  //this is going to call the Voltage_Display_Displayer function periodically
  gdk_threads_add_timeout(VOLTAGE_DISPLAY_UPDATE_MS,P0_Display_Displayer,NULL);
  //this is going to call the Voltage_Display_Displayer function periodically
  gdk_threads_add_timeout(100,P1_Display_Displayer,NULL);
  //this is going to call the Voltage_Display_Displayer function periodically
  gdk_threads_add_timeout(100,P1_MAX_Display_Displayer,NULL);
  //this is going to call the Voltage_Display_Displayer function periodically
  gdk_threads_add_timeout(100,P0_MAX_Display_Displayer,NULL);
  //this is going to call the Voltage_Display_Displayer function periodically
  gdk_threads_add_timeout(100,valve_Display_Displayer,NULL);


  gtk_main();

  if(gui_app)
    g_slice_free(Gui_Window_AppWidgets, gui_app);

  return 0;
}