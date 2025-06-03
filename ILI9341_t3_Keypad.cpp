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

*/

#include "ILI9341_t3_Keypad.h"
#include <ILI9341_t3.h>
// #include <ILI9341_t3_Controls.h>  // button library
#include <XPT2046_Touchscreen.h>


NumberPad::NumberPad(ILI9341_t3 *Display, XPT2046_Touchscreen *Touch) {
  d = Display;
  t = Touch;
}

void NumberPad::init(uint16_t BackColor, uint16_t TextColor,
                  uint16_t ButtonColor,
                  uint16_t PressedTextColor,
                  uint16_t PressedButtonColor, uint16_t PressedBorderColor,
                  const ILI9341_t3_font_t &ButtonFont) {

	kcolor = BackColor;
	tcolor = TextColor;
	ptcolor = PressedTextColor;
	bcolor = ButtonColor;
	pbcolor = PressedButtonColor;
	rcolor = PressedBorderColor;
	ptextcolor = PressedTextColor;
	inputt = BackColor;
	inputb = TextColor;
	value = 0.0;
	bfont = ButtonFont;
	bHigh = 40;
	bWide = 40;
	rad = 0;
	numdec = 3;
	clickpin = -1;
   
  // in this class we are NOT initially writing to the char[0] as it's reserved for the - sign
  // hence we need to populate it to eliminate null terminator


}

void NumberPad::setTouchLimits(uint16_t ScreenLeft, uint16_t ScreenRight, uint16_t ScreenTop, uint16_t ScreenBottom) {

  screenX0 = ScreenLeft;
  screenX320 = ScreenRight;
  screenY0 = ScreenTop;
  screenY240 = ScreenBottom;
}

char *NumberPad::getCharValue() {

  return dn;
}

void NumberPad::setDisplayColor(uint16_t TextColor, uint16_t BackColor) {
  inputt = TextColor;
  inputb = BackColor;
}

void NumberPad::setLocation(uint16_t CenterWidth, uint16_t CenterHeight) {
  CW = CenterWidth;   // width center of screen
  CH = CenterHeight;  // height center of screen
}

void NumberPad::setButtonSizes(uint16_t ButtonWidth, uint16_t ButtonHeight, uint16_t Margins, uint16_t OKButtonWidth, uint16_t OKButtonHeight) {

  BW = ButtonWidth;
  BH = ButtonHeight;
  BS = Margins;
  OKBW = OKButtonWidth;
  OKBH = OKButtonHeight;
}

void NumberPad::enableDecimal(bool State ) {
  decstate = State;
}

void NumberPad::enableNegative(bool State ) {
  negstate = State;
}

void NumberPad::setMinMax(float MininumValue, float MaximumValue) {

  if (MininumValue == MaximumValue) {
    minmaxstate = false;
  } else {
    minmaxstate = true;
  }

  minval = MininumValue;
  maxval = MaximumValue;
}

void NumberPad::setCornerRadius(uint8_t Radius) {
  rad = Radius;
  if (Radius > 10) {
    rad = 10;
  }
}

void NumberPad::setInitialText(const char *Text) {

  uint8_t i;

  for (i = 0; i < (MAX_KEYBOARD_CHARS); i++) {
    inittext[i] = Text[i];
  }
  hasinittext = true;
}

void NumberPad::hideInput() {

  hideinput = true;
}

void NumberPad::setDecimals(uint8_t Value) {

numdec = Value;

}

void NumberPad::setClickPin(int Value) {

clickpin = Value;

}

