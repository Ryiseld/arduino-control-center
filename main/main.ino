#include <UTFT.h>
#include <URTouch.h>
#include <dht11.h>

// LCD and everything related to it
UTFT          myLCD(ILI9341_16, 38, 39, 40, 41);
URTouch       myTouch(6, 5, 4, 3, 2);

// DHT11 sensor
#define DHT11PIN 14
dht11 DHT11;

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

// Button parameters
#define BTN_OFFSET 20
#define BTN_HEIGHT 40

// Led control button
#define BTN_LED_CONTROL_X       35
#define BTN_LED_CONTROL_Y       50
#define BTN_LED_CONTROL_X2      285
#define BTN_LED_CONTROL_Y2      BTN_LED_CONTROL_Y + BTN_HEIGHT

// Distance sensor button
#define BTN_DIST_SENSOR_X       35
#define BTN_DIST_SENSOR_Y       BTN_LED_CONTROL_Y2 + BTN_OFFSET
#define BTN_DIST_SENSOR_X2      285
#define BTN_DIST_SENSOR_Y2      BTN_DIST_SENSOR_Y + BTN_HEIGHT

// DHT11 sensor button
#define BTN_DHT11_SENSOR_X       35
#define BTN_DHT11_SENSOR_Y       BTN_DIST_SENSOR_Y2 + BTN_OFFSET
#define BTN_DHT11_SENSOR_X2      285
#define BTN_DHT11_SENSOR_Y2      BTN_DHT11_SENSOR_Y + BTN_HEIGHT

char currentPage, selectedUnit, tempUnit;

int x, y;

// RGB LED
const int redLed = 10;
const int greenLed = 9;
const int blueLed = 8;
int xR = 38;
int xG = 38;
int xB = 38;

// Ultrasonic distance sensor
const int VCC = 13;
const int trigPin = 11;
const int echoPin = 12;

long duration;
int distanceInch, distanceCm;

void setup()
{
  // LCD and touch
  myLCD.InitLCD();
  myLCD.fillScr(0, 0, 0);
  
  myTouch.InitTouch();
  myTouch.setPrecision(PREC_MEDIUM);

  // Defining pin modes
  pinMode(VCC, OUTPUT);
  digitalWrite(VCC, HIGH);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(redLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(blueLed, OUTPUT);

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
        if ((x >= BTN_LED_CONTROL_X) && (x <= BTN_LED_CONTROL_X2) &&
            (y >= BTN_LED_CONTROL_Y) && (y <= BTN_LED_CONTROL_Y2)) {
          currentPage = '1';
          drawLedScreen();
        }
        
        // Distance sensor button
        if ((x >= BTN_DIST_SENSOR_X) && (x <= BTN_DIST_SENSOR_X2) &&
            (y >= BTN_DIST_SENSOR_Y) && (y <= BTN_DIST_SENSOR_Y2)) {
          currentPage = '2';
          drawDistControl();
        }
        
        // DHT11 sensor button
        if ((x >= BTN_DHT11_SENSOR_X) && (x <= BTN_DHT11_SENSOR_X2) &&
            (y >= BTN_DHT11_SENSOR_Y) && (y <= BTN_DHT11_SENSOR_Y2)) {
          currentPage = '3';
          drawDhtControl();
        }
        break;

      case '1': // LED Control
          if (!detectBackButton()) {
            setLedColor();
          } else {
            analogWrite(redLed, 0);
            analogWrite(greenLed, 0);
            analogWrite(blueLed, 0); 
          } 
        break;
        
      case '2': // Distance sensor
        if (!detectBackButton()) {
          showDistance();
           
          // If we press the Centimeters Button
          if ((x>=10) && (x<=135) &&(y>=90) && (y<=163)) {
            selectedUnit = '0';
          }
          
          // If we press the Inches Button
          if ((x>=10) && (x<=135) &&(y>=173) && (y<=201)) {
            selectedUnit = '1';
          }
        }
        break;
        
      case '3': // DHT11 sensor
        if (!detectBackButton()) {
          showDht(); 
          
          // If we press the Celcius Button
          if ((x>=10) && (x<=135) &&(y>=90) && (y<=163)) {
            tempUnit = '0';
          }
          
          // If we press the Fahrenheit Button
          if ((x>=10) && (x<=135) &&(y>=173) && (y<=201)) {
            tempUnit = '1';
          }
        }
        
        break;
    }
  }
}

