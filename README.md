# Keyboard

This library includes 2 classes for getting user input using a 0-9 digit number pad and a full keyboard. This library is for the ILI9341_t3 driver to drive common 2.8" LCF displays. 
It also requires touch screen. 

<br>
<b><h3>Library highlights</b></h3>
1. Simple to implement library
2. Users can include both input types in the same program
3. The keyboard includes all charaters, upper and lower case as well as all special symbols.
4. The keyboard colors can be configured, but due to size, it will consume the entire screen
5. The number pad can be configured for both colors and button sizes
6. Both classes require a custom for for a better visual experience
7. Both classes can allow hidden character input for password entry
8. Both classes can allow initial instruction text in the input field
9. Both classes can allow a predefined input 
10. The number input can handle decimal and negative number inputs and can enable or disabled

<b><h3>General implementation</b></h3>
<br>
1. include the library
<br>
#include "Keypad.h"
#include "ILI9341_t3.h"  
#include "font_Arial.h" 
#include <ILI9341_t3_Controls.h> // required for the example only
#include <ILI9341_t3_Keypad.h>
#include <XPT2046_Touchscreen.h>
<br>
<br>
2. create a display object
<br>
3. create a touch object
<br>
4. create the keyboard and or numberpad object
<br>
NumberPad MyNumberPad(&Display, &Touch);
Keyboard MyKeyboard(&Display, &Touch);
<br>
<br>
5. In setup, initialize the objects, 6. and optionally specify any options
<br>
MyNumberPad.init(ILI9341_BLACK, ILI9341_WHITE, ILI9341_BLUE, ILI9341_NAVY, ILI9341_WHITE, ILI9341_CYAN, ILI9341_YELLOW, FONT_BUTTON);
  // MyNumberPad.enableDecimal(bool State); // disable / enable decimal point (enabled by default)
  // MyNumberPad.enableNegative(bool State); // disable / enable negative sign (enabled by default)
  // MyNumberPad.setMinMax(float MininumValue, float MaximumValue); // want bound checks?
  // use the value property to set the initial value if desired
  // MyNumberPad.value = 3.14159;
MyKeyboard.init(ILI9341_BLACK, ILI9341_WHITE, ILI9341_BLUE, ILI9341_NAVY, ILI9341_WHITE, ILI9341_CYAN, ILI9341_YELLOW, FONT_BUTTON);
  // MyKeyboard.setDisplayColor(uint16_t TextColor, uint16_t BackColor); change colors
  // MyKeyboard.hideInput(); // for expanded password input
  // MyKeyboard.setInitialText(const char *Text); // maybe show and ip address format
  // MyKeyboard.setInitialText("IP 111.222.333.444");
  // optional to populate the input box
  // strcpy(MyKeyboard.data, "TEXT");
<br>

5. When ready get user input
<br>
MyNumberPad.getInput();
or
MyKeyboard.getInput();
<br>
6. Once the user completes input, read the results
<br>
Serial.println(MyNumberPad.value);
note that the data type is a float so you will need to recast if an int is desired
Serial.println((int) MyNumberPad.value);
or
Serial.println(MyKeyboard.data);
the data property is a char


<b><h3>ToDo...</b></h3>
1. test more
2. document the examples and code
3. Add and example on how to read data and write to and SD card)
5. create YouTube vid in how to use
6. test with other chips
7. remove hard code for chip size, pages, etc.