void NumberPad::getInput() {

  uint16_t i = 0;
  uint16_t b = 0;
  bool hasDP = false;
  uint8_t np = 1;  // digit number
  // bool CanBackUp = false;
  bool hasneg = false;
  bool KeepIn = true;
  float TheNumber = 0.0;
    
	


  memset(dn, '\0', MAX_KEYBOARD_CHARS + 2);
  dn[0] = ' ';
  memset(hc, '\0', MAX_KEYBOARD_CHARS + 2);
  hc[0] = ' ';

	
dtostrf(value, 0, numdec, dn);
		/*
  if (value != 0.0) {

	if (value < 0) {
		hasneg = true;
	}

	if (numdec== 0){
		sprintf(dn, "% d", (int) value);
	}
	else if (numdec == 1) {
		sprintf(dn, "% 0.1f",  value);	
	}
	else if (numdec == 2) {
		sprintf(dn, "% 0.2f",  value);	
	}
	else if (numdec == 3) {
		sprintf(dn, "% 0.3f",  value);	
		



	}
	else if (numdec == 4) {
		sprintf(dn, "% 0.4f",  value);	
	}
	else  {
		sprintf(dn, "% 0.5f",  value);	
	}

	
  }
	*/
	
	np = strlen(dn); 
	/*
    // get the decimals
	  for (i = 0; i < strlen(dn); i++){
		  
		  if(dn[i] == '.'){
			  hasDP = true;
			  break;
		  }
	  }

	// strip trailing zeros
	if (hasDP) {
		
		for ( i = strlen(dn)-1; i > 0; i--) {
					
			if (dn[i] == '.'){
				dn[i] = '\0';
				np--;
				hasDP = false;
			}
			else if (dn[i] == '0'){
				dn[i] = '\0';
				np--;
			}
			else {
				break;
			}
		}
	}
	*/
  if (hideinput){
	  hc[0] = ' ';
    for (i = 1; i < strlen(dn); i++){	  
	  hc[i] = '*';
	}
  }

  BUTTON Buttons[15];

  BuildButton(&Buttons[0], Col2, Row4, bWide, bHigh, 0x30);             // 0
  BuildButton(&Buttons[1], Col1, Row1, bWide, bHigh, 0x31);             // 1
  BuildButton(&Buttons[2], Col2, Row1, bWide, bHigh, 0x32);             // 2
  BuildButton(&Buttons[3], Col3, Row1, bWide, bHigh, 0x33);             // 3
  BuildButton(&Buttons[4], Col1, Row2, bWide, bHigh, 0x34);             // 4
  BuildButton(&Buttons[5], Col2, Row2, bWide, bHigh, 0x35);             // 5
  BuildButton(&Buttons[6], Col3, Row2, bWide, bHigh, 0x36);             // 6
  BuildButton(&Buttons[7], Col1, Row3, bWide, bHigh, 0x37);             // 7
  BuildButton(&Buttons[8], Col2, Row3, bWide, bHigh, 0x38);             // 8
  BuildButton(&Buttons[9], Col3, Row3, bWide, bHigh, 0x39);             // 9
  BuildButton(&Buttons[10], Col1, Row4, bWide, bHigh, 0x2E);            // .
  BuildButton(&Buttons[11], Col3, Row4, bWide, bHigh, 0x2D);            // -
  BuildButton(&Buttons[12], Col4, Row0, bWide * 2, bHigh, 0x00);            // back
  BuildButton(&Buttons[13], Col4, Row1, bWide * 2, (bHigh * 2) + 5, 0x01);  // OK
  BuildButton(&Buttons[14], Col4, Row3, bWide * 2, (bHigh * 2) + 5, 0x02);  // cancel

  // large background box
  d->fillRect(Col1 - 5, Row0 - 5, Col4 - Col1 + Buttons[13].w + 10, Row4 - Row0 + bHigh + 10, kcolor);

  // text input box
  d->fillRect(Col1, Row0, Col3 - Col1 + bWide, bHigh, inputb);
  d->setFont(bfont);
  d->setTextColor(inputt, inputb);
  d->setCursor(Col1 + 5, Row0 + 10);

	if (hasinittext) {
		d->print(inittext);
	} 
	else if (hideinput){
		d->print(hc);
	}
	else {
		d->print(dn);
	}


  for (i = 0; i < 15; i++) {
    DrawButton(&Buttons[i], BUTTON_RELEASED);
  }
  
  while (KeepIn) {

    if (t->touched()) {

      ProcessTouch();
      for (b = 0; b < 15; b++) {

        if (Pressed(&Buttons[b])) {
          //valid number
          if (b <= 9) {
            if (np > (MAX_KEYBOARD_CHARS-1)) {
              break;
            }
            if ((dn[1] == '0') && (dn[2] != '.')) {

              dn[1] = b + '0';
              hc[1] = '*';
            } else {

              dn[np] = b + '0';
              hc[np] = '*';
              np++;
            }
          } else if (b == 11) {
            //negative number
            if (dn[0] == '-') {
              dn[0] = ' ';
            } else {
              dn[0] = '-';
            }
          } else if (b == 10) {
            // decimal point
            if (!hasDP) {
              dn[np] = '.';
              hc[np] = '*';
              hasDP = true;
              np++;
            }
          } else if (b == 12) {
            // back space
            if (np > 0) {
              --np;
              if (dn[np] == '.') { hasDP = false; }
              dn[np] = ' ';
              hc[np] = ' ';
            }

          } else if (b == 13) {

            // done
            TheNumber = atof(dn);
            value = TheNumber;
            KeepIn = false;
            break;
          } else if (b == 14) {
            // cancel, just get the heck out
            KeepIn = false;

            break;
          }
        }
		
	  }
	if (minmaxstate) {
	  TheNumber = atof(dn);
	  // check min bounds
	  if ((TheNumber < minval) || (TheNumber > maxval)){
		  rangeOK = false;
		  DrawButton(&Buttons[13], BUTTON_PRESSED);
	  }
	  else {
		  rangeOK = true;
		  DrawButton(&Buttons[13], BUTTON_RELEASED);
	  }
	}
      

      // text input box
      d->fillRect(Col1, Row0, Col3 - Col1 + bWide, bHigh, inputb);
	  d->setFont(bfont);
      d->setTextColor(inputt, inputb);
	  d->setCursor(Col1 + 5, Row0 + 10);
      

      if (hideinput) {
        d->print(hc);
      } else {
        d->print(dn);
      }
    }
  }

  // if no negative / shift chars
  if (!negstate) {
    for (i = 1; i < MAX_KEYBOARD_CHARS; i++) {
      dn[i - 1] = dn[i];
    }
    dn[i] = '\0';
  }
  

}

void NumberPad::useButtonIcon(bool UseIcon) { 
		useicon = UseIcon;
	}

void NumberPad::BuildButton(BUTTON *temp, int Col, int Row, uint8_t Wide, uint8_t High, uint8_t ascii) {
  temp->x = Col;
  temp->y = Row;
  temp->w = Wide;
  temp->h = High;
  temp->ascii = ascii;
}

