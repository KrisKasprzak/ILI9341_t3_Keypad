// required libraries
#include "Adafruit_ILI9341.h"
#include "fonts\FreeSansBold12pt7b.h"
#include "XPT2046_Touchscreen.h"
#include "Adafruit_ILI9341_Keypad.h"

#define T_CS 0
#define T_IRQ 1
#define TFT_DC 9
#define TFT_CS 10
#define TFT_RST 8
#define PIN_LED 26

#define C_BUTTON 0x2945

// easy way to include fonts but change globally
#define FONT_BUTTON FreeSansBold12pt7b  // font for keypad buttons

int BtnX = 0, BtnY = 0;
int i = 0;

// you will probably need to calibrate your screen, these are coordinates of presses on display
uint16_t ScreenLeft = 350, ScreenRight = 3900, ScreenTop = 300, ScreenBottom = 3800;

// you know the drill
Adafruit_ILI9341 Display = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

XPT2046_Touchscreen Touch(T_CS, T_IRQ);
TS_Point p;

// create some keypad objects
NumberPad MyNumberPad(&Display, &Touch);

void setup() {

  Serial.begin(9600);

  // fire up the display
  Display.begin();

  Display.setRotation(1);

  Touch.begin();
  Touch.setRotation(1);

  Display.fillScreen(ILI9341_BLACK);

    /*
  while (1) {

    CalibrateScreen();
    delay(50);
  }
*/


  // initialize the numberpad object
  // simulate a good old fashioned number input
  // void init(uint16_t BackColor,uint16_t TextColor, uint16_t ButtonColor,
  // uint16_t PressedTextColor, uint16_t PressedButtonColor,
  // const GFXfont *ButtonFont);

  MyNumberPad.init(ILI9341_BLACK, ILI9341_WHITE, C_BUTTON, ILI9341_DARKGREEN, ILI9341_BLACK, &FONT_BUTTON);
  MyNumberPad.setTouchLimits(ScreenLeft, ScreenRight, ScreenTop, ScreenBottom);

  // optional methods
  MyNumberPad.setDisplayColor(ILI9341_CYAN, ILI9341_DARKGREY);
  MyNumberPad.useButtonIcon(true); // want icons for action buttons? omit call or false for text.
  MyNumberPad.enableDecimal();   // disable / enable decimal point (enabled by default)
  MyNumberPad.enableNegative();  // disable / enable negative sign (enabled by default)
  // MyNumberPad.setMinMax(3, 300); // want bounds checks (disables OK button if out of range)?
  // MyNumberPad.setCornerRadius(5);
  // MyNumberPad.setInitialText("Height?");  
  // use the value property to set the initial value if desired
  // MyNumberPad.value = 3.14159; // set initial value
  // MyNumberPad.hideInput();
}

void loop() {

  Display.fillScreen(ILI9341_BLACK);
  MyNumberPad.getInput();

  Serial.print("Entered value ");
  Serial.println(MyNumberPad.value);
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