// Screen functions
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
  myLCD.fillRoundRect(BTN_LED_CONTROL_X, BTN_LED_CONTROL_Y, BTN_LED_CONTROL_X2, BTN_LED_CONTROL_Y2);
  myLCD.setColor(255, 255, 255);
  myLCD.drawRoundRect(BTN_LED_CONTROL_X, BTN_LED_CONTROL_Y, BTN_LED_CONTROL_X2, BTN_LED_CONTROL_Y2);
  myLCD.setBackColor(16, 167, 103);
  myLCD.print("LED Control", CENTER, ((BTN_LED_CONTROL_Y + BTN_LED_CONTROL_Y2) / 2) - 5);
  
  // Distance sensor button
  myLCD.setColor(16, 167, 103);
  myLCD.fillRoundRect(BTN_DIST_SENSOR_X, BTN_DIST_SENSOR_Y, BTN_DIST_SENSOR_X2, BTN_DIST_SENSOR_Y2);
  myLCD.setColor(255, 255, 255);
  myLCD.drawRoundRect(BTN_DIST_SENSOR_X, BTN_DIST_SENSOR_Y, BTN_DIST_SENSOR_X2, BTN_DIST_SENSOR_Y2);
  myLCD.setBackColor(16, 167, 103);
  myLCD.print("Distance Sensor", CENTER, ((BTN_DIST_SENSOR_Y + BTN_DIST_SENSOR_Y2) / 2) - 5);
  
  // DHT11 sensor button
  myLCD.setColor(16, 167, 103);
  myLCD.fillRoundRect(BTN_DHT11_SENSOR_X, BTN_DHT11_SENSOR_Y, BTN_DHT11_SENSOR_X2, BTN_DHT11_SENSOR_Y2);
  myLCD.setColor(255, 255, 255);
  myLCD.drawRoundRect(BTN_DHT11_SENSOR_X, BTN_DHT11_SENSOR_Y, BTN_DHT11_SENSOR_X2, BTN_DHT11_SENSOR_Y2);
  myLCD.setBackColor(16, 167, 103);
  myLCD.print("DHT11 Sensor", CENTER, ((BTN_DHT11_SENSOR_Y + BTN_DHT11_SENSOR_Y2) / 2) - 5);

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

void drawDistControl()
{
  myLCD.clrScr();
  drawBackButton();
  
  myLCD.print("Ultrasonic Sensor", CENTER, 50);
  myLCD.print("Press a unit to see distance", CENTER, 76);
  myLCD.setColor(255, 0, 0);
  myLCD.drawLine(0,100,319,100);
  myLCD.setBackColor(0, 0, 0);
  myLCD.setColor(255, 255, 255);
  myLCD.setFont(SmallFont);
  myLCD.print("Select Unit", 10, 114);
  myLCD.setFont(BigFont);
  myLCD.print("Distance:", 130, 120);
  myLCD.setColor(223, 77, 55);
  myLCD.fillRoundRect (10, 135, 90, 163);
  myLCD.setColor(225, 255, 255);
  myLCD.drawRoundRect (10, 135, 90, 163);
  myLCD.setBackColor(223, 77, 55);
  myLCD.setColor(255, 255, 255);
  myLCD.print("cm", 33, 140);
  myLCD.setColor(223, 77, 55);
  myLCD.fillRoundRect (10, 173, 90, 201);
  myLCD.setColor(255, 255, 255);
  myLCD.drawRoundRect (10, 173, 90, 201);
  myLCD.setBackColor(223, 77, 55);
  myLCD.setColor(255, 255, 255);
  myLCD.print("inch", 17, 180);
}

