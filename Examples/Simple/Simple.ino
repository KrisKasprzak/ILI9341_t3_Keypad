// required libraries
#include "ILI9341_t3.h"
#include "font_Arial.h"
#include <ILI9341_t3_Keypad.h>
#include <XPT2046_Touchscreen.h>

#define CS_PIN 10
#define DC_PIN 9
#define T_CS 0
#define T_IRQ 1

#define FONT_BUTTON Arial_16  // font for keypad buttons

int BtnX = 0, BtnY = 0;

ILI9341_t3 Display(CS_PIN, DC_PIN, 240, 320, 8);  //Display object

XPT2046_Touchscreen Touch(T_CS, T_IRQ);
TS_Point p;

Keyboard MyKeyboard(&Display, &Touch);
// or
NumberPad MyNumberPad(&Display, &Touch);

void setup() {

  Serial.begin(9600);

  Display.begin();
  Display.setRotation(1);

  Touch.begin();
  Touch.setRotation(1);

  Display.fillScreen(ILI9341_BLACK);

  MyKeyboard.init(ILI9341_BLACK, ILI9341_WHITE, ILI9341_BLUE, ILI9341_NAVY, ILI9341_WHITE, ILI9341_CYAN, ILI9341_YELLOW, FONT_BUTTON);
  // or
  // MyNumberPad.init(ILI9341_BLACK, ILI9341_WHITE, ILI9341_BLUE, ILI9341_NAVY, ILI9341_WHITE, ILI9341_CYAN, ILI9341_YELLOW, FONT_BUTTON);

  MyKeyboard.getInput();
  // MyNumberPad.getInput();

  Serial.print(MyKeyboard.data);
  // or
  // Serial.print(MyNumberPad.value);

}

void loop() {

}
