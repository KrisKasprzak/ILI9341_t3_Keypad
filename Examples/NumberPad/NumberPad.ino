// required libraries
#include "ILI9341_t3.h"  // high speed display that ships with Teensy
#include "font_Arial.h"  // custom fonts that ships with ILI9341_t3.h
#include <font_ArialBold.h>
#include <ILI9341_t3_Keypad.h>
#include <XPT2046_Touchscreen.h>

// For Teensy
#define TFT_CS 10
#define TFT_DC 9
#define TFT_RST 8
#define T_CS 0
#define T_IRQ 1

// easy way to include fonts but change globally
#define FONT_BUTTON Arial_16_Bold  // font for keypad buttons

// you will probably need to calibrate your screen, these are coordinates of presses on display
uint16_t ScreenLeft = 3700, ScreenRight = 260, ScreenTop = 3800, ScreenBottom = 240;

// you know the drill
ILI9341_t3 Display(TFT_CS, TFT_DC, TFT_RST);

XPT2046_Touchscreen Touch(T_CS, T_IRQ);


// create some keypad objects
NumberPad MyNumberPad(&Display, &Touch);


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
void setup() {

  Serial.begin(9600);

  // fire up the display
  Display.begin();

  Display.setRotation(3);

  Touch.begin();
  Touch.setRotation(3);


  Display.fillScreen(ILI9341_BLACK);
  /*
  while (1) {

    CalibrateScreen();
    delay(50);
  }
*/
  MyNumberPad.init(ILI9341_BLACK, ILI9341_WHITE, ILI9341_BLUE, ILI9341_NAVY, ILI9341_DARKGREY, ILI9341_NAVY, ILI9341_BLACK, FONT_BUTTON);
  MyNumberPad.setTouchLimits(ScreenRight, ScreenLeft, ScreenBottom, ScreenTop);

  // optional methods
  MyNumberPad.setDisplayColor(ILI9341_CYAN, ILI9341_DARKGREY);
  //MyNumberPad.useButtonIcon(true);  // want icons for action buttons? omit call or false for text.
  MyNumberPad.enableDecimal(true);   // disable / enable decimal point (enabled by default)
  MyNumberPad.enableNegative(true);  // disable / enable negative sign (enabled by default)
  MyNumberPad.setCornerRadius(5);
  MyNumberPad.setMinMax(3, 300);  // want bounds checks (disables OK button if out of range)?
                                  // use the value property to set the initial value if desired
  MyNumberPad.setInitialText("Height?");
  MyNumberPad.value = 3.14159;  // set initial value
  // MyNumberPad.hideInput();
}

void loop() {

  Display.fillScreen(ILI9341_BLACK);
  MyNumberPad.getInput();

  Serial.print("Entered value ");
  Serial.println(MyNumberPad.value);
  // reset if needed
  // MyNumberPad.value = 0;
}