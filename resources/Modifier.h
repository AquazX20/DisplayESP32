class Modifier{       //ContentType (2)
  private:

  String title;      //Title of the modifier
  int *value;        //Pointer to the value to modify
  int max;           //Highest value to set
  int min;           //Lowest value to set
  int step;          //Size of the step

  int previousScreen = 0;
  int previousContentType = 0;

  public:

  void configure(String title, int *value, int max, int min, int step){
    this->title = title;
    this->value = value;
    this->max = max;
    this->min = min;
    this->step = step;
  }

  void drawModifier(){            //Draws or redraws the screen
    display.clearDisplay();
    display.fillRect(0, 0,  127 , 16, SSD1306_WHITE);
    display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
    display.setTextSize(1);
    display.setCursor(2, 4);
    display.println(this->title);

    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(3);
    display.setCursor(2, ((DISP_HEIGHT - 16 - 15)/2) + 16);
    display.println(*this->value);

    display.display();

    display.setTextSize(1);
  }

  void increaseValue(){         //Increases the value of the variable
    if((*this->value + this->step) <= this->max){
      *this->value += this->step;
    }
  }

  void decreaseValue(){         //Decreases the value of the variable
    if((*this->value - this->step) >= this->min){
      *this->value -= this->step;
    }
  }

  void setPreviousScreen(int prev){
    this->previousScreen = prev;
  }

  void setPreviousContentType(int prev){
    this->previousContentType = prev;
  }

  int getPreviousScreen(){
    int prev = this->previousScreen;
    return prev;
  }

  int getPreviousContentType(){
    int prev = this->previousContentType;
    return prev;
  }
};
