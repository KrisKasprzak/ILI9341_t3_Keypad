// required libraries
#include "ILI9341_t3.h"  // high speed display that ships with Teensy
#include "font_Arial.h"  // custom fonts that ships with ILI9341_t3.h
#include <font_ArialBold.h>
#include <ILI9341_t3_Controls.h>
#include <ILI9341_t3_Keypad_UTouch.h>
#include <UTouch.h>
// pins for LED and select button on encoder
#define CS_PIN 10
#define DC_PIN 9
#define RST_PIN 8
// easy way to include fonts but change globally
#define FONT_BUTTON Arial_16  // font for keypad buttons

int BtnX = 0, BtnY = 0;
int i = 0;

// you know the drill
// use the second one, this is a mod I made to the lib
ILI9341_t3 Display(CS_PIN, DC_PIN, RST_PIN);  //Display object

UTouch Touch(4, 3, 2, 1, 0);

// create some UI buttons
Button ShowPassword(&Display);
Button ShowNumberPad(&Display);
Button ShowKeyboard(&Display);

// create some keypad objects
NumberPadU PasswordInput(&Display, &Touch);
NumberPadU MyNumberPad(&Display, &Touch);
KeyboardU MyKeyboard(&Display, &Touch);

void setup() {

  Serial.begin(9600);

  // while ((millis() > 5000) || (!Serial)) {}

  // fire up the display
  Display.begin();
  Display.setRotation(1);

  Touch.InitTouch(PORTRAIT);
  Touch.setPrecision(PREC_EXTREME);
  Touch.calibrateRead();


  /*
  while (1)) {

    if (Touch.dataAvailable()) {
      Touch.read();
      int BtnX = Touch.getX();
      int BtnY = Touch.getY();
      Serial.print(" real coordinates:");
      Serial.print(BtnX);
      Serial.print(" ,");
      Serial.println(BtnY);
      BtnX = map(BtnX, 0, 230, 0, 320);
      BtnY = map(BtnY, 28, 390, 0, 240);


      Display.fillCircle(BtnX, BtnY, 2, ILI9341_RED);
    }
  }
*/

  Display.fillScreen(ILI9341_BLACK);

  ShowPassword.init(160, 50, 150, 50, ILI9341_NAVY, ILI9341_BLUE, ILI9341_WHITE, ILI9341_BLACK, "Password", 0, 0, FONT_BUTTON);
  ShowNumberPad.init(160, 120, 150, 50, ILI9341_NAVY, ILI9341_BLUE, ILI9341_WHITE, ILI9341_BLACK, "Number pad", 0, 0, FONT_BUTTON);
  ShowKeyboard.init(160, 190, 150, 50, ILI9341_NAVY, ILI9341_BLUE, ILI9341_WHITE, ILI9341_BLACK, "Keyboard", 0, 0, FONT_BUTTON);

  // initialize the numberpad object
  // let's simulate a password control (pure numbers only)
  PasswordInput.init(ILI9341_BLACK, ILI9341_WHITE, ILI9341_BLUE, ILI9341_NAVY, ILI9341_WHITE, ILI9341_CYAN, ILI9341_YELLOW, FONT_BUTTON);
  PasswordInput.hideInput();
  PasswordInput.enableDecimal(false);
  PasswordInput.enableNegative(false);
  PasswordInput.setInitialText("Enter password");

  // initialize the numberpad object
  // simulate a good old fashioned number input
  MyNumberPad.init(ILI9341_BLACK, ILI9341_WHITE, ILI9341_BLUE, ILI9341_NAVY, ILI9341_WHITE, ILI9341_CYAN, ILI9341_YELLOW, FONT_BUTTON);
  // optional methods
  // MyNumberPad.enableDecimal(bool State); // disable / enable decimal point (enabled by default)
  // MyNumberPad.enableNegative(bool State); // disable / enable negative sign (enabled by default)
  // MyNumberPad.setMinMax(float MininumValue, float MaximumValue); // want bound checks?
  // use the value property to set the initial value if desired
  MyNumberPad.value = 3.14159;

  // initialize the keyboard object
  // simulate a good old fashioned keyboard
  MyKeyboard.init(ILI9341_BLACK, ILI9341_WHITE, ILI9341_BLUE, ILI9341_NAVY, ILI9341_WHITE, ILI9341_CYAN, ILI9341_YELLOW, FONT_BUTTON);
  // optional methods
  // MyKeyboard.setDisplayColor(uint16_t TextColor, uint16_t BackColor); change colors
  // MyKeyboard.hideInput(); // for expanded password input
  // MyKeyboard.setInitialText(const char *Text); // maybe show and ip address format
  // MyKeyboard.setInitialText("IP 111.222.333.444");
  // optional to populate the input box
  // strcpy(MyKeyboard.data, "TEXT");
  drawMenu();
}

