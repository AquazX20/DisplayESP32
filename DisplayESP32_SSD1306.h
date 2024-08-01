#define MAX_OPTIONS 10      //Maximum number of options for each menu
#define MAX_MENUS 3
#define MAX_GRAPHS 3
#define MAX_MODIFIERS 3
#define DISP_WIDTH 128      // OLED display width
#define DISP_HEIGHT 64      // OLED display height
#define REFRESH 10          //Refresh time in ms

//The following constants are used to configure the behaviour of the interface
// MAX_OPTIONS defines the maximum number of options a menu can hold
// MAX_MENUS declares the maximum number of menu screens in the interface
// MAX_GRAPHS is the maximum number of graphs to create
// DISP_WIDTH and DISP_HEIGHT are hardware specific (SSD1306)
// REFRESH: is the time in miliseconds the interface will take in refreshing (this time affects the loop, keep that in mind)

Adafruit_SSD1306 display(DISP_WIDTH, DISP_HEIGHT, &Wire, -1);

#include "resources/Option.h"
#include "resources/Menu.h"
#include "resources/Graph.h"
#include "resources/Modifier.h"
#include "resources/Screen.h"
#include "resources/Keyboard.h"
