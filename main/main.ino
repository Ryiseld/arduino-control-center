#include <UTFT.h>
#include <URTouch.h>
//#include <UTFT_Buttons.h>

// LCD and everything related to it
UTFT          myLCD(ILI9341_16, 38, 39, 40, 41);
URTouch       myTouch(6, 5, 4, 3, 2);
//UTFT_Buttons  myButtons(&myLCD, &myTouch);

// Fonts
extern uint8_t SmallFont[];
extern uint8_t BigFont[];
extern uint8_t SevenSegNumFont[];

// Button defines
// Back button
#define BTN_BACK_X              10
#define BTN_BACK_Y              10
#define BTN_BACK_WIDTH          60
#define BTN_BACK_HEIGHT         36


// Led control button
#define BTN_LED_CONTROL_X       35
#define BTN_LED_CONTROL_Y       50
#define BTN_LED_CONTROL_WIDTH   285
#define BTN_LED_CONTROL_HEIGHT  90

char currentPage;

int x, y;

// RGB LED
const int redLed = 10;
const int greenLed = 9;
const int blueLed = 8;
int xR = 38;
int xG = 38;
int xB = 38;

void setup()
{
  myLCD.InitLCD();
  myLCD.fillScr(0, 0, 0);
  
  myTouch.InitTouch();
  myTouch.setPrecision(PREC_MEDIUM);

  drawHomeScreen();
  currentPage = '0';
}

void loop()
{
  if (myTouch.dataAvailable()) {
    myTouch.read();
    x = myTouch.getX();
    y = myTouch.getY();
        
    switch (currentPage) {
      case '0': // Home page
        // LED Control button
        if ((x >= BTN_LED_CONTROL_X) && (x <= BTN_LED_CONTROL_WIDTH) &&
            (y >= BTN_LED_CONTROL_Y) && (y <= BTN_LED_CONTROL_HEIGHT)) {
          currentPage = '1';
          drawLedScreen();
        }
        break;

      case '1': // LED Control
        if ((x >= 10) && (x <= 60) && (y >= 10) && (y <= 36)) { // Back button
          currentPage = '0';
          drawHomeScreen();
          analogWrite(redLed, 0);
          analogWrite(greenLed, 0);
          analogWrite(blueLed, 0);
        } else {
          setLedColor();
        }
        break;
    }
  }
}

void drawBackButton() {
  myLCD.setColor(100, 155, 203);
  myLCD.fillRoundRect (BTN_BACK_X, BTN_BACK_Y, BTN_BACK_WIDTH, BTN_BACK_HEIGHT);
  myLCD.setColor(255, 255, 255);
  myLCD.drawRoundRect (BTN_BACK_X, BTN_BACK_Y, BTN_BACK_WIDTH, BTN_BACK_HEIGHT);
  myLCD.setFont(BigFont);
  myLCD.setBackColor(100, 155, 203);
  myLCD.print("<-", BTN_BACK_X + 8, BTN_BACK_Y + 5);
  myLCD.setBackColor(0, 0, 0);
  myLCD.setFont(SmallFont);
  myLCD.print("Back to Main Menu", (BTN_BACK_X + BTN_BACK_WIDTH), ((BTN_BACK_Y + BTN_BACK_HEIGHT) / 2) - 4);
}

void drawHomeScreen()
{
  myLCD.clrScr();
  
  // Title
  myLCD.setBackColor(0, 0, 0);
  myLCD.setColor(255, 255, 255);
  myLCD.setFont(BigFont);
  myLCD.print("Control Center", CENTER, 10);

  // LED control button
  myLCD.setColor(16, 167, 103);
  myLCD.fillRoundRect(BTN_LED_CONTROL_X, BTN_LED_CONTROL_Y, BTN_LED_CONTROL_WIDTH, BTN_LED_CONTROL_HEIGHT);
  myLCD.setColor(255, 255, 255);
  myLCD.drawRoundRect(BTN_LED_CONTROL_X, BTN_LED_CONTROL_Y, BTN_LED_CONTROL_WIDTH, BTN_LED_CONTROL_HEIGHT);
  myLCD.setBackColor(16, 167, 103);
  myLCD.print("LED Control", CENTER, ((BTN_LED_CONTROL_Y + BTN_LED_CONTROL_HEIGHT) / 2) - 5);

  // Made by
  myLCD.setBackColor(0, 0, 0);
  myLCD.setColor(255, 255, 255);
  myLCD.setFont(SmallFont);
  myLCD.print("Made by Orel Lazri", 10, 220);
}