void NumberPad::DrawButton(BUTTON *temp, uint8_t State) {

	if (!decstate && (temp->ascii == 0x2E)){
		return;
	}

	if (!negstate && (temp->ascii == 0x2D)){
		return;
	}
  
  
  if (State == BUTTON_PRESSED) {
    if (rad > 0) {
      d->fillRoundRect(temp->x, temp->y, temp->w, temp->h, rad, pbcolor);
    } else {
      d->fillRect(temp->x, temp->y, temp->w, temp->h, pbcolor);
    }
    d->setTextColor(ptcolor, bcolor);
  } else {
    if (rad > 0) {
      d->fillRoundRect(temp->x, temp->y, temp->w, temp->h, rad, bcolor);
    } else {
      d->fillRect(temp->x, temp->y, temp->w, temp->h, bcolor);
    }
    d->setTextColor(tcolor, bcolor);
  }
  
	 if (useicon){
		 
		if (temp->ascii == 0x00) {
			drawMonoBitmap(temp->x+ 10,temp->y+5, backspace, 54, 30 , ILI9341_KEYPAD_BLUE);
		} else if (temp->ascii == 0X01) {
			drawMonoBitmap(temp->x+15, temp->y+20, check, 50, 50 , ILI9341_KEYPAD_GREEN);
		} else if (temp->ascii == 0x02) {
			drawMonoBitmap(temp->x+12, temp->y+20, cancel, 50, 50 , ILI9341_KEYPAD_RED);
		}
	 }
	 else {
	  d->setFont(bfont);
	  if (temp->ascii == 0x00) {
		d->setCursor(temp->x+ 10,temp->y+10);
		d->print("Back");
	  } else if (temp->ascii == 0X01) {
		d->setCursor(temp->x+ 20,temp->y+50);
		d->print("OK");
	  } else if (temp->ascii == 0x02) {
		d->setCursor(temp->x+ 3,temp->y+50);
		d->print("Cancel");
	  }
	  }
	  
	const char text = temp->ascii;

	uint16_t xx = d->measureTextWidth(&text)/2;
	uint16_t yy = d->measureTextHeight(&text)/2;
	
	d->setCursor(temp->x +xx, temp->y + yy);
	
	d->print(text);	 
	 
}

	void NumberPad::drawMonoBitmap(uint16_t x, uint16_t y, const unsigned char *bitmap, uint8_t w, uint8_t h, uint16_t color) {

	  uint8_t sbyte = 0;
	  uint8_t byteWidth = 0;
	  uint16_t jj, ii;

	  byteWidth = (w + 7) / 8;

	  for (jj = 0; jj < h; jj++) {
		for (ii = 0; ii < w; ii++) {
		  if (ii & 7)  sbyte <<= 1;
		  else sbyte   = pgm_read_byte(bitmap + jj * byteWidth + ii / 8);
		  if (sbyte & 0x80) d->drawPixel(x + ii, y + jj, color);
		}
	  }
	}

bool NumberPad::Pressed(BUTTON *temp) {
  bool found = false;
  bool redraw = true;
  bool redrawoff = false;


	if (!decstate && (temp->ascii == 0x2E)){
		return false;
	}

	if (!negstate && (temp->ascii == 0x2D)){
		return false;
	}

	if (!rangeOK && (temp->ascii == 0x01)){
		return false;
	}

  if ((BtnX > temp->x) && (BtnX < (temp->x + temp->w))) {
    if ((BtnY > temp->y) && (BtnY < (temp->y + temp->h))) {
delay(50);
	if (clickpin > 0) {
		//analogWrite(clickpin, 60);
		//delay(5);
		//analogWrite(clickpin, 0);
	}


      while (t->touched()) {

        if (((BtnX > temp->x) && (BtnX < (temp->x + temp->w))) && ((BtnY > temp->y) && (BtnY < (temp->y + temp->h)))) {
          if (redraw) {
            DrawButton(temp, BUTTON_PRESSED);
            redraw = false;
            redrawoff = true;
          }
          found = true;
        } else {
          if (redrawoff) {
            DrawButton(temp, BUTTON_RELEASED);
            redrawoff = false;
          }
          found = false;
          redraw = true;
        }

        ProcessTouch();
      }

      DrawButton(temp, BUTTON_RELEASED);
      return found;
    }
  }
  return false;
}


void NumberPad::ProcessTouch() {
  if (t->touched()) {

    p = t->getPoint();
    BtnX = p.x;
    BtnY = p.y;

#ifdef debug
    Serial.print("real coordinates:");
    Serial.print(BtnX);
    Serial.print(" ,");
    Serial.println(BtnY);
#endif

    //different displays may require reversing last 2 args
    BtnX = map(BtnX, screenX0, screenX320, 320, 0);
    BtnY = map(BtnY, screenY0, screenY240, 240, 0);
    //d->fillCircle(BtnX, BtnY,2, 255);
#ifdef debug
    Serial.print(" , Mapped coordinates:");
    Serial.print(BtnX);
    Serial.print(" ,");
    Serial.println(BtnY);
#endif
  }
}








