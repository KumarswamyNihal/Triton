#include "global.h"

Gui_Window_AppWidgets *gui_app; //structure to keep all interesting widgets

char P0[40] = "0";
char P1[40]= "0";
char P0_max[40]= "0";
char P1_max[40]= "0";
bool valve_state = false;

GMutex * P0_mutex;
GMutex * P1_mutex;
GMutex *P0_max_mutex;
GMutex * P1_max_mutex;
GMutex * valve_mutex;

chat_client *c;