class Screen{
  private:
  
  Menu menu[MAX_MENUS];  //Array of menus to use
  Graph graph[MAX_GRAPHS];  //Array of graphs to use
  Modifier modifier[MAX_MODIFIERS];  //Array of modifiers to use

  int counterM = 0;        //Number of menus created
  int counterG = 0;        //Number of graphs created
  int counterMod = 0;      //Number of modifiers created
  bool redraw = true;      //Redraw interface for when there is a change of screen
  int currentScreen = 0;
  int contentType = 0;

  public:


  void configure(bool fullsetting, char address){  //This method allows the configuration of the display when the parameter is true. Otherwise only prints a greeting message.
    if(fullsetting){
      //Adafruit_SSD1306 display(DISP_WIDTH, DISP_HEIGHT, &Wire, -1);
      Serial.begin(115200);
      if (!display.begin(SSD1306_SWITCHCAPVCC, address)) {
  #ifdef __DEBUG__
        Serial.println("Display not found!");
  #endif
        while (true);
      }
    }
    display.clearDisplay();
  
    // Text size
    display.setTextSize(2);
    // Text color
    display.setTextColor(SSD1306_WHITE);
    // Text position
  
    display.setCursor(25, 20);
    display.println("Welcome");
    
    
    display.setTextSize(1);
  
    display.display();
    delay(5000);
  }

  void createMenu(int sizex, int sizey){   //This method is used for the creation of a menu
    this->menu[counterM].configure(sizex, sizey);
    this->counterM++;
  }

  void createOption(int menuIndex, String content, int destinationType, int destinationIndex){  //this method should be used for creating an option in a menu
    this->menu[menuIndex].createOption(content, destinationType, destinationIndex);
  }

  void createVGraph(String title, double xpos, double ypos, double width, double height,
  double yminimum, double ymaximum, double yStepSize, int digit){   //this method calls the configure() of graph for a vertical graph
    this->graph[counterG].configure(title, 'a', xpos, ypos, width, height, yminimum, ymaximum, 0, 0, yStepSize, 0, digit, 0);
    this->counterG++;
  }

  void createHGraph(String title, double xpos, double ypos, double width, double height, 
  double xminimum, double xmaximum, double xStepSize, int digit){  //this method calls the configure() of graph for a horizontal graph
    this->graph[counterG].configure(title, 'b', xpos, ypos, width, height, 0, 0, xminimum, xmaximum, 0, xStepSize, digit, 0);
    counterG++;
  }

  void createCGraph(String title, double xpos, double ypos, double width, double height,
  double yminimum, double ymaximum, double xminimum, double yStepSize, double xStepSize, int digit, void * maximum){  //this method calls the configure() of graph for a cartesian chart
    this->graph[counterG].configure(title, 'c', xpos, ypos, width, height, yminimum, ymaximum, xminimum, 0, yStepSize, xStepSize, digit, maximum);
    counterG++;
  }

  void createModifier(String title, int *value, int max, int min, int step){   //This method is used for the creation of a menu
    this->modifier[counterMod].configure(title, value, max, min, step);
    this->counterMod++;
  }

/*  
  void redrawFlag(){
    this->redraw = true;
  }
*/

//The following method is used for assingning a value to a graph
//This can be avoided using pointers to the variable to plot in the graph
  void graphAssignValue(int graphIndex, double value){
    this->graph[graphIndex].assignValue(value);
    if(this->currentScreen == graphIndex && this->contentType == 1)
    this->redraw = true;
  }

//This method controls the whole interface, it needs to be called within a loop
  void control(){
    if (redraw){
      if (contentType == 0){
        menu[currentScreen].drawMenu();
      }
      else if (contentType == 1){
        graph[currentScreen].drawGraph();
      }
      else if (contentType == 2){
        modifier[currentScreen].drawModifier();
      }
      this->redraw = false;
    }
  }

//The following two methods allow the change in position of the cursor
  void increasePos(){
    if(this->menu[this->currentScreen].extractPos() < this->menu[this->currentScreen].extractOptNumber() - 1)
      this->menu[this->currentScreen].increasePos();
  }

  void decreasePos(){
    if(this->menu[this->currentScreen].extractPos() > 0)
      this->menu[this->currentScreen].decreasePos();
  }
  
//This method lets the user go into another screen by selecting an option
  void goTo(){
    if(this->contentType == 0){
      int newScreen = this->menu[this->currentScreen].extractDestinationIndex();
      int newContentType = this->menu[this->currentScreen].extractDestinationType();
      if (newContentType == 0){
        this->menu[newScreen].setPreviousScreen(this->currentScreen);
        this->menu[newScreen].setPreviousContentType(this->contentType);
      }
      else if(newContentType == 1){
        this->graph[newScreen].setPreviousScreen(this->currentScreen);
        this->graph[newScreen].setPreviousContentType(this->contentType);
        this->graph[newScreen].reset();
        this->graph[newScreen].redrawFlag();
      }
      else if(newContentType == 2){
        this->modifier[newScreen].setPreviousScreen(this->currentScreen);
        this->modifier[newScreen].setPreviousContentType(this->contentType);
      }
      this->contentType = newContentType;
      this->currentScreen = newScreen;
      this->redraw = true;
    }
  }

  void goBack(){
    if(contentType == 0){
      //Gets indexes from previous screen saved in actual screen if it is a menu, and sets them as the current indexes
      this->currentScreen = this->menu[this->currentScreen].getPreviousScreen();
      this->contentType = this->menu[this->currentScreen].getPreviousContentType();
    }
    else if(contentType == 1){
      //Gets indexes from previous screen saved in actual screen if it is a graph, and sets them as the current indexes
      this->currentScreen = this->graph[this->currentScreen].getPreviousScreen();
      this->contentType = this->graph[this->currentScreen].getPreviousContentType();
    }
    else if(contentType == 2){
      this->currentScreen = this->modifier[this->currentScreen].getPreviousScreen();
      this->contentType = this->modifier[this->currentScreen].getPreviousContentType();
    }
    this->redraw = true;
  }

//These methods control the plus and minus button actions
  void plusAction(){
    if(contentType == 0){
      increasePos();
    }
    else if(contentType == 2){
      this->modifier[currentScreen].increaseValue();
    }
    this->redraw = true;
  }

  void minusAction(){
    if(contentType == 0){
      decreasePos();
    }
    else if(contentType == 2){
      this->modifier[currentScreen].decreaseValue();
    }
    this->redraw = true;
  }
  
  int getCurrentScreen(){
	  return this->currentScreen;
  }
  
  int getContentType(){
	  return this->contentType;
  }
  
};