void drawLedScreen()
{
  myLCD.clrScr();
  
  drawBackButton();
  
  myLCD.setFont(BigFont);
  myLCD.print("RGB LED Control", CENTER, 50);
  myLCD.print("LED Color:", 10, 95);
  myLCD.print("R", 10, 135);
  myLCD.print("G", 10, 175);
  myLCD.print("B", 10, 215);
  myLCD.setColor(255, 0, 0);
  myLCD.drawLine(0,75,319,75); 
  myLCD.setColor(255, 255, 255);
  myLCD.drawRect(30, 138, 310, 148);
  myLCD.drawRect(30, 178, 310, 188);
  myLCD.drawRect(30, 218, 310, 228);  
}

void setLedColor()
{
  if (myTouch.dataAvailable()) {
      myTouch.read();
      x=myTouch.getX();
      y=myTouch.getY();        
      // Area of the Red color slider
      if( (y>=130) && (y<=156)) {
        xR=x; // Stores the X value where the screen has been pressed in to variable xR
        if (xR<=38) { // Confines the area of the slider to be above 38 pixels
          xR=38;
        }
        if (xR>=303){ /// Confines the area of the slider to be under 310 pixels
          xR=303;
        }
      }
      // Area of the Green color slider
      if( (y>=170) && (y<=196)) {
        xG=x;
        if (xG<=38) {
          xG=38;
        }
        if (xG>=303){
          xG=303;
        }
      }
      // Area of the Blue color slider
      if( (y>=210) && (y<=236)) {
        xB=x;
        if (xB<=38) {
          xB=38;
        }
        if (xB>=303){
          xB=303;
        }
      }    
    }
    // Maps the values of the X - Axis from 38 to 0 and 310 to 255, because we need values from 0 to 255 for turning on the led
    int xRC = map(xR,38,310,0,255);
    int xGC = map(xG,38,310,0,255);
    int xBC = map(xB,38,310,0,255);
    
    // Sends PWM signal to the pins of the led
    analogWrite(redLed, xRC);
    analogWrite(greenLed, xGC);
    analogWrite(blueLed, xBC);
    
    // Draws a rectangle with the latest color combination 
    myLCD.setColor(xRC, xGC, xBC);
    myLCD.fillRoundRect(175, 87, 310, 119);
    
    // Draws the positioners
    myLCD.setColor(255, 255, 255);
    myLCD.fillRect(xR,139,(xR+4),147); // Positioner
    myLCD.setColor(xRC, 0, 0);
    myLCD.fillRect(31, 139, (xR-1), 147);
    myLCD.setColor(0, 0, 0);
    myLCD.fillRect((xR+5), 139, 309, 147);
    
    myLCD.setColor(255, 255, 255);
    myLCD.fillRect(xG,179,(xG+4),187);
    myLCD.setColor(0, xGC, 0);
    myLCD.fillRect(31, 179, (xG-1), 187);
    myLCD.setColor(0, 0, 0);
    myLCD.fillRect((xG+5), 179, 309, 187);
    
    myLCD.setColor(255, 255, 255);
    myLCD.fillRect(xB,219,(xB+4),227);
    myLCD.setColor(0, 0, xBC);
    myLCD.fillRect(31, 219, (xB-1), 227);
    myLCD.setColor(0, 0, 0);
    myLCD.fillRect((xB+5), 219, 309, 227);
}
