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
  P0_mutex.lock();
  gtk_label_set_text(GTK_LABEL(gui_app->entry_p0),P0);
  P0_mutex.unlock();
  return true;
}

gboolean  P1_Display_Displayer(gpointer p_gptr)
{
  // do not change this function
  P1_mutex.lock();
  gtk_label_set_text(GTK_LABEL(gui_app->entry_p1),P1);
  P1_mutex.lock();
  return true;
}

gboolean  P0_MAX_Display_Displayer(gpointer p_gptr)
{
  // do not change this function
  P0_max_mutex.lock();
  gtk_label_set_text(GTK_LABEL(gui_app->entry_p0_max),P0_max);
  P0_max_mutex.lock();
  return true;
}

gboolean  P1_MAX_Display_Displayer(gpointer p_gptr)
{
  // do not change this function
  P1_max_mutex.lock();
  gtk_label_set_text(GTK_LABEL(gui_app->entry_p1_max),P1_max);
  P1_max_mutex.lock();
  return true;
}

extern "C" void button_openport_clicked(GtkWidget *p_wdgt, gpointer p_data ) 
{
  /*Open asio*/

  
}

extern "C" void button_closedevice_clicked(GtkWidget *p_wdgt, gpointer p_data ) 
{
  /*close ASIO*/

}