#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define __DEBUG__


#include "DisplayESP32_SSD1306.h"

int i = 0;
int multiplier = 1;

Screen screen;
Keyboard keyboard(13, 12, 14, 27, 30, &screen);

void setup(){
  screen.configure(true, 0x3C);
  
  screen.createMenu(128, 13);             //Menu 0
  screen.createMenu(128, 13);             //Menu 1

  /*String title, char graphType, double xpos, double ypos, double width, double height,
  double yminimum, double ymaximum, double xminimum, double xmaximum, double yStepSize, double xStepSize, double digit*/
  
  screen.createVGraph("Grafica 1", 25, 60, 40, 40, 0, 100, 10, 0);               //Graph 0
  screen.createHGraph("Grafica 2", 10, 40, 100, 20, 0, 100, 10, 0);              //Graph 1
  screen.createCGraph("Grafica 3", 30, 50, 75, 30, 0, 100, 0, 1000, 25, 250, 0);   //Graph 2

  screen.createModifier("Modify variable", &multiplier, 5, 1, 1);
  
  screen.createOption(0, "Vertical graph", 1, 0);
  //Creates the first option in Menu 0, directing to a graph (contentType = 1 (Graph)), 0 (Graph 0)
  screen.createOption(0, "Horizontal graph", 1, 1);       
  screen.createOption(0, "Cartesian graph", 1, 2);
  screen.createOption(0, "Extra option", 0, 1);

  screen.createOption(1, "Test", 1, 3);
  screen.createOption(1, "Working?", 2, 2);
  screen.createOption(1, "Modify variable", 2, 0);

//  screen.increasePos();
//  screen.increasePos();
//  screen.goTo();
//  screen.graphAssignValue(2, 50);
//  screen.goBack();
//  screen.increasePos();
//  screen.goTo();
//  screen.goBack();
//  screen.decreasePos();
}

void loop(){
  screen.control();            //Controls the screen and redraws if needed
  keyboard.control();

  if(i <= 100){
    screen.graphAssignValue(1, i);    //Assigning a demo value to Graph 1
    screen.graphAssignValue(2, i);    //Assigning a demo value to Graph 2
    i += multiplier;
  }
  else
    i = 0;

  delay(REFRESH);                   //Refresh time (approx)
}