Keyboard::Keyboard(ILI9341_t3 *Display, XPT2046_Touchscreen *Touch) {
  d = Display;
  t = Touch;
}
void Keyboard::init(uint16_t BackColor, uint16_t TextColor, uint16_t ButtonColor, uint16_t BorderColor, uint16_t PressedTextColor, uint16_t PressedButtonColor, uint16_t PressedBorderColor, const ILI9341_t3_font_t &ButtonFont) {
  kcolor = BackColor;
  tcolor = TextColor;
  bcolor = ButtonColor;
  rcolor = BorderColor;
  ptextcolor = PressedTextColor;
  inputt = BackColor;
  inputb = TextColor;
  bfont = ButtonFont;
    
}
void Keyboard::setDisplayColor(uint16_t TextColor, uint16_t BackColor) {
  inputt = TextColor;
  inputb = BackColor;
}

void Keyboard::getInput() {
	
  uint8_t np = 0;
  uint16_t b = 0, i = 0;
  bool CapsLock = false;
  bool SpecialChar = false;
  bool KeepIn = true;
  
    memset(dn,'\0',MAX_KEYBOARD_CHARS+1);
  memset(hc,'\0',MAX_KEYBOARD_CHARS+1);
  
    // get the decimals
	if (strlen(data) > 0){
		strcpy(dn,data);
		np = strlen(dn); // account for possible sign
	}
	
  // https:javl.github.io image2cpp 
  Button KeyboardBtn[102]{
  { d },
  { d },
  { d },
  { d },
  { d },
  { d },
  { d },
  { d },
  { d },
  { d },
  { d },
  { d },
  { d },
  { d },
  { d },
  { d },
  { d },
  { d },
  { d },
  { d },
  { d },
  { d },
  { d },
  { d },
  { d },
  { d },
  { d },
  { d },
  { d },
  { d },
  { d },
  { d },
    { d },
  { d },
  { d },
  { d },
  { d },
  { d },
  { d },
  { d },
  { d },
  { d },
  { d },
  { d },
  { d },
  { d },
  { d },
  { d },
  { d },
  { d },
  { d },
  { d },
  { d },
  { d },
  { d },
  { d },
  { d },
  { d },
  { d },
  { d },
  { d },
  { d },
  { d },
  { d },
    { d },
  { d },
  { d },
  { d },
  { d },
  { d },
  { d },
  { d },
  { d },
  { d },
  { d },
  { d },
  { d },
  { d },
  { d },
  { d },
  { d },
  { d },
  { d },
  { d },
  { d },
  { d },
  { d },
  { d },
  { d },
  { d },
  { d },
  { d },
  { d },
  { d },
  { d },
  { d },
  { d },
  { d },
  { d },
  { d },
  { d },
  { d }
};  
  
  //numbers
  KeyboardBtn[16].init(COL1, ROW1, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "1", 0, 0, bfont);
  KeyboardBtn[17].init(COL2, ROW1, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "2", 0, 0, bfont);
  KeyboardBtn[18].init(COL3, ROW1, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "3", 0, 0, bfont);
  KeyboardBtn[19].init(COL4, ROW1, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "4", 0, 0, bfont);
  KeyboardBtn[20].init(COL5, ROW1, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "5", 0, 0, bfont);
  KeyboardBtn[21].init(COL6, ROW1, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "6", 0, 0, bfont);
  KeyboardBtn[22].init(COL7, ROW1, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "7", 0, 0, bfont);
  KeyboardBtn[23].init(COL8, ROW1, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "8", 0, 0, bfont);
  KeyboardBtn[24].init(COL9, ROW1, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "9", 0, 0, bfont);
  KeyboardBtn[15].init(COL10, ROW1, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "0", 0, 0, bfont);
  // special chars
  KeyboardBtn[0].init(COL1, ROW1, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "!", 0, 0, bfont);
  KeyboardBtn[31].init(COL2, ROW1, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "@", 0, 0, bfont);
  KeyboardBtn[2].init(COL3, ROW1, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "#", 0, 0, bfont);
  KeyboardBtn[3].init(COL4, ROW1, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "$", 0, 0, bfont);
  KeyboardBtn[4].init(COL5, ROW1, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "%", 0, 0, bfont);
  KeyboardBtn[61].init(COL6, ROW1, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "^", 0, 0, bfont);
  KeyboardBtn[5].init(COL7, ROW1, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "&", 0, 0, bfont);
  KeyboardBtn[9].init(COL8, ROW1, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "*", 0, 0, bfont);
  KeyboardBtn[7].init(COL9, ROW1, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "(", 0, 0, bfont);
  KeyboardBtn[8].init(COL10, ROW1, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, ")", 0, 0, bfont);
  // special chars
  KeyboardBtn[63].init(COL1, ROW2, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "`", 0, 0, bfont);
  KeyboardBtn[93].init(COL2, ROW2, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "~", 0, 0, bfont);
  KeyboardBtn[12].init(COL3, ROW2, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "-", 0, 0, bfont);
  KeyboardBtn[62].init(COL4, ROW2, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "_", 0, 0, bfont);
  KeyboardBtn[28].init(COL5, ROW2, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "=", 0, 0, bfont);
  KeyboardBtn[10].init(COL6, ROW2, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, " +", 0, 0, bfont);
  KeyboardBtn[58].init(COL7, ROW2, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "[", 0, 0, bfont);
  KeyboardBtn[60].init(COL8, ROW2, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "]", 0, 0, bfont);
  KeyboardBtn[90].init(COL9, ROW2, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "{", 0, 0, bfont);
  KeyboardBtn[92].init(COL10, ROW2, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "}", 0, 0, bfont);
  KeyboardBtn[59].init(COL1, ROW3, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "\\", 0, 0, bfont);
  KeyboardBtn[91].init(COL2, ROW3, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "|", 0, 0, bfont);
  KeyboardBtn[25].init(COL3, ROW3, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, ":", 0, 0, bfont);
  KeyboardBtn[26].init(COL4, ROW3, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, ";", 0, 0, bfont);
  KeyboardBtn[1].init(COL5, ROW3, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "''", 0, 0, bfont);
  KeyboardBtn[6].init(COL6, ROW3, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "'", 0, 0, bfont);
  KeyboardBtn[11].init(COL7, ROW3, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, ",", 0, 0, bfont);
  KeyboardBtn[13].init(COL8, ROW3, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, ".", 0, 0, bfont);
  KeyboardBtn[27].init(COL9, ROW3, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "<", 0, 0, bfont);
  KeyboardBtn[29].init(COL10, ROW3, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, ">", 0, 0, bfont);
  KeyboardBtn[14].init(COL5, ROW4, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "/", 0, 0, bfont);
  KeyboardBtn[30].init(COL6, ROW4, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, " ?", 0, 0, bfont);
  
  KeyboardBtn[48].init(COL1, ROW2, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "Q", 0, 0, bfont);
  KeyboardBtn[54].init(COL2, ROW2, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "W", 0, 0, bfont);
  KeyboardBtn[36].init(COL3, ROW2, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "E", 0, 0, bfont);
  KeyboardBtn[49].init(COL4, ROW2, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "R", 0, 0, bfont);
  KeyboardBtn[51].init(COL5, ROW2, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "T", 0, 0, bfont);
  KeyboardBtn[56].init(COL6, ROW2, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "Y", 0, 0, bfont);
  KeyboardBtn[52].init(COL7, ROW2, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "U", 0, 0, bfont);
  KeyboardBtn[40].init(COL8, ROW2, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "I", 0, 0, bfont);
  KeyboardBtn[46].init(COL9, ROW2, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "O", 0, 0, bfont);
  KeyboardBtn[47].init(COL10, ROW2, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "P", 0, 0, bfont);
  KeyboardBtn[32].init(COL1, ROW3, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "A", 0, 0, bfont);
  KeyboardBtn[50].init(COL2, ROW3, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "S", 0, 0, bfont);
  KeyboardBtn[35].init(COL3, ROW3, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "D", 0, 0, bfont);
  KeyboardBtn[37].init(COL4, ROW3, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "F", 0, 0, bfont);
  KeyboardBtn[38].init(COL5, ROW3, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "G", 0, 0, bfont);
  KeyboardBtn[39].init(COL6, ROW3, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "H", 0, 0, bfont);
  KeyboardBtn[41].init(COL7, ROW3, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "J", 0, 0, bfont);
  KeyboardBtn[42].init(COL8, ROW3, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "K", 0, 0, bfont);
  KeyboardBtn[43].init(COL9, ROW3, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "L", 0, 0, bfont);
  KeyboardBtn[57].init(COL2, ROW4, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "Z", 0, 0, bfont);
  KeyboardBtn[55].init(COL3, ROW4, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "X", 0, 0, bfont);
  KeyboardBtn[34].init(COL4, ROW4, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "C", 0, 0, bfont);
  KeyboardBtn[53].init(COL5, ROW4, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "V", 0, 0, bfont);
  KeyboardBtn[33].init(COL6, ROW4, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "B", 0, 0, bfont);
  KeyboardBtn[45].init(COL7, ROW4, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "N", 0, 0, bfont);
  KeyboardBtn[44].init(COL8, ROW4, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "M", 0, 0, bfont);
  KeyboardBtn[80].init(COL1, ROW2, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "q", 0, 0, bfont);
  KeyboardBtn[86].init(COL2, ROW2, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "w", 0, 0, bfont);
  KeyboardBtn[68].init(COL3, ROW2, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "e", 0, 0, bfont);
  KeyboardBtn[81].init(COL4, ROW2, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "r", 0, 0, bfont);
  KeyboardBtn[83].init(COL5, ROW2, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "t", 0, 0, bfont);
  KeyboardBtn[88].init(COL6, ROW2, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "y", 0, 0, bfont);
  KeyboardBtn[84].init(COL7, ROW2, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "u", 0, 0, bfont);
  KeyboardBtn[72].init(COL8, ROW2, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "i", 0, 0, bfont);
  KeyboardBtn[78].init(COL9, ROW2, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "o", 0, 0, bfont);
  KeyboardBtn[79].init(COL10, ROW2, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "p", 0, 0, bfont);
  KeyboardBtn[64].init(COL1, ROW3, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "a", 0, 0, bfont);
  KeyboardBtn[82].init(COL2, ROW3, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "s", 0, 0, bfont);
  KeyboardBtn[67].init(COL3, ROW3, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "d", 0, 0, bfont);
  KeyboardBtn[69].init(COL4, ROW3, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "f", 0, 0, bfont);
  KeyboardBtn[70].init(COL5, ROW3, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "g", 0, 0, bfont);
  KeyboardBtn[71].init(COL6, ROW3, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "h", 0, 0, bfont);
  KeyboardBtn[73].init(COL7, ROW3, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "j", 0, 0, bfont);
  KeyboardBtn[74].init(COL8, ROW3, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "k", 0, 0, bfont);
  KeyboardBtn[75].init(COL9, ROW3, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "l", 0, 0, bfont);
  KeyboardBtn[89].init(COL2, ROW4, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "z", 0, 0, bfont);
  KeyboardBtn[87].init(COL3, ROW4, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "x", 0, 0, bfont);
  KeyboardBtn[66].init(COL4, ROW4, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "c", 0, 0, bfont);
  KeyboardBtn[85].init(COL5, ROW4, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "v", 0, 0, bfont);
  KeyboardBtn[65].init(COL6, ROW4, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "b", 0, 0, bfont);
  KeyboardBtn[77].init(COL7, ROW4, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "n", 0, 0, bfont);
  KeyboardBtn[76].init(COL8, ROW4, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "m", 0, 0, bfont);
  
  
  KeyboardBtn[94].init(COL5 + (BTNS /2 ), ROW5, BTNS * 4, BTNS, rcolor, bcolor, tcolor, kcolor, "Space", 0, 0, bfont);
  KeyboardBtn[95].init(COL9, ROW5, BTNS * 3, BTNS, rcolor, bcolor, tcolor, kcolor, "Caps", 0, 0, bfont);
  KeyboardBtn[96].init(COL2, ROW5, BTNS * 3, BTNS, rcolor, bcolor, tcolor, kcolor, "$%", 0, 0, bfont);
  KeyboardBtn[97].init(COL9, BTNM + (BTNS/2), BTNS * 3, BTNS, rcolor, bcolor, tcolor, kcolor, "Back", 0, 0, bfont);
  KeyboardBtn[98].init(COL3 + (BTNS / 2), ROW6, BTNS * 4, BTNS * 1.2, rcolor, bcolor, tcolor, kcolor, "Done", 0, 0, bfont);
  KeyboardBtn[99].init(COL7 + (BTNS / 2), ROW6, BTNS * 4, BTNS * 1.2, rcolor, bcolor, tcolor, kcolor, "Cancel", 0, 0, bfont);
  
  d->fillScreen(kcolor);
    
  d->fillRect(BTNM * 2, BTNM * 2, BTNM + (BTNS * 7), 30, inputb);
  d->setCursor(BTNM + 5, BTNM + 5);
  d->setFont(bfont);
  d->setTextColor(inputt, inputb);
  
	if(hasinittext){
		d->print(inittext);
	}
	else {
		d->print(dn);
	}
	
  for (i = 0; i <= 99; i++) {
    KeyboardBtn[i].setCornerRadius(3);
  }
  

  KeyboardBtn[94].draw();
  KeyboardBtn[95].draw();
  KeyboardBtn[96].draw();
  KeyboardBtn[97].draw();
  KeyboardBtn[98].draw();
  KeyboardBtn[99].draw();
  //hide number special characters

  KeyboardBtn[0].hide();
  KeyboardBtn[31].hide();
  KeyboardBtn[2].hide();
  KeyboardBtn[3].hide();
  KeyboardBtn[4].hide();
  KeyboardBtn[61].hide();
  KeyboardBtn[5].hide();
  KeyboardBtn[9].hide();
  KeyboardBtn[7].hide();
  KeyboardBtn[8].hide();
  //hide keyboard special chars
  KeyboardBtn[63].hide();
  KeyboardBtn[93].hide();
  KeyboardBtn[12].hide();
  KeyboardBtn[62].hide();
  KeyboardBtn[28].hide();
  KeyboardBtn[10].hide();
  KeyboardBtn[58].hide();
  KeyboardBtn[60].hide();
  KeyboardBtn[90].hide();
  KeyboardBtn[92].hide();
  KeyboardBtn[59].hide();
  KeyboardBtn[91].hide();
  KeyboardBtn[25].hide();
  KeyboardBtn[26].hide();
  KeyboardBtn[1].hide();
  KeyboardBtn[6].hide();
  KeyboardBtn[11].hide();
  KeyboardBtn[13].hide();
  KeyboardBtn[27].hide();
  KeyboardBtn[29].hide();
  KeyboardBtn[14].hide();
  KeyboardBtn[30].hide();
  
  //hide upper case
  for (i = 32; i < 58; i++) {
    KeyboardBtn[i].hide();
  }
  
  //draw numbers
  for (i = 15; i < 25; i++) {
    KeyboardBtn[i].show();
  }
  
  //draw lower case
  for (i = 64; i < 90; i++) {
    KeyboardBtn[i].show();
  }
  
  while (KeepIn) {
    if (t->touched()) {
      ProcessTouch();
      //go thru all the KeyboardBtn, checking if they were pressed
      for (b = 0; b <= 100; b++) {
        if (ProcessButtonPress(KeyboardBtn[b])) {
			
          if (b == 95){

            CapsLock = !CapsLock;
            KeyboardBtn[63].hide();
            KeyboardBtn[93].hide();
            KeyboardBtn[12].hide();
            KeyboardBtn[62].hide();
            KeyboardBtn[28].hide();
            KeyboardBtn[10].hide();
            KeyboardBtn[58].hide();
            KeyboardBtn[60].hide();
            KeyboardBtn[90].hide();
            KeyboardBtn[92].hide();
            KeyboardBtn[59].hide();
            KeyboardBtn[91].hide();
            KeyboardBtn[25].hide();
            KeyboardBtn[26].hide();
            KeyboardBtn[1].hide();
            KeyboardBtn[6].hide();
            KeyboardBtn[11].hide();
            KeyboardBtn[13].hide();
            KeyboardBtn[27].hide();
            KeyboardBtn[29].hide();
            KeyboardBtn[14].hide();
            KeyboardBtn[30].hide();
			
            if (CapsLock) {
			  KeyboardBtn[95].setColors(rcolor, rcolor, tcolor, kcolor, kcolor, kcolor);
              KeyboardBtn[95].draw();
			  KeyboardBtn[96].setColors(rcolor, bcolor, tcolor, kcolor, kcolor, kcolor);
              KeyboardBtn[96].draw();
              //hide numbers
              for (i = 15; i < 25; i++) {
                KeyboardBtn[i].hide();
              }
              // hide lower case
              for (i = 64; i < 90; i++) {
                KeyboardBtn[i].hide();
              }
              // show number special characters
              KeyboardBtn[0].show();
              KeyboardBtn[31].show();
              KeyboardBtn[2].show();
              KeyboardBtn[3].show();
              KeyboardBtn[4].show();
              KeyboardBtn[61].show();
              KeyboardBtn[5].show();
              KeyboardBtn[9].show();
              KeyboardBtn[7].show();
              KeyboardBtn[8].show();
              //draw upper case
              for (i = 32; i < 58; i++) {
                KeyboardBtn[i].show();
              }
            } else {
			  KeyboardBtn[95].setColors(rcolor, bcolor, tcolor, kcolor, kcolor, kcolor);
              KeyboardBtn[95].draw();
			  KeyboardBtn[96].setColors(rcolor, bcolor, tcolor, kcolor, kcolor, kcolor);
              KeyboardBtn[96].draw();
              // hide number special characters
              KeyboardBtn[0].hide();
              KeyboardBtn[31].hide();
              KeyboardBtn[2].hide();
              KeyboardBtn[3].hide();
              KeyboardBtn[4].hide();
              KeyboardBtn[61].hide();
              KeyboardBtn[5].hide();
              KeyboardBtn[9].hide();
              KeyboardBtn[7].hide();
              KeyboardBtn[8].hide();
              KeyboardBtn[8].draw();
              // hide upper case
              for (i = 32; i < 58; i++) {
                KeyboardBtn[i].hide();
              }
              //  show numbers
              for (i = 15; i < 25; i++) {
                KeyboardBtn[i].show();
                KeyboardBtn[i].draw();
              }
              // show lower case
              for (i = 64; i < 90; i++) {
                KeyboardBtn[i].show();
              }
            }
            break;
          }
          if (b == 96) {

            SpecialChar = !SpecialChar;
            if (SpecialChar) {
		
			  KeyboardBtn[96].setColors(rcolor, rcolor, tcolor, kcolor, kcolor, kcolor);
		      KeyboardBtn[96].draw();
			  
              //hide everything but special characters
              if (CapsLock) {
                //hide upper case
                for (i = 32; i < 58; i++) {
                  KeyboardBtn[i].hide();
                }
                //hide number special characters
                KeyboardBtn[0].show();
                KeyboardBtn[31].show();
                KeyboardBtn[2].show();
                KeyboardBtn[3].show();
                KeyboardBtn[4].show();
                KeyboardBtn[61].show();
                KeyboardBtn[5].show();
                KeyboardBtn[9].show();
                KeyboardBtn[7].show();
                KeyboardBtn[8].show();
                KeyboardBtn[8].show();
                KeyboardBtn[63].show();
                KeyboardBtn[93].show();
                KeyboardBtn[12].show();
                KeyboardBtn[62].show();
                KeyboardBtn[28].show();
                KeyboardBtn[10].show();
                KeyboardBtn[58].show();
                KeyboardBtn[60].show();
                KeyboardBtn[90].show();
                KeyboardBtn[92].show();
                KeyboardBtn[59].show();
                KeyboardBtn[91].show();
                KeyboardBtn[25].show();
                KeyboardBtn[26].show();
                KeyboardBtn[1].show();
                KeyboardBtn[6].show();
                KeyboardBtn[11].show();
                KeyboardBtn[13].show();
                KeyboardBtn[27].show();
                KeyboardBtn[29].show();
                KeyboardBtn[14].show();
                KeyboardBtn[30].show();
              } else {
				  
				 
                //show numbers
                for (i = 15; i < 25; i++) {
                  KeyboardBtn[i].show();
				  
                }
				
                // hide lower case
                for (i = 64; i < 90; i++) {
                  KeyboardBtn[i].hide();
                }
				
                // show number special characters
                KeyboardBtn[0].show();
                KeyboardBtn[1].show();
                KeyboardBtn[31].show();
                KeyboardBtn[2].show();
                KeyboardBtn[3].show();
                KeyboardBtn[4].show();
                KeyboardBtn[61].show();
                KeyboardBtn[5].show();
                KeyboardBtn[9].show();
                KeyboardBtn[7].show();
                KeyboardBtn[8].show();
                // show special chars hide keyboard special chars
                KeyboardBtn[63].show();
                KeyboardBtn[93].show();
                KeyboardBtn[12].show();
                KeyboardBtn[62].show();
                KeyboardBtn[28].show();
                KeyboardBtn[10].show();
                KeyboardBtn[58].show();
                KeyboardBtn[60].show();
                KeyboardBtn[90].show();
                KeyboardBtn[92].show();
                KeyboardBtn[59].show();
                KeyboardBtn[91].show();
                KeyboardBtn[25].show();
                KeyboardBtn[26].show();
                KeyboardBtn[1].show();
                KeyboardBtn[6].show();
                KeyboardBtn[11].show();
                KeyboardBtn[13].show();
                KeyboardBtn[27].show();
                KeyboardBtn[29].show();
                KeyboardBtn[14].show();
                KeyboardBtn[30].show();
              }
            } else {
			  KeyboardBtn[96].setColors(rcolor, bcolor, tcolor, kcolor, kcolor, kcolor);
              KeyboardBtn[96].draw();
              // hide no matter what
              KeyboardBtn[63].hide();
              KeyboardBtn[93].hide();
              KeyboardBtn[12].hide();
              KeyboardBtn[62].hide();
              KeyboardBtn[28].hide();
              KeyboardBtn[10].hide();
              KeyboardBtn[58].hide();
              KeyboardBtn[60].hide();
              KeyboardBtn[90].hide();
              KeyboardBtn[92].hide();
              KeyboardBtn[59].hide();
              KeyboardBtn[91].hide();
              KeyboardBtn[25].hide();
              KeyboardBtn[26].hide();
              KeyboardBtn[1].hide();
              KeyboardBtn[6].hide();
              KeyboardBtn[11].hide();
              KeyboardBtn[13].hide();
              KeyboardBtn[27].hide();
              KeyboardBtn[29].hide();
              KeyboardBtn[14].hide();
              KeyboardBtn[30].hide();
              if (CapsLock) {
                // show upper case
                for (i = 32; i < 58; i++) {
                  KeyboardBtn[i].show();
                }
              } else {
                // hide all special chars and show lower case and numbers hide number special characters
                KeyboardBtn[0].hide();
                KeyboardBtn[31].hide();
                KeyboardBtn[2].hide();
                KeyboardBtn[3].hide();
                KeyboardBtn[4].hide();
                KeyboardBtn[61].hide();
                KeyboardBtn[5].hide();
                KeyboardBtn[9].hide();
                KeyboardBtn[7].hide();
                KeyboardBtn[8].hide();
                KeyboardBtn[8].draw();
                // show numbers
                for (i = 15; i < 25; i++) {
                  KeyboardBtn[i].show();
                }
                // show lower case
                for (i = 64; i < 90; i++) {
                  KeyboardBtn[i].show();
                }
              }
              break;
            }
          }
          // valid number or character 
		  else if ((b >= 0) & (b < 93)) {
		
			  if (aclear) {

				memset(dn, '\0', sizeof(dn));

				d->fillRect(BTNM * 2, BTNM * 2, BTNM + (BTNS * 7), 30, inputb);

				aclear = false;
				np = 0;
			  }
			  
            b += 33;
            if (np >= (MAX_KEYBOARD_CHARS)) {
              break;
            }
            dn[np] = (char)b;
			hc[np] = '*';
			
            np++;
			 
            break;
          }
          else if (b == 94) {
            // space 
			dn[np] = ' ';
			hc[np] = ' ';
            np++;
            break;
          }
          else if (b == 97) {
            // back space 
			if (np > 0) {
              --np;
			  dn[np] = ' ';
			  hc[np] = ' ';
              
            }
            break;
          }
          else if (b == 98) {
            // done
			
			strcpy(data, dn);
            KeepIn = false;
			break;
          }
          else if (b == 99) {
            // cancel, just get the heck out
            KeepIn = false;
			break;
          }
        }
      }
		d->fillRect(BTNM * 2, BTNM * 2, BTNM + (BTNS * 7), 30, inputb);
		d->setCursor(BTNM + 5, BTNM + 5);
		d->setFont(bfont);
		d->setTextColor(inputt, inputb);
		if (hideinput){
			d->print(hc);
		}
		else{
			d->print(dn);
		}
    }
  }
  
}


