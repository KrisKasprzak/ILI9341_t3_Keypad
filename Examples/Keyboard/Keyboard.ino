// required libraries
#include "ILI9341_t3.h"  // high speed display that ships with Teensy
#include "font_Arial.h"  // custom fonts that ships with ILI9341_t3.h
#include <font_ArialBold.h>
#include <ILI9341_t3_Keypad.h>
#include <XPT2046_Touchscreen.h>

// pins for LED and select button on encoder
#define CS_PIN 10
#define DC_PIN 9
#define RST_PIN 8

#define T_CS 0
#define T_IRQ 1

// easy way to include fonts but change globally
#define FONT_BUTTON Arial_16  // font for keypad buttons
uint16_t ScreenLeft = 3700, ScreenRight = 260, ScreenTop = 3800, ScreenBottom = 240;

// you know the drill
// use the second one, this is a mod I made to the lib
ILI9341_t3 Display(CS_PIN, DC_PIN);  //Display object


XPT2046_Touchscreen Touch(T_CS, T_IRQ);
TS_Point p;

Keyboard MyKeyboard(&Display, &Touch);

void setup() {

  Serial.begin(9600);

  // while ((millis() > 5000) || (!Serial)) {}

  // fire up the display
  Display.begin();
  Display.setRotation(3);

  Touch.begin();
  Touch.setRotation(3);

  Display.fillScreen(ILI9341_BLACK);

  //  you will probably have to calibrate your screen
  // uncomment and press upper left and lower right, get x and y for both
  // and update uint16_t ScreenLeft = 3700, ScreenRight = 260, ScreenTop = 3800, ScreenBottom = 240;
  /*
  while (1) {
    CalibrateScreen();
  }
*/

  // simulate a good old fashioned keyboard
  MyKeyboard.init(ILI9341_BLACK, ILI9341_WHITE, ILI9341_BLUE, ILI9341_DARKGREY, ILI9341_DARKGREY, ILI9341_NAVY, ILI9341_BLACK, FONT_BUTTON);
  MyKeyboard.setTouchLimits(ScreenRight, ScreenLeft, ScreenBottom, ScreenTop);
  // optional methods
  // max input characters is controlled by in the .h file
  // #define MAX_KEYBOARD_CHARS 18
  // change input display color
  MyKeyboard.setDisplayColor(ILI9341_WHITE, ILI9341_BLUE);
  // want rounded corners?
  // MyKeyboard.setCornerRadius(3);

  // Set initial instructions
  // MyKeyboard.setInitialText("IP 111.222.333.444");

  // MyKeyboard.hideInput(); // for hidden password input
  // MyKeyboard.setInitialText("Password");

  // optional to populate the input box
  //  strcpy(MyKeyboard.data, "TEXT");
}


void loop() {

  MyKeyboard.getInput();
  Serial.print("Password is: ");
  Serial.println(MyKeyboard.data);
}


void CalibrateScreen() {
  TS_Point p;
  if (Touch.touched()) {
    p = Touch.getPoint();
    int BtnX = p.x;
    int BtnY = p.y;
    Serial.print("Coordinates: ");
    Serial.print(BtnX);
    Serial.print(" ,");
    Serial.print(BtnY);

    BtnX = map(BtnX, ScreenLeft, ScreenRight, 0, 320);
    BtnY = map(BtnY, ScreenTop, ScreenBottom, 0, 240);

    Serial.print(" - Mapped: ");
    Serial.print(BtnX);
    Serial.print(" ,");
    Serial.println(BtnY);
    Display.fillCircle(BtnX, BtnY, 2, 255);
  }
}
