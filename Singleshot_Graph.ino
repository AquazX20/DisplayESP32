#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define __DEBUG__


#include "DisplayESP32_SSD1306.h"

int i = 0;
int multiplier = 1;
int samples = 1000;

Screen screen;
Keyboard keyboard(13, 12, 14, 27, 30, &screen);

void setup(){
  screen.configure(true, 0x3C);
  
  screen.createMenu(128, 13);             //Menu 0
  
  screen.createCGraph("Test", 30, 50, 75, 30, 0, 100, 0, 25, 250, 0, &samples);   //Graph 0

  screen.createModifier("Multiplier", &multiplier, 5, 1, 1);  //Modifier 0
  screen.createModifier("Samples number", &samples, 1000, 500, 10);   //Modifier 1
  
  screen.createOption(0, "Adjust multiplier", 2, 0);
  screen.createOption(0, "Adjust samples #", 2, 1);

  screen.createOption(0, "Run test", 1, 0);       
}

void loop(){
  while(screen.getCurrentScreen() != 0 || screen.getContentType() != 1){
    screen.control();            //Controls the screen and redraws if needed while not in the test's graph
    keyboard.control();
    delay(REFRESH);
  }

  for(int j = 0; j <= samples; j++){
    if(i <= 100){
      screen.graphAssignValue(0, i);    //Assigning a demo value to Graph 0
      i += multiplier;
    }
    else
      i = 0;
    screen.control();
    Serial.println(samples);
    delay(REFRESH);
  }

  while(screen.getCurrentScreen() == 0 && screen.getContentType() == 1){
    screen.control();            //Controls the screen and redraws if needed while in the test's graph
    keyboard.control();
    delay(REFRESH);
  }
}
