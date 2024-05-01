// required libraries
#include "ILI9341_t3.h"  // high speed display that ships with Teensy
#include "font_Arial.h"  // custom fonts that ships with ILI9341_t3.h
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

// you will probably need to calibrate your screen, these are coordinates of presses on display
uint16_t ScreenLeft = 350, ScreenRight = 3900, ScreenTop = 300, ScreenBottom = 3800;


ILI9341_t3 Display(CS_PIN, DC_PIN, RST_PIN);

XPT2046_Touchscreen Touch(T_CS, T_IRQ);

TS_Point TP;


Keyboard SSIDPasswordPad(&Display, &Touch);


void setup() {


  Serial.begin(115200);

  Display.begin();

  Display.setRotation(3);
  Display.fillScreen(ILI9341_BLACK);

  Touch.begin();
  Touch.setRotation(3);

  SSIDPasswordPad.init(ILI9341_BLACK, ILI9341_BLACK, ILI9341_LIGHTGREY, ILI9341_WHITE, ILI9341_DARKGREY, FONT_BUTTON);
  SSIDPasswordPad.setTouchLimits(ScreenLeft, ScreenRight, ScreenTop, ScreenBottom);

  // optional
  // SSIDPasswordPad.setCornerRadius(5);  
  SSIDPasswordPad.setDisplayColor(40, 48520);
  // SSIDPasswordPad.hideInput();
  // SSIDPasswordPad.clearInput();
  SSIDPasswordPad.setInitialText("Enter IP Address");


}

void loop() {

  SSIDPasswordPad.getInput();

  Display.fillScreen(ILI9341_BLACK);

  Serial.print("data method 1 ");
  Serial.println(SSIDPasswordPad.data);

  // or
  const char *pw;

  pw = SSIDPasswordPad.data;
  Serial.print("data method 2 ");
  Serial.println(pw);

  SSIDPasswordPad.clearInput();
  SSIDPasswordPad.setInitialText("Enter IP Address");

}
