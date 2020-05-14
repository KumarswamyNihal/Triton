#include <gtk/gtk.h>
#include <stdlib.h>
#include <iostream>
#include <thread>
#include <mutex>

#include "chat_client.h"

#define __STDC_FORMAT_MACROS

#pragma once

typedef struct 
{
  GtkWidget *window1; //visualization window
  GtkWidget *entry_ip; //Ip entry 
  GtkWidget *button_open_port;
  GtkWidget *button_close_port;
  GtkWidget *entry_p0;
  GtkWidget *entry_p0_max;
  GtkWidget *entry_p1;
  GtkWidget *entry_p1_max;
  GtkWidget *entry_valve;
  GtkWidget *button_reset_p1;
  GtkWidget *button_reset_p0;
  GtkWidget *button_open_valve;
  GtkWidget *button_close_valve;
  GtkWidget *button_5s;
  GtkWidget *button_10s;
  GtkWidget *button_15s;
} Gui_Window_AppWidgets;

extern Gui_Window_AppWidgets *gui_app;



extern char P0[40];
extern char P1[40];
extern char P0_max[40];
extern char P1_max[40];
extern bool valve_state;
extern GMutex *P0_mutex;
extern GMutex *P1_mutex;
extern GMutex *P0_max_mutex;
extern GMutex *P1_max_mutex;
extern GMutex *valve_mutex;

extern chat_client *c;