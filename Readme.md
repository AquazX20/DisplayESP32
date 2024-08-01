# Basic Instructions of Repository DisplayESP32

**This repository aims to provide some functions for creating a user interface for the display 
Adafruit SSD1306 on microcontroller ESP32.**  
For such, new classes are implemented, which are included in the *.ino* code included in this repo.  

**In order to run the example codes, it's important to include the DisplayESP32.ino file and the resources folder (where each class in this library is defined) into the project folder**


## To begin with

To be able to use this repository, it's important to first connect the display to the 
microcontroller correctly. The following diagram shows how it should be done when using an ESP32-WROOM board.  
![Connection diagram used for DisplayESP32 repository](http://gmarxcc.com:8088/MSP430/DisplayESP32/raw/commit/b68980865195c06e96173dc9e7beeacc0f3e3a5f/Connection%20Diagram_UI.png "Connection diagram")

It is possible to see that the display used in this project uses I2C protocol, thus only two pins are needed to allow 
communication with the microcontroller.  
On the other hand, the keyboard uses one pin for each button (configured as Pull up), and a connection to Ground.

## Macros

Macros are defined to help use the library easily, making possible to modify the display specs in an easy way and also, the behaviour of the interface, by just changing the values defined.

    #define MAX_OPTIONS 10      //Maximum number of options for each menu
    #define MAX_MENUS 3
    #define MAX_GRAPHS 3
    #define MAX_MODIFIERS 3
    #define DISP_WIDTH 128      // OLED display width
    #define DISP_HEIGHT 64      // OLED display height
    #define REFRESH 10          //Refresh time in ms
    #define ADDRESS 0x3C            //I2C address of the display

* **MAX_OPTIONS** defines the maximum number of options a menu can hold, in this case, there can only be three menus in total, as there is only enough memory reserved for them.
* **MAX_MENUS** declares the maximum number of menu screens in the interface, it's not possible to create more menus than this number.
* **MAX_GRAPHS** is the maximum number of graphs to create.
* **MAX_MODIFIERS** is the maximum number of modifiers to create.
* **DISP_WIDTH** and **DISP_HEIGHT** are hardware specific (SSD1306), it's possible to modify this values in the case that it's required to employ a different kind of display.
* **REFRESH** is the time in milliseconds the interface will take in refreshing (this time affects the loop, keep that in mind)
* **ADDRESS** address of the display


## Content types

This library depends on some indexes to display content, that's why it's important to keep in mind that all screens are described by two parameters: Content Type and Content Index. Indexes are assigned according to the order in which the elements were created.  
  
On the other hand, type is defined according to the following.

1. Menu
2. Graph
3. Setting

Those integer values are recognized by screen and lead to the different content screens configured.

## Classes

In this section, the created classes are discussed, to better understand how each of them works.  
  
Before getting into the next classes, it's necessary to note that most methods in them aren't supposed to be accessed directly, there are methods in the classes Screen and Keyboard that use them to control the interface through their own methods, which end up calling those in each class but also control the behaviour of the display at the same time, which allows for less code lines when using the library to program an interface.  
Despite the above, the methods in each class will be presented to better know how they work.

### Option class

This class is implemented to create options that will be later used to conform a menu.

#### Attributes

The attributes of this class are:  

    private:

    int sizex;
    int sizey;
    String content;
    int pos;
    int textSpacing;   //According to the height, defines the space for the text, so that it's vertically centered
    bool fill = false;  //In case an option is not selected this should be false
    bool disp = false;   //In case an option is not configured, it should be false and, thus hidden
    int destinationType;   //Defines what the option leads to (another menu, graph, something else)
    int destinationIndex;  //Defines where the option leads to (index of the destination)

* **sizex**: Defines the size the option will occupy in the x axis (width), this value is gotten from the menu. Menu class has an attribute that defines the size it will have in the x axis.
* **sizey**: Defines the height of the option (this value is gotten from the menu it belongs to).
* **content**: This string corresponds to the caption the option will display once created.
* **pos**: Integer that defines the position it has in the menu.
* **destinationType**: Used to know what type of screen the option leads to (menu, graph, etc...).
* **destinationIndex**: Used to know what index the destination screen has, leading only to that screen.

**It is important to note that indexes are assigned according to the order the screen was created, if it is the first screen created, then its index is *0*.**

#### Methods

The following are the methods used to interact with each option created.

##### configure()

This method assigns the values to the option created, according to the way it's containing menu is defined and the parameters we want for such option as is its destination.
This method shouldn't be called on its own, because class Screen already has one method to create options and assign them to a specific menu. The prototype for this method is:

    void configure(String content, int sizex, int sizey, int pos, int destinationType, int destinationIndex)

##### getDestinationType() and getDestinationIndex()

Methods accessed by others from Screen class. These return and integer corresponding to the type and index of the screen they lead to. It's not needed to call this methods directly, there are functions that do that and update the screen at the same time.

##### drawOpt()

With this method, an option is drawn. This is another method that shouldn't be called directly, as there are other methods in the next classes that call it for all the options in a menu.  
This function requires the parameters shown in its prototype:

    void drawOpt(int page, int pos, int optPPage)

These parameters are obtained automatically from the method drawMenu in Menu that draws all the options within it. In this method, if the option is selected, then it will be printed in a filled rectangle, otherwise, it will appear as a black rectangle.

### Menu class

This class is used to create menus and keep their attributes stored throughout the use of the interface.

#### Attributes

The following attributes belong to this class:

    private:

    int sizex;
    int sizey;                  //Y size of each option in the menu
    int options = 0;            //This indicates the number of options created
    int pos = 0;                //This indicates the position of the cursor
    int page = 0;               //If the menu is too long, this indicates the page that is being displayed
    Option opt[MAX_OPTIONS];
    int optPPage;

    int previousScreen = 0;
    int previousContentType = 0;

Where:

* **sizex**: Defines the width of the options, it's preferable to leave this option as DISP_WIDTH or 128, since it will cover the whole display width.
* **sizey**: Defines the height of each option in this menu, a value of 13 grants a good visualization of each option, greater values improve spacing, reducing the number of options per page though.
* **options**: This acts as a counter that stores the number of options created in the menu.
* **pos**: This variable stores the position of the menu in which the user is at a certain time.
* **page**: According to the number of options per page (optPPage), this variable holds the page to be printed in larger menus.
* **Option opt[MAX_OPTIONS]**: This is an array that holds every option created in the menu (with a maximum of MAX_OPTIONS).
* **previousScreen** and **previousContentType**: These integers keep the values of the screen that led to this menu, to make it possible to return to that screen. This values are assigned from a method in Menu.h, called from another one in Screen.h, which is used to control the interaction with the interface. This will be discussed later in this document, see Screen class.

#### Methods

Now, we'll talk about the methods included in this class.

##### configure()

To create a menu, it's important to configure it first. That's where this method is applied, though not directly, since there is another method in Screen that calls for this method and creates a menu for the interface. The prototype for this method is as follows:

    void configure(int sizex, int sizey)

From the height of each option, it is possible to determine the number of options by each page of the menu. As a result, in this method, optPPage is assigned too.

##### createOption

This method is in charge of adding options to the menu. It recurs to the method configure in Option and assigns the values for the size, position and destination. The prototype is the shown next:

    void createOption(String content, bool destinationTypeMenu, int destinationIndex)

The parameters are received from the method in Screen to create an option for a menu.  
As we can see in the next line, the option receives a position according to the number of options currently in the menu, then the variable is increased because there is one more option.

    this->opt[this->options].configure(content, this->sizex, this->sizey, this->options++, destinationTypeMenu, destinationIndex)

Furthermore, the option is assinged to a place in the array of options.

##### extractDestinationType() and extractDestinationIndex()

Both methods are used to get the integers corresponding to the destination of the option selected, which serve to interact with the interface, allowing the user to enter an option of the menu and change from screen to screen.

##### drawMenu()

**drawMenu()** is used to draw all the options in the menu. It draws only the options that have been configured, thus ignores those within the array **opt** that haven't been used. To draw the options, it is important to provide the parameters for the drawOpt() method in Option, which are page, pos and optPPage, so first we get the page by getting the options per page, optPPage is calculated according to **sizey**, defined at the creation of the menu (this calculation takes into account the macro DISP_HEIGHT, discussed previously), pos is the attribute of the menu, which stores the current position of the cursor (option to display as selected by user). All the above is shown in the next lines:

    void drawMenu(){
        display.clearDisplay();
        this->page = pos/this->optPPage;
        for(int i = 0; i < options; i++){
        this->opt[i].drawopt(this->page, this->pos, this->optPPage);
        }
        display.display();
    }

In configure():

    this->optPPage = DISP_HEIGHT / this->sizey;


##### extractPos() and extractOptNumber()

These methods are only used to extract values from the attributes of the objects of Menu. These data will be useful for knowing where the cursor is in regards to the number of options in the menu. That way, it's possible to prevent the cursor from moving beyond the options in the menu. These will be used in another method from Screen.

##### increasePos() and decreasePos()

These methods are accessed by Screen in order to increase or decrease the position of the cursor, since Screen serves as the controller of the whole interface, and keyboard interacts directly with its methods.

##### Previous screen storing

In order to store the previous screen, it's necessary to store the values that lead to it, both, index and type of content, thus four methods are implemented for that, two of them retrieve the values of the actual screen before changing to the new menu, the other two methods are called to store them in the latter, modifying the attributes **previousScreen** and **previousContentType**.  

**setPreviousScreen(int previousIndex)** and **setPreviousScreenType(int previousScreenType)** store the values of the previous screen passed from a Screen object, making it possible to go to the next menu and update the current Screen attributes, without losing the data of the previous screen.  

**getPreviousScreen()** and **getPreviousScreenType()** retrieve the data from **previousScreen** and **previousScreenType**, allowing the transition to a previous menu.

### Graph class

This class is for the creation of graphs. It's based on Kris Kasprzak's code for drawing graphs on an OLED display using Arduino [Kasprzak K.][1]. It allows the ploting of three types of graphs: vertical bar, horizontal bar and cartesian chart.

#### Attributes

To achieve the above stated the following attributes are used:

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

* **title**: This string allocates the name of the graph to be displayed at the top of the display.
* **graphType**: The type of graph created 'a' means a vertical graph, 'b' a horizontal graph and 'c' a cartesian chart.
* **value**: It's the value to pass to the graph, there is method for that. This attribute is used by all types of graphs.
* **xpos**: This is the position in x of the bottom left corner of the graph. Required by all graph types.
* **ypos**: This is the position in y of the bottom left corner of the graph. Required by all graph types.
* **height**: Height of the graph (pixels).
* **width**: Width of the graph(pixels).
* **yminimum**: This is the minimum value to be graphed in the y axis. Required only for vertical graph and cartesian chart.
* **ymaximum**: Maximum value to graph in the y axis. Required only for vertical graph and cartesian chart.
* **xminimum**: This is the minimum value to be graphed in the x axis. Required only for horizontal graph and cartesian chart.
* **xmaximum**: Maximum value to graph in the x axis. Required only for horizontal graph and cartesian chart.
* **yStepSize**: Size of the step in the y axis. This is the interval in which the axis is going to be split. Required only for vertical graph and cartesian chart.
* **xStepSize**: Size of the step in the x axis. This is the interval in which the axis is going to be split. Required only for horizontal graph and cartesian chart.
* **digit**: Number of decimal digits to display in the axis labels.
* ***maximum**: Maximum number to display in the x axis of a cartesian graph (it can be the number of samples to take).
* **x**: This is used to know the ending point of a line in the cartesian chart.
* **yrange**: Range in the y axis, depends on the maximum and minimum of this axis.
* **xrange**: Range in the x axis, depends on the maximum and minimum of this axis.
* **ox**: Starting point in x of a line to plot in the cartesian chart (Previous *x* value).
* **oy**: Starting point in y of a line to plot in the cartesian chart (Previous value received).
* **count**: Last iteration of the cartesian chart, in the x axis.
* **graphScale**: Scale of the graph (vertical or horizontal), according to its minimum and maximum.
* **redraw**: This boolean should only be true then the whole screen was cleared. It redraws the axes or the bars respectively.
* **previousScreen** and **previousContentType**: These integers keep the values of the screen that led to this graph, to make it possible to return to that screen. This values are assigned from a method in Graph.h, called from another one in Screen.h, which is used to control the interaction with the interface. This will be discussed later in this document, see Screen class.

#### Methods
The following methods are applied to manage data from Graph.

##### configure()

This method sets up the new graph, its main attributes are defined here. The parameters passed in this case include all the attributes required for any kind of graph, however, as stated in some other methods, this one should not be called, there are some methods in Screen that call for this method and only require the parameters necessary for a certain type of graph (**createVGraph()**, **createHGraph()**, **createCGraph()**), these will be explained later.  

The prototype is:

    void configure(String title, char graphType, double xpos, double ypos, double width, double height, double yminimum, double ymaximum, double xminimum, double xmaximum, double yStepSize, double xStepSize, int digit)

According to the type of graph configured, the next calculations are made:

    switch(graphType){
        case 'a':
        this->yrange = ymaximum - yminimum;
        this->graphScale = (yStepSize) * (height / this->yrange) - .001;
        break;

        case 'b':
        this->xrange = xmaximum - xminimum;
        this->graphScale = (xStepSize) * (width / this->xrange) - .001;
        break;

        case 'c':
        this->yrange = ymaximum - yminimum;
        break;

##### drawGraph()

To draw the graph and plot the values, **drawGraph()** is the method in graph applied. This method is called automatically from Screen when delay(REFRESH) expires. There is a swith case block that allows to print all types of graphs within this function, recognizing them by their **graphType** attribute.

###### Vertical graph (graphType == 'a')

For vertical bar graphs, the following code block is implemented for the creation of the labels:

    if (this->redraw) {
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
            display.setTextSize(1);
            display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
            display.setCursor(this->xpos + this->width + 12, my - 3 );
            data = this->ymaximum - ( i * (this->yStepSize / this->graphScale));
            display.print(data, this->digit);
            }
        }

To graph the value received, two rectangles are display. A white one fills the portion of the bar occupied by the amount measured. A black rectangle fills the remaining part of the bar, as shown in the next block. This prevents flickering.

    level = (this->height * (((this->value - this->yminimum) / (this->yrange))));

        display.drawRect(this->xpos, this->ypos - this->height, this->width, this->height, SSD1306_WHITE);
        display.fillRect(this->xpos, this->ypos - this->height, this->width, this->height - level,  SSD1306_BLACK);
        display.drawRect(this->xpos, this->ypos - this->height, this->width, this->height, SSD1306_WHITE);
        display.fillRect(this->xpos, this->ypos - level, this->width,  level, SSD1306_WHITE);

###### Horizontal graph (graphType == 'b')

For horizontal graphs, the following code block is implemented to diplay the labels of the graph:

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

Two plot the data, the previous ending point of the graph is stored in **ox** and **oy**, and the new values are calculated depending on the value received and the scale of the graph. When the next point in the graph is greater than its width, the graph is redrawn and count is reset, in order to start graphing anew.

##### redrawFlag()

This method is accessed when necessary to redraw the whole interface of the graph. It only sets **redraw** to true.

##### Previous screen storing

It has the same methods as Menu for this, so it's recommended to read about them in the previous section of this document.

##### assignValue()

This method passes a value as a new attribute of the graph so it can be plotted. It has only one parameter, being it a double, corresponding to the value to receive.

##### reset()

Resets the position of x to redraw the cartesian chart when exiting and entering into it from the menus.

### Modifier class

This class is used to create screens for runtime variable manipulation, without need to rewrite the code and load it into the microcontroller. It has the following attributes.

#### Attributes

* **title**: String containing the title of the screen.
* ***value**: Integer pointer to the value to modify.
* **max**: Highest possible value to set.
* **min**: Lowest possible value to set.
* **step**: Size of the step.
* **previousScreen** and **previousContentType**: These integers keep the values of the screen that led to this modifier, to make it possible to return to that screen. This values are assigned from a method in Modifier.h, called from another one in Screen.h, which is used to control the interaction with the interface. This will be discussed later in this document, see Screen class.

#### Methods

##### configure()

This function configures the modifier screen after being created. Its prototype is:

    void configure(String title, int *value, int max, int min, int step)

##### drawModifier()

This function allows the drawing of the modifier screen on the OLED display. This function is used by the Screen object controlling the user interface.

##### increaseValue() and decreaseValue()

Both functions are designed to change the values of the variable pointed by the pointer configured within the modifier, dependig on the value of *step*.

### Screen class

This class is the main class of this code, it's responsible for the control of the whole interface, allowing to switch between different menus and graphs, also for receiving data and interacting with the keyboard.

#### Attributes

Screen attributes are listed in the following code and explained afterwards.

    private:

    Menu menu[MAX_MENUS];
    Graph graph[MAX_GRAPHS];
    int counterM = 0;
    int counterG = 0;
    int counterMod = 0; 
    bool redraw = true;
    int currentScreen = 0;
    int contentType = 0;

* **menu**: This array holds all the menus for the interface, limited by **MAX_MENUS**, see Macros.
* **graph**: This array holds all the graphs for the interface, limited by **MAX_GRAPHS**, see Macros.
* **counterM**: This is the number of menus created, it increases when a new menu is configured using the methods available from Screen.
* **counterG**: This is the number of graphs created, it increases when a new graph is configured using the methods available from Screen.
* **counterMod**: This is the number of modifiers created, it increases when a new modifier is configured using the methods available from Screen.
* **redraw**: Variable to control the redrawing of the interface.
* **currentScreen** and **contentType**: Variables where the current screen direction is stored, so that it's the only screen displayed.

#### Methods

To control the behaviour of the interface the following methods are used.

##### configure()

This method is used to set up the display and also print a welcome message, it is necessary to call this method once the object is created, in order to configure the direction of the display and allow communication. This configuration has place only if the parameter in configure is *true*, otherwise the display is only cleared and the welcome message is printed. Its prototype is shown in the following code block.

    void configure(bool fullsetting, char address)

*address* corresponds to the I2C address of the display.

##### createMenu()

This method allows the creation of a new menu. Here is where configure() from Menu is called and the attributes of the object get their values.

    void createMenu(int sizex, int sizey)

##### createOption()

This is the recommended method for the creation of an option for a menu, it requires the index of the menu it belongs to, the destination index and type, and the message to show. Its prototype is:

    void createOption(int menuIndex, String content, bool destinationTypeMenu, int destinationIndex)

##### Graph creation

To create graphs, it is advisable to do it from Screen, using the methods **createVGraph()**, **createHGraph()** and **createCGraph()**. These allow the allocation of the graphs in the array of the Screen object, allowing them to interact with the interface. Their prototypes are:

    void createVGraph(String title, double xpos, double ypos, double width, double height, double yminimum, double ymaximum, double yStepSize, int digit)

    void createHGraph(String title, double xpos, double ypos, double width, double height, double xminimum, double xmaximum, double xStepSize, int digit)

    void createCGraph(String title, double xpos, double ypos, double width, double height, double yminimum, double ymaximum, double xminimum, double yStepSize, double xStepSize, int digit, void * maximum)

##### graphAssignValue()

Using this method, it's possible to assing a value to a graph, specifying its index and the variable with the value to assign. This function should be called everytime that such value needs to be updated.

    void graphAssignValue(int graphIndex, double value)

##### Modifier creation

To create a new modifier screen, the following method must be used:

    void createModifier(String title, int *value, int max, int min, int step)

##### control()

This method is the main method of Screen. It redraws the content and prints the actual screen when needed, all the interface depends on it. This method should be used in a loop in order to keep it running and refreshing the contents. It will be shown later in the Implementation section.

##### increasePos() and decreasePos()

These methods' sole task is to call the methods to increase or decrease the position of the cursor in the current menu displayed. This methods shouldn't be called directly, there is another method that uses them, though they can be used if we are in a menu screen, otherwise they might not work as expected.

##### goTo()

This method changes the screen and allows to enter a new screen if an option in a menu is currently selected, for that, it needs to check the content currently displayed and retrieve the destination indexes and types using the methods from Menu. Furthermore, it is in charge of storing the current screen as a previous screen inside of the screen to be displayed, where it makes use the other methods defined in Menu, Graph, etc. That is done in the following lines:

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

##### goBack()

This method does the opposite to the previous one, it retrieves the data of the previous screen stored in the current screen and sets the former as the current screen in the attributes of the object Screen, that way it allows the user to return to a preceding screen.

##### plusAction() and minusAction()

Depending on the type of screen the user is at, this methods determine whether the options plus and minus increase or decrease positions or modify variables or do something else. This methods are planned to be used from Keyboard.

### Keyboard class

This class permits the usage of buttons to control the interface.

#### Attributes

The attributes used in this class are:

    private:

    byte goTo;
    byte goBack;
    byte plus;
    byte minus;
    byte debounceTime;

    Screen *screen;

Where:

* **goTo**, **goBack**, **plus** and **minus**: This variables hold the pins where these buttons are configured.
* ***screen**: This is a pointer to the object Screen created, giving place to the interaction between both of them.

#### Methods

The methods listed below are defined for Keyboard:

##### Keyboard

This is the constructor for Keyboard, here the buttons are defined and the pointer to the Screen object is selected. The prototype is:

    Keyboard(byte goTo, byte goBack, byte plus, byte minus, byte debounceTime, Screen * screen)

The debounceTime parameter is a number in milliseconds that the buttons are going to be checked by the debouncing methods.

##### Debouncing functions

This group of methods is used to eliminate the effect of physical buttons bouncing, waiting a specified number of millisenconds. This depends on REFRESH, as is shown belown:

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

These methods are **checkGoTo()**, **checkGoBack()**, **checkPlus()** and **checkMinus()**.
##### control()

Finally, the debouncing methods are called in the main method of Keyboard, which is control(), this should run in a loop, to make it work continuously.

### Implementation

#### Simple_Implementation

There are two example implementation codes, the first one (Simple_Implementation), allows for the creation of a simple user interface, with two example menus, three graphs and two modifiers.
The library already includes the creation of the Adafruit_SSD1306 object as display. It can be modified to match other display controllers.

    Adafruit_SSD1306 display(DISP_WIDTH, DISP_HEIGHT, &Wire, -1);

In the example provided, an integer variable *i* is declared to test the graphs. Furthermore, two integer variables are created to test the modifiers.

    int i = 0;
    int multiplier = 1;
    int samples = 1000;

To use the interface, it is important to create a Screen and a Keyboard objects, in order to have full control of the interface, that is done in the lines below:

    Screen screen;
    Keyboard keyboard(13, 12, 14, 27, 30, &screen);

Then, in the setup() function of the program, it's necessary call the configure() method from screen to establish communication, afterwards, the menus and graphs are created. Finally the options in the menus are configured.

    screen.configure(true, 0x3C);
  
    screen.createMenu(128, 13);             //Menu 0
    screen.createMenu(128, 13);             //Menu 1

    screen.createVGraph("Grafica 1", 25, 60, 40, 40, 0, 100, 10, 0);                     //Graph 0
    screen.createHGraph("Grafica 2", 10, 40, 100, 20, 0, 100, 20, 0);                    //Graph 1
    screen.createCGraph("Grafica 3", 30, 50, 75, 30, 0, 100, 0, 25, 250, 0, &samples);   //Graph 2

    screen.createModifier("Modify variable", &multiplier, 5, 1, 1);
    
    screen.createOption(0, "Vertical graph", 1, 0);
    //Creates the first option in Menu 0, directing to a graph (contentType = 1 (Graph)), 0 (Graph 0)
    screen.createOption(0, "Horizontal graph", 1, 1);       
    screen.createOption(0, "Cartesian graph", 1, 2);
    screen.createOption(0, "Extra option", 0, 1);

    screen.createOption(1, "Test", 1, 3);
    screen.createOption(1, "Working?", 2, 2);
    screen.createOption(1, "Modify variable", 2, 0);

There are some lines commented below which can be used to test the methods from Screen, to manipulate the interface without a keyboard. These can methods can be tested with a keyboard, if configured.

    //  screen.increasePos();
    //  screen.increasePos();
    //  screen.goTo();
    //  screen.graphAssignValue(2, 50);
    //  screen.goBack();
    //  screen.increasePos();
    //  screen.goTo();
    //  screen.goBack();
    //  screen.decreasePos();

In the loop() function, we find the main methods of Screen and Keyboard, which need to be run continuously. And then, we also manipulate *i* to change its value and be able to plot a sawtooth signal. This value is passed to graph 1 and graph 2.

    screen.control();            //Controls the screen and redraws if needed
    keyboard.control();

    if(i <= 100){
        screen.graphAssignValue(1, i);    //Assigning a demo value to Graph 1
        screen.graphAssignValue(2, i);    //Assigning a demo value to Graph 2
        i++;
    }
    else
    i = 0;

    delay(REFRESH);                   //Refresh time (approx)

The last delay is the time at which the display will refresh.

#### Singleshot_Graph

This example code allows for the creation of a graph that runs once. It contains a menu that enables the user to modify some parameters of the system before running the graph. The keyboard gets blocked until completing the graph.

In the example provided, an integer variable *i* is declared to test the graphs. Furthermore, two integer variables are created to test the modifiers.

    int i = 0;
    int multiplier = 1;
    int samples = 1000;

To use the interface, it is important to create a Screen and a Keyboard objects, in order to have full control of the interface, that is done in the lines below:

    Screen screen;
    Keyboard keyboard(13, 12, 14, 27, 30, &screen);

In the setup() function, the screen is configured, as is a menu with three options too, and two modifiers.

    screen.configure(true, 0x3C);
    
    screen.createMenu(128, 13);             //Menu 0
    
    screen.createCGraph("Test", 30, 50, 75, 30, 0, 100, 0, 25, 250, 0, &samples);   //Graph 0

    screen.createModifier("Multiplier", &multiplier, 5, 1, 1);  //Modifier 0
    screen.createModifier("Samples number", &samples, 1000, 500, 10);   //Modifier 1
    
    screen.createOption(0, "Adjust multiplier", 2, 0);
    screen.createOption(0, "Adjust samples #", 2, 1);

    screen.createOption(0, "Run test", 1, 0);

In the loop() section, it is possible to see that at first, the user interface is completely responsive, being in a while loop, until the current screen is that of the graph, which means the test has started and is running. This permits the user to change the values of the variables of the system before starting the test.

    while(screen.getCurrentScreen() != 0 || screen.getContentType() != 1){
        screen.control();            //Controls the screen and redraws if needed while not in the test's graph
        keyboard.control();
        delay(REFRESH);
    }

Once the user enter the graph, the system enters another loop, in which it the keyboard gets blocked, waiting for the test to finish.

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

Finally, when this is done, the user interface shows the graph completely plotted and expects for the user to exit this screen by enabling the keyboard.

    while(screen.getCurrentScreen() == 0 && screen.getContentType() == 1){
        screen.control();            //Controls the screen and redraws if needed while in the test's graph
        keyboard.control();
        delay(REFRESH);
    }



[1]: <https://github.com/KrisKasprzak/96_Graphing> "Kris Kasprzak, OLED_Graphing.ino"