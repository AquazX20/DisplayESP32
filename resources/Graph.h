class Graph{     //ContentType (1)
  private:

  String title;
  char graphType;       //'a' Vertical Bar, 'b' Horizontal Bar, 'c' Cartesian Graph

  //Assign whatever value in "configure(..." if a parameter is not required for the specified graphType
  
  double value;         //For: Vertical Bar  Horizontal Bar  Cartesian
  double xpos;          //For: Vertical Bar  Horizontal Bar  Cartesian
  double ypos;          //For: Vertical Bar  Horizontal Bar  Cartesian
  double height;        //For: Vertical Bar  Horizontal Bar  Cartesian
  double width;         //For: Vertical Bar  Horizontal Bar  Cartesian
  double yminimum;      //For: Vertical Bar                  Cartesian
  double ymaximum;      //For: Vertical Bar                  Cartesian
  double xminimum;      //For:               Horizontal Bar  Cartesian
  double xmaximum;      //For:               Horizontal Bar  Cartesian
  double yStepSize;     //For: Vertical Bar                  Cartesian
  double xStepSize;     //For:               Horizontal Bar  Cartesian
  int digit;            //For: Vertical Bar  Horizontal Bar  Cartesian
  int * maximum;        //For:                               Cartesian
  double x;
  double yrange;
  double xrange;
  double ox;
  double oy;
  double count;
  double graphScale;
  bool redraw = true;
  
  int previousScreen = 0;
  int previousContentType = 0;
  
  public:

//This method configures the graph created, defines its parameters according the type of graph selected.
  void configure(String title, char graphType, double xpos, double ypos, double width, double height,
  double yminimum, double ymaximum, double xminimum, double xmaximum, double yStepSize, double xStepSize, int digit, void *maximum){
    this->title = title;
    this->graphType = graphType;
    this->yminimum = yminimum;
    this->ymaximum = ymaximum;
    this->xminimum = xminimum;
    this->count = xminimum;
    this->xmaximum = xmaximum;
    this->height = height;
    this->width = width;
    this->yStepSize = yStepSize;
    this->xStepSize = xStepSize;
    this->digit = digit;
    this->xpos = xpos;
    this->ypos = ypos;
    this->maximum = (int *) maximum;
    switch(graphType){
      case 'a':
      this->yrange = ymaximum - yminimum;
      this->graphScale = (yStepSize) * (height / this->yrange) - .001; //Adjusts the scale of the graph, according to the range and the size of the step
      break;

      case 'b':
      this->xrange = xmaximum - xminimum;
      this->graphScale = (xStepSize) * (width / this->xrange) - .001; //Adjusts the scale of the graph, according to the range and the size of the step
      break;

      case 'c':
      this->yrange = ymaximum - yminimum;
      break;
    }
  }

  void drawGraph(){
    double level, data, i;
    switch(graphType){
      case 'a':
      double my;
  
      if (this->redraw) {  //Prints the labels
        display.clearDisplay();
        this->redraw = false;
        display.fillRect(0, 0,  127 , 14, SSD1306_WHITE);
        display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
        display.setTextSize(1);
        display.setCursor(2, 4);
        display.println(this->title);
  
        for (i = 0; i <= this->height; i += this->graphScale) {
          my =  this->ypos - this->height + i;
          display.drawFastHLine(this->xpos + this->width + 1, my,  5, SSD1306_WHITE);
          // draw lables
          display.setTextSize(1);
          display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
          display.setCursor(this->xpos + this->width + 12, my - 3 );
          data = this->ymaximum - ( i * (this->yStepSize / this->graphScale));
          display.print(data, this->digit);
        }
      }
      // compute level of bar graph that is scaled to the  height and the hi and low vals
      // this is needed to accompdate for +/- range
      level = (this->height * (((this->value - this->yminimum) / (this->yrange))));
      // draw the bar graph
      // write a upper and lower bar to minimize flicker cause by blanking out bar and redraw on update
      display.drawRect(this->xpos, this->ypos - this->height, this->width, this->height, SSD1306_WHITE);
      display.fillRect(this->xpos, this->ypos - this->height, this->width, this->height - level,  SSD1306_BLACK);
      display.drawRect(this->xpos, this->ypos - this->height, this->width, this->height, SSD1306_WHITE);
      display.fillRect(this->xpos, this->ypos - level, this->width,  level, SSD1306_WHITE);
      // up until now print sends data to a video buffer NOT the screen
      // this call sends the data to the screen
      display.display();
      break;

      case 'b':

      if (this->redraw) {
        display.clearDisplay();
        this->redraw = false;
        display.fillRect(0, 0,  127 , 16, SSD1306_WHITE);
        display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
        display.setTextSize(1);
        display.setCursor(2, 4);
        display.println(this->title);
        // draw the text
        
        for (i = 0; i <= this->width; i += this->graphScale) {
          display.drawFastVLine(i + this->xpos , this->ypos ,  5, SSD1306_WHITE);
          // draw lables
          display.setTextSize(1);
          display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
          display.setCursor(i + this->xpos , this->ypos + 10);
          // addling a small value to eliminate round off errors
          // this val may need to be adjusted
          data =  ( i * (this->xStepSize / this->graphScale)) + this->xminimum + 0.00001;
          display.print(data, this->digit);
        }
      }
      // compute level of bar graph that is scaled to the width and the hi and low vals
      // this is needed to accompdate for +/- range capability
      // draw the bar graph
      // write a upper and lower bar to minimize flicker cause by blanking out bar and redraw on update
      level = (this->width * (((this->value - this->xminimum) / (this->xmaximum - this->xminimum))));
      display.fillRect(this->xpos + level, this->ypos - this->height, this->width - level, this->height,  SSD1306_BLACK);
      display.drawRect(this->xpos, this->ypos - this->height, this->width,  this->height, SSD1306_WHITE);
      display.fillRect(this->xpos, this->ypos - this->height, level,  this->height, SSD1306_WHITE);
      // up until now print sends data to a video buffer NOT the screen
      // this call sends the data to the screen
      display.display();
      break;

      case 'c':
      double temp;
    
      if (this->redraw == true) {
        this->redraw = false;
        this->xrange = *this->maximum - xminimum;
        display.clearDisplay();
        display.fillRect(0, 0,  127 , 16, SSD1306_WHITE);
        display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
        display.setTextSize(1);
        display.setCursor(2, 4);
        display.println(title);
        this->ox = (this->count - this->xminimum) * (this->width) / (this->xrange) + this->xpos;
        this->oy = (this->value - this->yminimum) * (- this->height) / (this->yrange) + this->ypos;
        // draw y scale
        display.setTextSize(1);
        display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
        for ( i = this->yminimum; i <= this->ymaximum; i += this->yStepSize) {
          // compute the transform
          temp =  (i - this->yminimum) * (- this->height) / (this->ymaximum - this->yminimum) + this->ypos;
          if (i == 0) {
            display.drawFastHLine(this->xpos - 3, temp, this->width + 3, SSD1306_WHITE);
          }
          else {
            display.drawFastHLine(this->xpos - 3, temp, 3, SSD1306_WHITE);
          }
          display.setCursor(this->xpos - 27, temp - 3);
          display.println(i, this->digit);
        }
        // draw x scale
        for (i = this->xminimum; i <= *this->maximum; i += this->xStepSize) {
          // compute the transform
          display.setTextSize(1);
          display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
          temp =  (i - this->xminimum) * (this->width) / (this->xrange) + this->xpos;
          if (i == 0) {
            display.drawFastVLine(temp, this->ypos - this->height, this->height + 3, SSD1306_WHITE);
          }
          else {
            display.drawFastVLine(temp, this->ypos, 3, SSD1306_WHITE);
          }
          display.setCursor(temp, this->ypos + 6);
          display.println(i, this->digit);
        }
      }
    
      // graph drawn now plot the data
      // the entire plotting code are these few lines...
    
      this->x =  (this->count - this->xminimum) * (this->width) / (this->xrange) + this->xpos;
      this->value =  (this->value - this->yminimum) * (- this->height) / (this->yrange) + this->ypos;
      display.drawLine(this->ox, this->oy, this->x, this->value, SSD1306_WHITE);
      display.drawLine(this->ox, this->oy - 1, this->x, this->value - 1, SSD1306_WHITE);
      this->ox = this->x;
      this->oy = this->value;
    
      // up until now print sends data to a video buffer NOT the screen
      // this call sends the data to the screen
      display.display();
      this->count += 1;
      if(this->ox >= (this->xpos + this->width)){
        this->redraw = true;
        this->count = xminimum;
      }
    }
  }

  void redrawFlag(){ // Activates the redraw bool to get the graph printed correctly
    this->redraw = true;
    this->count = xminimum;
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

  void assignValue(double value){
    this->value = value;
  }

  void reset(){
    this->x = 0;
  }
  
};