void drawMenu() {
  Display.fillScreen(ILI9341_BLACK);
  ShowPassword.draw();
  ShowNumberPad.draw();
  ShowKeyboard.draw();
  delay(500);
}

void loop() {

  if (Touch.dataAvailable()) {

    ProcessTouch();

    if (ProcessButtonPress(ShowPassword)) {
      Display.fillScreen(ILI9341_BLACK);
      PasswordInput.getInput();

      Display.fillScreen(ILI9341_BLACK);
      Display.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
      Display.setCursor(40, 50);
      Display.print("PasswordInput.value is: ");
      Display.setCursor(40, 100);
      Display.print((uint32_t)PasswordInput.value);
      Display.setCursor(40, 200);
      Display.print("Pass screen to continue.");
      PasswordInput.value = 0;
      while (!Touch.dataAvailable()) {
      }
      drawMenu();
    }
    if (ProcessButtonPress(ShowNumberPad)) {
      Display.fillScreen(ILI9341_BLACK);
      MyNumberPad.getInput();

      Display.fillScreen(ILI9341_BLACK);
      Display.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
      Display.setCursor(40, 50);
      Display.print("MyNumberPad.value is: ");
      Display.setCursor(40, 100);
      Display.print(MyNumberPad.value);
      Display.setCursor(40, 200);
      Display.print("Pass screen to continue.");
      MyNumberPad.value = 0;
      while (!Touch.dataAvailable()) {
      }
      drawMenu();
    }
    if (ProcessButtonPress(ShowKeyboard)) {
      Display.fillScreen(ILI9341_BLACK);
      MyKeyboard.getInput();

      Display.fillScreen(ILI9341_BLACK);
      Display.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
      Display.setCursor(40, 50);
      Display.print("MyKeyboard.data is: ");
      Display.setCursor(40, 100);
      Display.print(MyKeyboard.data);
      Display.setCursor(40, 200);
      Display.print("Pass screen to continue.");

      MyKeyboard.data[0] = '\0';
      //strcpy(MyKeyboard.data, ' ');
      while (!Touch.dataAvailable()) {
      }
      drawMenu();
    }
  }
}

void ProcessTouch() {

  if (Touch.dataAvailable()) {
    Touch.read();
    BtnX = Touch.getX();
    BtnY = Touch.getY();

#ifdef debug
    Serial.print(" real coordinates:");
    Serial.print(BtnX);
    Serial.print(" ,");
    Serial.print(BtnY);
#endif

    // different displays may require reversing last 2 args
    BtnX = map(BtnX, 0, 230, 0, 320);
    BtnY = map(BtnY, 28, 390, 0, 240);

#ifdef debug
    Serial.print(" , Mapped coordinates:");
    Serial.print(BtnX);
    Serial.print(" ,");
    Serial.println(BtnY);
    Display.fillCircle(BtnX, BtnY, 2, ILI9341_RED);
#endif
  }
}

bool ProcessButtonPress(Button TheButton) {

  if (TheButton.press(BtnX, BtnY)) {

    TheButton.draw(B_PRESSED);
    while (Touch.dataAvailable()) {
      if (TheButton.press(BtnX, BtnY)) {

        TheButton.draw(B_PRESSED);
      } else {
        TheButton.draw(B_RELEASED);
        return false;
      }
      ProcessTouch();
    }

    TheButton.draw(B_RELEASED);
    return true;
  }
  return false;
}
