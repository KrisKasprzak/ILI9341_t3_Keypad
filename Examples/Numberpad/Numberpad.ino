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
uint16_t ScreenLeft = 350, ScreenRight = 3900, ScreenTop = 300, ScreenBottom = 3800;

// you know the drill
ILI9341_t3 Display(TFT_CS, TFT_DC, TFT_RST);

XPT2046_Touchscreen Touch(T_CS, T_IRQ);
TS_Point p;

// create some keypad objects
NumberPad MyNumberPad(&Display, &Touch);

void setup() {

  Serial.begin(9600);

  // fire up the display
  Display.begin();

  Display.setRotation(3);

  Touch.begin();
  Touch.setRotation(3);

  Display.fillScreen(ILI9341_BLACK);

  MyNumberPad.init(ILI9341_BLACK, ILI9341_BLACK, ILI9341_LIGHTGREY, ILI9341_WHITE, ILI9341_DARKGREY, FONT_BUTTON);
  MyNumberPad.setTouchLimits(ScreenLeft, ScreenRight, ScreenTop, ScreenBottom);

  // optional methods
  MyNumberPad.setDisplayColor(ILI9341_CYAN, ILI9341_DARKGREY);
  //MyNumberPad.useButtonIcon(true);  // want icons for action buttons? omit call or false for text.
  MyNumberPad.enableDecimal();      // disable / enable decimal point (enabled by default)
  MyNumberPad.enableNegative();     // disable / enable negative sign (enabled by default)
  // MyNumberPad.setCornerRadius(5); 
  // MyNumberPad.setMinMax(3, 300); // want bounds checks (disables OK button if out of range)?
  // use the value property to set the initial value if desired
  // MyNumberPad.setInitialText("Height?");
  // MyNumberPad.value = 3.14159; // set initial value
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