void Keyboard::hideInput(){
	hideinput = true;
}
	
void Keyboard::setInitialText(const char *Text){
	
	uint8_t i;
	
	for (i = 0; i < (MAX_KEYBOARD_CHARS); i++){
		inittext[i] = Text[i];
	}
	hasinittext = true;
	
}


void Keyboard::ProcessTouch() {

	if (t->touched()){		  
		p = t->getPoint();
		BtnX = p.x;
		BtnY = p.y;

#ifdef debug
     Serial.print(" real coordinates:");
     Serial.print(BtnX);
     Serial.print(" ,");
     Serial.print (BtnY);
#endif

    // different displays may require reversing last 2 args
    //BtnX = map(p.x, 3975, 169, 0, 320);
   //BtnY = map(p.y, 3850, 304, 0, 240);
	
	    BtnX = map(p.x, 169, 3975, 0, 320);
    BtnY = map(p.y, 304, 3850, 0, 240);
	

#ifdef debug
     Serial.print(" , Mapped coordinates:");
     Serial.print(BtnX);
     Serial.print(" ,");
     Serial.println(BtnY);
    d->fillCircle(BtnX, BtnY, 2, ILI9341_RED);
#endif
  }
}

bool Keyboard::ProcessButtonPress(Button TheButton) {

  if (TheButton.press(BtnX, BtnY)) {

    TheButton.draw(B_PRESSED);
    while (t->touched()) {
      if (TheButton.press(BtnX, BtnY)) {
delay(50);
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