void drawDhtControl()
{
  myLCD.clrScr();
  drawBackButton();
 
  myLCD.print("DHT11 Sensor", CENTER, 50);
  myLCD.print("Temperature and Humidity", CENTER, 76);
  myLCD.setColor(255, 0, 0);
  myLCD.drawLine(0,100,319,100);
  myLCD.setBackColor(0, 0, 0);
  myLCD.setColor(255, 255, 255);
  myLCD.setFont(SmallFont);
  myLCD.print("Select Unit", 10, 114);
  myLCD.setFont(BigFont);
  myLCD.print("Temperature:", 130, 120);
  myLCD.setColor(223, 77, 55);
  myLCD.fillRoundRect (10, 135, 90, 163);
  myLCD.setColor(225, 255, 255);
  myLCD.drawRoundRect (10, 135, 90, 163);
  myLCD.setBackColor(223, 77, 55);
  myLCD.setColor(255, 255, 255);
  myLCD.print("C", 40, 140);
  myLCD.setColor(223, 77, 55);
  myLCD.fillRoundRect (10, 173, 90, 201);
  myLCD.setColor(255, 255, 255);
  myLCD.drawRoundRect (10, 173, 90, 201);
  myLCD.setBackColor(223, 77, 55);
  myLCD.setColor(255, 255, 255);
  myLCD.print("F", 40, 180);
}

// Utility functions
boolean detectBackButton()
{
  if ((x >= 10) && (x <= 60) && (y >= 10) && (y <= 36)) { // Back button
    currentPage = '0';
    drawHomeScreen();
    return true;
  }
  
  return false;
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

void showDistance()
{
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  // Sets the trigPin to HIGH for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  
  // Calculate the distance
  distanceCm = duration * 0.034 / 2;
  distanceInch = distanceCm / 2.53;
  
  // Print the distance in centimeters
  if (selectedUnit == '0' && distanceCm <= 400) {
    myLCD.setFont(SevenSegNumFont);
    myLCD.setColor(0, 255, 0);
    myLCD.setBackColor(0, 0, 0);
    myLCD.printNumI(distanceCm,130, 145, 3,'0');
    myLCD.setFont(BigFont);
    myLCD.print("cm  ", 235, 178);
  
  }
  
  // Print the distance in inches
  if (selectedUnit == '1' && distanceCm <= 160) {
    myLCD.setFont(SevenSegNumFont);
    myLCD.setColor(0, 255, 0);
    myLCD.setBackColor(0, 0, 0);
    myLCD.printNumI(distanceInch,130, 145, 3,'0');
    myLCD.setFont(BigFont);
    myLCD.print("inch", 235, 178);
  } 
  
  delay(1);
}

void showDht()
{
  int chk = DHT11.read(DHT11PIN);
 
  // Print the temperature in Celcius
  if (tempUnit == '0') {
    myLCD.setFont(SevenSegNumFont);
    myLCD.setColor(0, 255, 0);
    myLCD.setBackColor(0, 0, 0);
    myLCD.printNumI((int)DHT11.temperature,130, 145, 3,'0');
    myLCD.setFont(BigFont);
    myLCD.print("C", 235, 178);
  
  }
  
  // Print the temperature in Fahrenheit
  if (tempUnit == '1') {
    myLCD.setFont(SevenSegNumFont);
    myLCD.setColor(0, 255, 0);
    myLCD.setBackColor(0, 0, 0);
    myLCD.printNumI((1.8 * (int)DHT11.temperature) + 32,130, 145, 3,'0');
    myLCD.setFont(BigFont);
    myLCD.print("F", 235, 178);
  } 
  
  // Print humidity
  myLCD.setFont(BigFont);
  myLCD.setColor(255, 255, 255);
  myLCD.print("% Humidity: ", 80, 220);
  myLCD.printNumI((int)DHT11.humidity, 260, 220, 2, '0');
}
