# Keyboard (numbers and characters) for the ILI9341_t3 and XPT2046_Touchscreen Libraries
This library includes 2 classes for getting user input using a 0-9 digit number pad and a full keyboard. This library is for the ILI9341_t3 driver to drive common 2.8" LCF displays, and also requires touch screen capability.  This library is dependent on the ILI9341_t3 and the XPT2046_Touchscreen libraries.

![header image](https://raw.github.com/KrisKasprzak/ILI9341_t3_Keypad/master/Images/09.jpg)
![header image](https://raw.github.com/KrisKasprzak/ILI9341_t3_Keypad/master/Images/25.jpg)
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
11. The library handles button presses, hence why you pass in the Touch object

<b><h3>General implementation</b></h3>
<br>
1. include the library
<br>
#include "Keypad.h"
<br>
#include "ILI9341_t3.h"  
<br>
#include "font_Arial.h" 
<br>
#include "ILI9341_t3_Controls.h" // required for the example only
<br>
#include "ILI9341_t3_Keypad.h"
<br>
#include "XPT2046_Touchscreen.h"
<br>
<br>
2. create a display object
<br>
3. create a touch object
<br>
4. create the keyboard and / or numberpad object
<br>
NumberPad MyNumberPad(&Display, &Touch);
Keyboard MyKeyboard(&Display, &Touch);
<br>
<br>
5. In setup, initialize the objects, and optionally specify any options
<br>
MyNumberPad.init(ILI9341_BLACK, ILI9341_WHITE, ILI9341_BLUE, ILI9341_NAVY, ILI9341_WHITE, ILI9341_CYAN, ILI9341_YELLOW, FONT_BUTTON);
<br>
  // MyNumberPad.enableDecimal(bool State); // disable / enable decimal point (enabled by default)
  <br>
  // MyNumberPad.enableNegative(bool State); // disable / enable negative sign (enabled by default)
  <br>
  // MyNumberPad.setMinMax(float MininumValue, float MaximumValue); // want bound checks?
  <br>
  // use the value property to set the initial value if desired
  <br>
  // MyNumberPad.value = 3.14159;
  <br>
  <br>
MyKeyboard.init(ILI9341_BLACK, ILI9341_WHITE, ILI9341_BLUE, ILI9341_NAVY, ILI9341_WHITE, ILI9341_CYAN, ILI9341_YELLOW, FONT_BUTTON);
<br>
  // MyKeyboard.setDisplayColor(uint16_t TextColor, uint16_t BackColor); change colors
  <br>
  // MyKeyboard.hideInput(); // for expanded password input
_  <br>
  // MyKeyboard.setInitialText(const char *Text); // maybe show and ip address format
  <br>
  // MyKeyboard.setInitialText("IP 111.222.333.444");
  <br>
  // optional to populate the input box
  <br>
  // strcpy(MyKeyboard.data, "TEXT");
  <br>
<br>

6. When ready get user input
<br>
MyNumberPad.getInput(); // this will display the key pad control
<br>
or
<br>
<br>
MyKeyboard.getInput(); // this will display the key pad control
<br>
<br>
7. Once the user completes input, read the results
<br>
Serial.println(MyNumberPad.value);
<br>
note that the data type is a float so you will need to recast if an int is desired
<br>
Serial.println((int) MyNumberPad.value);
<br>
or
<br>
Serial.println(MyKeyboard.data);
<br>
the data property is a char
<br>
<br>
<b><h3>Example Screenshots</b></h3>
<br>
<br>

![header image](https://raw.github.com/KrisKasprzak/ILI9341_t3_Keypad/master/Images/01.jpg)
<br>
![header image](https://raw.github.com/KrisKasprzak/ILI9341_t3_Keypad/master/Images/03.jpg)
![header image](https://raw.github.com/KrisKasprzak/ILI9341_t3_Keypad/master/Images/05.jpg)
![header image](https://raw.github.com/KrisKasprzak/ILI9341_t3_Keypad/master/Images/07.jpg)
![header image](https://raw.github.com/KrisKasprzak/ILI9341_t3_Keypad/master/Images/08.jpg)
![header image](https://raw.github.com/KrisKasprzak/ILI9341_t3_Keypad/master/Images/09.jpg)
![header image](https://raw.github.com/KrisKasprzak/ILI9341_t3_Keypad/master/Images/12.jpg)
![header image](https://raw.github.com/KrisKasprzak/ILI9341_t3_Keypad/master/Images/14.jpg)
![header image](https://raw.github.com/KrisKasprzak/ILI9341_t3_Keypad/master/Images/16.jpg)
<br>
![header image](https://raw.github.com/KrisKasprzak/ILI9341_t3_Keypad/master/Images/18.jpg)
![header image](https://raw.github.com/KrisKasprzak/ILI9341_t3_Keypad/master/Images/20.jpg)
![header image](https://raw.github.com/KrisKasprzak/ILI9341_t3_Keypad/master/Images/23.jpg)
![header image](https://raw.github.com/KrisKasprzak/ILI9341_t3_Keypad/master/Images/25.jpg)
![header image](https://raw.github.com/KrisKasprzak/ILI9341_t3_Keypad/master/Images/27.jpg)


