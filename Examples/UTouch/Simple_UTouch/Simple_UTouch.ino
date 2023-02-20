// required libraries
#include "ILI9341_t3.h"
#include "font_Arial.h"
#include "ILI9341_t3_Keypad_UTouch.h"
#include <UTouch.h>

#define CS_PIN 10
#define DC_PIN 9
#define RST_PIN 8

#define FONT_BUTTON Arial_16  // font for keypad buttons that fits well

ILI9341_t3 Display(CS_PIN, DC_PIN, RST_PIN);  //Display object

// UTouch(byte tclk, byte tcs, byte tdin, byte dout, byte irq);
UTouch Touch(4, 3, 2, 1, 0);
KeyboardU MyKeyboard(&Display, &Touch);
// or
//NumberPadU MyNumberPad(&Display, &Touch);

void setup() {

  Serial.begin(9600);

  Display.begin();
  Display.setRotation(1);

  Touch.InitTouch(PORTRAIT);
  Touch.setPrecision(PREC_EXTREME);
  Touch.calibrateRead();

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
