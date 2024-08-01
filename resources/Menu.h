class Menu{    //ContentType (0)
  private:

  int sizex;                  //X size for each option in the menu
  int sizey;                  //Y size of each option in the menu
  int options = 0;            //This indicates the number of options created
  int pos = 0;                //This indicates the position of the cursor
  int page = 0;               //If the menu is too long, this indicates the page that is being displayed
  Option opt[MAX_OPTIONS];
  int optPPage;

  int previousScreen = 0;
  int previousContentType = 0;
  
  public:

  void configure(int sizex, int sizey){   //This method configures the menu created from Screen
    this->sizex = sizex;
    this->sizey = sizey;
    this->optPPage = DISP_HEIGHT / this->sizey;
  }

  //The following method is used to created an option for the menu
  void createOption(String content, int destinationType, int destinationIndex){
    //The option takes the place in the array defined by the options number variable (options), which is later increased.
    this->opt[this->options].configure(content, this->sizex, this->sizey, this->options++, destinationType, destinationIndex);
  }

  int extractDestinationType(){
    int destinationType = this->opt[this->pos].getDestinationType();
    return destinationType;
  }

  int extractDestinationIndex(){
    int destinationIndex = this->opt[this->pos].getDestinationIndex();
    return destinationIndex;
  }

//The following method draws the whole menu by drawing every option configured within it
  void drawMenu(){
    display.clearDisplay();
    this->page = pos/this->optPPage;  //The current page is obtained by dividing the position by the number of options per page (only integer)
    for(int i = 0; i < options; i++){
      this->opt[i].drawopt(this->page, this->pos, this->optPPage);
    }
    display.display();
  }
  //Methods used by Screen
  int extractPos(){ //Gets the current position of the cursor
    return(this->pos);
  }

  int extractOptNumber(){  //Gets the number of options in the menu
    return(this->options);
  }

  void increasePos(){  //Increases the position of the cursor
    this->pos++;
  }

  void decreasePos(){   //Decreases the position of the cursor
    this->pos--;
  }

//Both of the following methods store the values of the previous screen passed as parameters by Screen

  void setPreviousScreen(int prev){
    this->previousScreen = prev;
  }

  void setPreviousContentType(int prev){
    this->previousContentType = prev;
  }

//Both of the following methods retrieve the values of the screen previous to the menu containing these data.
  int getPreviousScreen(){
    int prev = this->previousScreen;
    return prev;
  }

  int getPreviousContentType(){
    int prev = this->previousContentType;
    return prev;
  }
};
