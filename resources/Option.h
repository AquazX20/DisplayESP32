class Option{
  private:

  int sizex;    //Defines the size it will occupy in the x axis (width), this value is gotten from the menu
  int sizey;    //Defines the height of the option (this value is gotten from the menu it belongs to)
  String content;   //Text of the option
  int pos;      //Defines the position it has in the menu
  int textSpacing;   //According to the height, defines the space for the text, so that it's vertically centered
  bool fill = false;  //In case an option is not selected this should be false
  bool disp = false;   //In case an option is not configured, it should be false and, thus hidden
  int destinationType;   //Defines what the option leads to (another menu, graph, something else)
  int destinationIndex;  //Defines where the option leads to (index of the destination)

  public:

  //Option(){}
  
  //Method to configure an option, all attributes are assigned, and disp is true, so the option can be displayed
  void configure(String content, int sizex, int sizey, int pos, int destinationType, int destinationIndex){
    this->sizex = sizex;
    this->sizey = sizey;
    this->content = content;
    this->pos = pos;
    this->destinationType = destinationType;
    this->destinationIndex = destinationIndex;
    this->disp = true;
    this->textSpacing = ((sizey - 7)/2) + 7;
  }

  int getDestinationType(){
    int destinationType = this->destinationType;
    return destinationType;
  }

  int getDestinationIndex(){
    int destinationIndex = this->destinationIndex;
    return destinationIndex;
  }

//This method draws each option

  void drawopt(int page, int pos, int optPPage){
    if(this->disp){    //Checks if the option was configured and, as a result, is displayable
      if(this->pos == pos){   //If the position of the option corresponds to the position passed to the function, then it should be selected
        display.fillRect(0, (this->sizey)*(this->pos) + 1 - (page*optPPage*this->sizey), this->sizex, this->sizey, WHITE);
        display.setTextColor(SSD1306_BLACK);
        display.setCursor(5, (this->sizey)*(this->pos + 1) - (page*optPPage*this->sizey) - this->textSpacing);
        display.print(this->content);
        display.setTextColor(SSD1306_WHITE);
      }
      else{                  //If the option is not selected, the rectangle containing it shouldn't be filled
        display.drawRect(0, (this->sizey)*(this->pos) + 1 - (page*optPPage*this->sizey), this->sizex, this->sizey, WHITE);
        display.setCursor(5, (this->sizey)*(this->pos + 1) - (page*optPPage*this->sizey) - this->textSpacing);
        display.print(this->content);
      }
    }
  }
};
