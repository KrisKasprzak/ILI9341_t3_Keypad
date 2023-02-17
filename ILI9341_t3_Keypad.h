/*
  The MIT License (MIT)

  library writen by Kris Kasprzak

  Permission is hereby granted, free of charge, to any person obtaining a copy of
  this software and associated documentation files (the "Software"), to deal in
  the Software without restriction, including without limitation the rights to
  use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
  the Software, and to permit persons to whom the Software is furnished to do so,
  subject to the following conditions:
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
  FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
  COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
  IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
  CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

  On a personal note, if you develop an application or product using this library
  and make millions of dollars, I'm happy for you!

  rev   date      author        change
  1.0   2/12/2023      kasprzak      initial code


*/

#ifndef ILI9341_KEYPAD_H
#define ILI9341_KEYPAD_H

#define ILI9341_KEYPAD_VER 1.0

#if ARDUINO >= 100
	 #include "Arduino.h"
	 #include "Print.h"
#else
	
#endif

#ifdef __cplusplus
	
#endif

#include <ILI9341_t3.h> 
#include <ILI9341_t3_Controls.h> 	// button library
#include <XPT2046_Touchscreen.h>

#define BTNS 30
#define BTNM 2

#define ROW1 54
#define ROW2 86
#define ROW3 118
#define ROW4 150
#define ROW5 182
#define ROW6 220


#define COL1 17
#define COL2 49
#define COL3 81
#define COL4 113
#define COL5 145
#define COL6 177
#define COL7 209
#define COL8 241
#define COL9 273
#define COL10 305

#define MAX_KEYBOARD_CHARS 20


class  NumberPad {
		
public:

	NumberPad(ILI9341_t3 *Display, XPT2046_Touchscreen *Touch);

	void init(uint16_t BackColor,uint16_t TextColor, uint16_t ButtonColor, uint16_t BorderColor, 
	uint16_t PressedTextColor, uint16_t PressedButtonColor, uint16_t PressedBorderColor,
	const ILI9341_t3_font_t &ButtonFont);

	void getInput();
	
	void setLocation(uint16_t CenterWidth, uint16_t CenterHeight);
	
	void setButtonSizes(uint16_t ButtonWidth, uint16_t ButtonHeight, uint16_t Margins, uint16_t OKButtonWidth, uint16_t OKButtonHeight);
	
	void enableDecimal(bool State);
	
	void enableNegative(bool State);
	
	void setDisplayColor(uint16_t TextColor, uint16_t BackColor);
	
	void setMinMax(float MininumValue, float MaximumValue);
	
	void setInitialText(const char *Text);
	
	void hideInput();
	
	
	void setColors(
		uint16_t BackColor, 
		uint16_t TextColor, 
		uint16_t ButtonColor, 
		uint16_t BorderColor, 
		uint16_t PressedTextColor, 
		uint16_t PressedButtonColor,
		uint16_t PressedBorderColor
		);

	float value;

private:

	
	ILI9341_t3 *d;
	XPT2046_Touchscreen  *t;
	TS_Point p;
	
	void ProcessTouch();
	
	bool ProcessButtonPress(Button TheButton);
	
	uint16_t CW = 160;  // width center of screen
	uint16_t CH = 120;  // height center of screen
	uint16_t BH = 35;
	uint16_t BW = 60;
	uint16_t BS = 5;
	uint16_t TBH = 30;
	uint16_t OKBW = 70;
	uint16_t OKBH = 70;
	char inittext[MAX_KEYBOARD_CHARS];  // display initial text
	char hc[MAX_KEYBOARD_CHARS];
	char dn[MAX_KEYBOARD_CHARS];  // display number
	uint16_t BtnX, BtnY;
	uint16_t kcolor;
	uint16_t tcolor; 
	uint16_t bcolor;
	uint16_t rcolor;
	uint16_t ptextcolor;
	int16_t inputb;
	int16_t inputt;
	bool decstate = true;
	bool negstate = true;
	bool minmaxstate = false;
	bool hasinittext = false;
	bool hideinput = false;
	float minval = 0.0;
	float maxval = 0.0;
	ILI9341_t3_font_t bfont;
	


};

class  Keyboard {
		
public:

	Keyboard(ILI9341_t3 *Display, XPT2046_Touchscreen *Touch);

	void init(uint16_t BackColor,uint16_t TextColor, uint16_t ButtonColor, uint16_t BorderColor, 
	uint16_t PressedTextColor, uint16_t PressedButtonColor, uint16_t PressedBorderColor,
	const ILI9341_t3_font_t &ButtonFont);

	void getInput();
	
	void setDisplayColor(uint16_t TextColor, uint16_t BackColor);
	
	void hideInput();
	
	void setInitialText(const char *Text);
	
	void setColors(
		uint16_t BackColor, 
		uint16_t TextColor, 
		uint16_t ButtonColor, 
		uint16_t BorderColor, 
		uint16_t PressedTextColor, 
		uint16_t PressedButtonColor,
		uint16_t PressedBorderColor
		);

	 char data[MAX_KEYBOARD_CHARS];

private:

	
	ILI9341_t3 *d;
	XPT2046_Touchscreen  *t;
	TS_Point p;
	
	void ProcessTouch();
	char dn[MAX_KEYBOARD_CHARS];
    char hc[MAX_KEYBOARD_CHARS];
	char inittext[MAX_KEYBOARD_CHARS];  // display initial text
	bool ProcessButtonPress(Button TheButton);
	bool aclear = false;
	bool hideinput = false;
	bool hasinittext = false;
	uint16_t BtnX, BtnY;
	uint16_t kcolor;
	uint16_t tcolor; 
	uint16_t bcolor;
	uint16_t rcolor;
	uint16_t ptextcolor;
	int16_t inputb;
	int16_t inputt;
	ILI9341_t3_font_t bfont;

};




#endif