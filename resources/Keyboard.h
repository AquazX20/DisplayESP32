class Keyboard{
  private:

  byte goTo;
  byte goBack;
  byte plus;
  byte minus;
  byte debounceTime;

  Screen *screen;

  public:

//Keyboard constructor
  Keyboard(byte goTo, byte goBack, byte plus, byte minus, byte debounceTime, Screen * screen){
    this->goTo = goTo;
    this->goBack = goBack;
    this->plus = plus;
    this->minus = minus;
    this->debounceTime = debounceTime;

    this->screen = screen;

    pinMode(goTo, INPUT_PULLUP);
    pinMode(goBack, INPUT_PULLUP);
    pinMode(plus, INPUT_PULLUP);
    pinMode(minus, INPUT_PULLUP);
  }
//Debouncing functions
  void checkGoTo(){
    static char cont;
    if(digitalRead(this->goTo) == LOW)
      cont++;
    else
      cont = 0;
    if(cont == debounceTime/REFRESH){
      this->screen->goTo();
    }
  }
  
  void checkGoBack(){
    static char cont;
    if(digitalRead(this->goBack) == LOW){
      cont++;
    }
    else
      cont = 0;
    if(cont == debounceTime/REFRESH){
      this->screen->goBack();
    }
  }

  void checkPlus(){
    static char cont;
    if(digitalRead(this->plus) == LOW)
      cont++;
    else
      cont = 0;
    if(cont == debounceTime/REFRESH){
      this->screen->plusAction();
    }
  }

  void checkMinus(){
    static char cont;
    if(digitalRead(this->minus) == LOW)
      cont++;
    else
      cont = 0;
    if(cont == debounceTime/REFRESH){
      this->screen->minusAction();
    }
  }

// All buttons are checked with this method
  void control(){
    this->checkGoTo();
    this->checkGoBack();
    this->checkPlus();
    this->checkMinus();
  }
};
