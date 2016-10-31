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
  myLCD.setColor(168, 168, 168);
  myLCD.setFont(SmallFont);
  myLCD.print("Made by Orel Lazri", 10, 220);
}

void drawLedScreen() {
  myLCD.clrScr();

  // Back button
  drawBackButton();

  // Title
  myLCD.setBackColor(0, 0, 0);
  myLCD.setColor(255, 255, 255);
  myLCD.setFont(BigFont);
  myLCD.print("LED Control", CENTER, 50);
}

