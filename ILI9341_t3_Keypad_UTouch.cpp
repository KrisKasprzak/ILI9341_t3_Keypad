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

#include <ILI9341_t3_Keypad_UTouch.h>
#include <ILI9341_t3.h>
#include <ILI9341_t3_Controls.h>  // button library
#include <UTouch.h>


NumberPadU::NumberPadU(ILI9341_t3 *Display, UTouch *Touch) {
  d = Display;
  t = Touch;
}

void NumberPadU::init(uint16_t BackColor, uint16_t TextColor,
                  uint16_t ButtonColor, uint16_t BorderColor,
                  uint16_t PressedTextColor,
                  uint16_t PressedButtonColor, uint16_t PressedBorderColor,
                  const ILI9341_t3_font_t &ButtonFont) {

  kcolor = BackColor;
  tcolor = TextColor;
  bcolor = ButtonColor;
  rcolor = BorderColor;
  ptextcolor = PressedTextColor;
  inputt = BackColor;
  inputb = TextColor;
  value = 0.0;
  bfont = ButtonFont;
  
  // in this class we are NOT initially writing to the char[0] as it's reserved for the - sign
  // hence we need to populate it to eliminate null terminator


}

void NumberPadU::setDisplayColor(uint16_t TextColor, uint16_t BackColor) {
  inputt = TextColor;
  inputb = BackColor;
}

void NumberPadU::setLocation(uint16_t CenterWidth, uint16_t CenterHeight) {
  CW = CenterWidth;   // width center of screen
  CH = CenterHeight;  // height center of screen
}

void NumberPadU::setButtonSizes(uint16_t ButtonWidth, uint16_t ButtonHeight, uint16_t Margins, uint16_t OKButtonWidth, uint16_t OKButtonHeight) {

  BW = ButtonWidth;
  BH = ButtonHeight;
  BS = Margins;
  OKBW = OKButtonWidth;
  OKBH = OKButtonHeight;
}

void NumberPadU::enableDecimal(bool State = true) {
  decstate = State;
}

void NumberPadU::enableNegative(bool State = true) {
  negstate = State;
}

void NumberPadU::setMinMax(float MininumValue, float MaximumValue) {

  if (MininumValue == MaximumValue) {
    minmaxstate = false;
  } else {
    minmaxstate = true;
  }

  minval = MininumValue;
  maxval = MaximumValue;
}

void NumberPadU::setInitialText(const char *Text){
	
	uint8_t i;
	
	for (i = 0; i < (MAX_KEYBOARD_CHARS-1); i++){
		inittext[i] = Text[i];
	}
	hasinittext = true;
	
}

void NumberPadU::hideInput(){
	
	hideinput = true;
}

void NumberPadU::getInput() {


  uint16_t KW = (3 * BW) + (5 * BS) + OKBW;
  uint16_t KH = (4 * BH) + (6 * BS) + TBH;
  uint16_t i = 0;
  uint16_t b = 0;

  bool hasDP = false;
  uint8_t np = 1;              // digit number

  bool hasneg = false;

  bool KeepIn = true;
  float TheNumber = 0.0;
  
  memset(dn,'\0',MAX_KEYBOARD_CHARS);
  dn[0] = ' ';
  memset(hc,'\0',MAX_KEYBOARD_CHARS);
  hc[0] = ' ';

  

  // get the decimals
	if (value != 0.0){

		if (value < 0) {
			hasneg = true;
		}
		else {
			value = value * -1.0;
		}
		
		dtostrf(value, 0, 6, dn);
		
		if (!hasneg){
			value = value * -1.0;
			dn[0] =  ' ';
		}
	  
		np = strlen(dn); // account for possible sign
	}
	
  //  https://javl.github.io/image2cpp/

  // 20 x 20
  // 'check', 40x40px
  const unsigned char check[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0x80, 0x00, 0x00, 0x0f, 0xff, 0xf0, 0x00, 0x00,
    0x3f, 0xff, 0xfc, 0x00, 0x00, 0x7e, 0x00, 0x7e, 0x00, 0x01, 0xf8, 0x00, 0x1f, 0x80, 0x03, 0xe0,
    0x00, 0x07, 0x00, 0x07, 0xc0, 0x00, 0x02, 0x38, 0x07, 0x80, 0x00, 0x00, 0x7c, 0x0f, 0x00, 0x00,
    0x00, 0xfc, 0x1e, 0x00, 0x00, 0x01, 0xfe, 0x1c, 0x00, 0x00, 0x03, 0xfc, 0x3c, 0x00, 0x00, 0x07,
    0xf8, 0x38, 0x00, 0x00, 0x0f, 0xf0, 0x38, 0x00, 0x00, 0x1f, 0xe0, 0x70, 0x10, 0x00, 0x3f, 0xc6,
    0x70, 0x38, 0x00, 0x7f, 0xce, 0x70, 0x7c, 0x00, 0xff, 0x8e, 0x70, 0xfe, 0x01, 0xff, 0x0e, 0x70,
    0xff, 0x01, 0xfe, 0x0e, 0x70, 0xff, 0x83, 0xfc, 0x0e, 0x70, 0x7f, 0xc7, 0xf8, 0x0e, 0x70, 0x7f,
    0xef, 0xf0, 0x0e, 0x70, 0x3f, 0xff, 0xf0, 0x0e, 0x70, 0x1f, 0xff, 0xe0, 0x0e, 0x78, 0x0f, 0xff,
    0xc0, 0x1c, 0x38, 0x07, 0xff, 0x80, 0x1c, 0x38, 0x03, 0xff, 0x80, 0x1c, 0x1c, 0x01, 0xff, 0x00,
    0x38, 0x1e, 0x00, 0xfe, 0x00, 0x78, 0x0e, 0x00, 0x7c, 0x00, 0x70, 0x0f, 0x00, 0x1c, 0x00, 0xf0,
    0x07, 0x80, 0x00, 0x01, 0xe0, 0x03, 0xe0, 0x00, 0x07, 0xc0, 0x01, 0xf0, 0x00, 0x0f, 0x80, 0x00,
    0xfe, 0x00, 0x7f, 0x00, 0x00, 0x3f, 0xff, 0xfc, 0x00, 0x00, 0x0f, 0xff, 0xf0, 0x00, 0x00, 0x03,
    0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  };
  // 'Arrow', 20x20px
  const unsigned char arrow[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x06,
    0x00, 0x00, 0x0e, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x3f, 0xff, 0xf0, 0x7f, 0xff, 0xf0, 0x7f, 0xff,
    0xf0, 0x3f, 0xff, 0xf0, 0x1e, 0x00, 0x00, 0x0e, 0x00, 0x00, 0x06, 0x00, 0x00, 0x02, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  };
  // 'x', 40x40px
  const unsigned char cancel[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xc0, 0x00, 0x00, 0x0f, 0xff, 0xf0, 0x00, 0x00,
    0x3f, 0xff, 0xfc, 0x00, 0x00, 0xfe, 0x00, 0xff, 0x00, 0x01, 0xf8, 0x00, 0x1f, 0x80, 0x03, 0xe0,
    0x00, 0x07, 0xc0, 0x07, 0xc0, 0x00, 0x03, 0xe0, 0x0f, 0x80, 0x00, 0x01, 0xf0, 0x0f, 0x1c, 0x00,
    0x00, 0xf0, 0x1e, 0x3e, 0x00, 0x3c, 0x78, 0x1c, 0x7f, 0x00, 0x7e, 0x38, 0x3c, 0x7f, 0x00, 0xfe,
    0x3c, 0x38, 0x7f, 0x81, 0xfe, 0x1c, 0x78, 0x7f, 0xc3, 0xfe, 0x1c, 0x70, 0x3f, 0xef, 0xfc, 0x1e,
    0x70, 0x1f, 0xff, 0xf8, 0x0e, 0x70, 0x1f, 0xff, 0xe0, 0x0e, 0x70, 0x0f, 0xff, 0xc0, 0x0e, 0x70,
    0x07, 0xff, 0x80, 0x0e, 0x70, 0x03, 0xff, 0x00, 0x0e, 0x70, 0x03, 0xff, 0x00, 0x0e, 0x70, 0x07,
    0xff, 0x00, 0x0e, 0x70, 0x0f, 0xff, 0x80, 0x0e, 0x70, 0x0f, 0xff, 0xc0, 0x0e, 0x78, 0x1f, 0xff,
    0xe0, 0x1e, 0x38, 0x3f, 0xdf, 0xf0, 0x1c, 0x3c, 0x7f, 0x8f, 0xf8, 0x3c, 0x3c, 0xff, 0x07, 0xfc,
    0x38, 0x19, 0xfe, 0x03, 0xfc, 0x78, 0x0b, 0xfe, 0x01, 0xfc, 0xf0, 0x03, 0xfc, 0x00, 0xf9, 0xf0,
    0x07, 0xf8, 0x00, 0x73, 0xe0, 0x07, 0xf0, 0x00, 0x07, 0xc0, 0x03, 0xe0, 0x00, 0x1f, 0x80, 0x01,
    0xce, 0x00, 0x7f, 0x00, 0x00, 0x8f, 0xff, 0xfc, 0x00, 0x00, 0x1f, 0xff, 0xf8, 0x00, 0x00, 0x03,
    0xff, 0xc0, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00
  };

  Button NumberPadUBtn[16](d);

  //7
  NumberPadUBtn[7].init(CW - (KW / 2) + BS + (BW / 2), CH - KH / 2 + BS + BS + TBH + (BH / 2), BW, BH, rcolor, bcolor, tcolor, kcolor, "7", 0, 0, bfont);
  //8
  NumberPadUBtn[8].init(CW - (KW / 2) + 2 * BS + BW + (BW / 2), CH - KH / 2 + BS + BS + TBH + (BH / 2), BW, BH, rcolor, bcolor, tcolor, kcolor, "8", 0, 0, bfont);
  //9
  NumberPadUBtn[9].init(CW - (KW / 2) + 3 * BS + 2 * BW + (BW / 2), CH - KH / 2 + BS + BS + TBH + (BH / 2), BW, BH, rcolor, bcolor, tcolor, kcolor, "9", 0, 0, bfont);
  //4
  NumberPadUBtn[4].init(CW - (KW / 2) + BS + (BW / 2), CH - KH / 2 + 3 * BS + TBH + BH + (BH / 2), BW, BH, rcolor, bcolor, tcolor, kcolor, "4", 0, 0, bfont);
  // 5
  NumberPadUBtn[5].init(CW - (KW / 2) + 2 * BS + BW + (BW / 2), CH - KH / 2 + 3 * BS + TBH + BH + (BH / 2), BW, BH, rcolor, bcolor, tcolor, kcolor, "5", 0, 0, bfont);
  //6
  NumberPadUBtn[6].init(CW - (KW / 2) + 3 * BS + 2 * BW + (BW / 2), CH - KH / 2 + 3 * BS + TBH + BH + (BH / 2), BW, BH, rcolor, bcolor, tcolor, kcolor, "6", 0, 0, bfont);
  // 1
  NumberPadUBtn[1].init(CW - (KW / 2) + BS + (BW / 2), CH - KH / 2 + 4 * BS + TBH + 2 * BH + (BH / 2), BW, BH, rcolor, bcolor, tcolor, kcolor, "1", 0, 0, bfont);
  // 2
  NumberPadUBtn[2].init(CW - (KW / 2) + 2 * BS + BW + (BW / 2), CH - KH / 2 + 4 * BS + TBH + 2 * BH + (BH / 2), BW, BH, rcolor, bcolor, tcolor, kcolor, "2", 0, 0, bfont);
  // 3
  NumberPadUBtn[3].init(CW - (KW / 2) + 3 * BS + 2 * BW + (BW / 2), CH - KH / 2 + 4 * BS + TBH + 2 * BH + (BH / 2), BW, BH, rcolor, bcolor, tcolor, kcolor, "3", 0, 0, bfont);
  // -
  NumberPadUBtn[10].init(CW - (KW / 2) + BS + (BW / 2), CH - KH / 2 + 5 * BS + TBH + 3 * BH + (BH / 2), BW, BH, rcolor, bcolor, tcolor, kcolor, "-", 0, 0, bfont);
  // 0
  NumberPadUBtn[0].init(CW - (KW / 2) + 2 * BS + BW + (BW / 2), CH - KH / 2 + 5 * BS + TBH + 3 * BH + (BH / 2), BW, BH, rcolor, bcolor, tcolor, kcolor, "0", 0, 0, bfont);
  // .
  NumberPadUBtn[11].init(CW - (KW / 2) + 3 * BS + 2 * BW + (BW / 2), CH - KH / 2 + 5 * BS + TBH + 3 * BH + (BH / 2), BW, BH, rcolor, bcolor, tcolor, kcolor, ".", 0, 0, bfont);
  // backspace
  NumberPadUBtn[12].init(CW + (KW / 2) - BS - (OKBW / 2), CH - (KH / 2) + BS + (TBH / 2), OKBW, TBH, rcolor, bcolor, tcolor, kcolor, arrow, 20, 20, (OKBW - 20) / 2, (TBH - 20) / 5);
  // done
  NumberPadUBtn[13].init(CW + (KW / 2) - BS - (OKBW / 2), CH + (KH / 2) - BS - (OKBH / 2), OKBW, OKBH, rcolor, bcolor, ILI9341_GREEN, kcolor, check, 40, 40, (OKBW - 40) / 2, (OKBH - 40) / 2);
  // cancel
  NumberPadUBtn[14].init(CW + (KW / 2) - BS - (OKBW / 2), CH + KH / 2 - 2 * BS - OKBH - (OKBH / 2), OKBW, OKBH, rcolor, bcolor, ILI9341_RED, kcolor, cancel, 40, 40, (OKBW - 40) / 2, (OKBH - 40) / 2);


  // large background box
  d->fillRect(CW - (KW / 2), CH - KH / 2, KW, KH, kcolor);

  // text input box
  d->fillRect(CW - (KW / 2) + BS, CH - KH / 2 + BS, 2 * BS + 3 * BW, TBH, inputb);
  d->setCursor(CW - (KW / 2) + BS + 5, CH - KH / 2 + BS + 5);
  d->setFont(bfont);
  d->setTextColor(inputt, inputb);
  if (hasinittext){
	d->print(inittext);
  }
    else {
	d->print(dn);
  }
  
  for (i = 0; i <= 14; i++) {
    NumberPadUBtn[i].setCornerRadius(3);
  }
  NumberPadUBtn[0].draw();
  NumberPadUBtn[1].draw();
  NumberPadUBtn[2].draw();
  NumberPadUBtn[3].draw();
  NumberPadUBtn[4].draw();
  NumberPadUBtn[5].draw();
  NumberPadUBtn[6].draw();
  NumberPadUBtn[7].draw();
  NumberPadUBtn[8].draw();
  NumberPadUBtn[9].draw();
  if (negstate) { NumberPadUBtn[10].draw(); }
  if (decstate) { NumberPadUBtn[11].draw(); }
  NumberPadUBtn[12].draw();
  NumberPadUBtn[13].draw();
  NumberPadUBtn[14].draw();
  
  while (KeepIn) {
	  
    if (t->dataAvailable()) {
		
      ProcessTouch();
      //go thru all the NumberPadUBtn, checking if they were pressed
      for (b = 0; b <= 14; b++) {
		  	  
        if (ProcessButtonPress(NumberPadUBtn[b])) {
		
          //valid number
          if ((b >= 0) & (b <= 9)) {
            if (np > 10) { 
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
          } else if (b == 10) {
            //negative number
            if (dn[0] == '-') {
              dn[0] = ' ';
            } else {
              dn[0] = '-';
            }
          } else if (b == 11) {
            // decimal point
            if (!hasDP) {
              dn[np] = '.';
			  hc[np] = '*';
              hasDP = true;
              np++;
            }
          } else if (b == 12) {
            // back space
            if (np > 1) {
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
        if (minmaxstate) {
          TheNumber = atof(dn);
          // check min bounds
          if (TheNumber < minval) {
            // back out last entry
            np--;
            dn[np] = ' ';
          }
          // check max bounds
          if (TheNumber > maxval) {
            // back out last entry
            np--;
            dn[np] = ' ';
          }
        }
      }
	  d->fillRect(CW - (KW / 2) + BS, CH - KH / 2 + BS, 2 * BS + 3 * BW, TBH, inputb);
	  d->setCursor(CW - (KW / 2) + BS + 5, CH - KH / 2 + BS + 5);
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

void NumberPadU::ProcessTouch() {
	if(t->dataAvailable()){
		t->read();
		BtnX = t->getX();
		BtnY = t->getY();
	
#ifdef debug 
	Serial.print("real coordinates:");
    Serial.print(BtnX);
    Serial.print(" ,");
    Serial.print(BtnY);
#endif 

//different displays may require reversing last 2 args 
      BtnX = map(BtnX, 0, 230, 0, 320);
      BtnY = map(BtnY, 28, 390, 0, 240);
	
#ifdef debug 
	Serial.print(" , Mapped coordinates:");
    Serial.print(BtnX);
    Serial.print(" ,");
    Serial.println(BtnY);
    d->fillCircle(BtnX, BtnY,2, ILI9341_RED);
#endif
  }
}


bool NumberPadU::ProcessButtonPress(Button TheButton) {
  if (TheButton.press(BtnX, BtnY)) {
    TheButton.draw(B_PRESSED);
    while (t->dataAvailable()) {
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








KeyboardU::KeyboardU(ILI9341_t3 *Display, UTouch *Touch) {
  d = Display;
  t = Touch;
}
void KeyboardU::init(uint16_t BackColor, uint16_t TextColor, uint16_t ButtonColor, uint16_t BorderColor, uint16_t PressedTextColor, uint16_t PressedButtonColor, uint16_t PressedBorderColor, const ILI9341_t3_font_t &ButtonFont) {
  kcolor = BackColor;
  tcolor = TextColor;
  bcolor = ButtonColor;
  rcolor = BorderColor;
  ptextcolor = PressedTextColor;
  inputt = BackColor;
  inputb = TextColor;
  bfont = ButtonFont;
    
}
void KeyboardU::setDisplayColor(uint16_t TextColor, uint16_t BackColor) {
  inputt = TextColor;
  inputb = BackColor;
}

void KeyboardU::getInput() {
	
  uint8_t np = 0;
  uint16_t b = 0, i = 0;
  bool CapsLock = false;
  bool SpecialChar = false;
  bool KeepIn = true;
  
    memset(dn,'\0',MAX_KEYBOARD_CHARS);
  memset(hc,'\0',MAX_KEYBOARD_CHARS);
  
    // get the decimals
	if (strlen(data) > 0){
		strcpy(dn,data);
		np = strlen(dn); // account for possible sign
	}
	
  // https:javl.github.io image2cpp 
  
  Button KeyboardUBtn[101](d);
  
  //numbers
  KeyboardUBtn[16].init(COL1, ROW1, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "1", 0, 0, bfont);
  KeyboardUBtn[17].init(COL2, ROW1, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "2", 0, 0, bfont);
  KeyboardUBtn[18].init(COL3, ROW1, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "3", 0, 0, bfont);
  KeyboardUBtn[19].init(COL4, ROW1, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "4", 0, 0, bfont);
  KeyboardUBtn[20].init(COL5, ROW1, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "5", 0, 0, bfont);
  KeyboardUBtn[21].init(COL6, ROW1, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "6", 0, 0, bfont);
  KeyboardUBtn[22].init(COL7, ROW1, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "7", 0, 0, bfont);
  KeyboardUBtn[23].init(COL8, ROW1, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "8", 0, 0, bfont);
  KeyboardUBtn[24].init(COL9, ROW1, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "9", 0, 0, bfont);
  KeyboardUBtn[15].init(COL10, ROW1, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "0", 0, 0, bfont);
  // special chars
  KeyboardUBtn[0].init(COL1, ROW1, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "!", 0, 0, bfont);
  KeyboardUBtn[31].init(COL2, ROW1, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "@", 0, 0, bfont);
  KeyboardUBtn[2].init(COL3, ROW1, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "#", 0, 0, bfont);
  KeyboardUBtn[3].init(COL4, ROW1, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "$", 0, 0, bfont);
  KeyboardUBtn[4].init(COL5, ROW1, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "%", 0, 0, bfont);
  KeyboardUBtn[61].init(COL6, ROW1, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "^", 0, 0, bfont);
  KeyboardUBtn[5].init(COL7, ROW1, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "&", 0, 0, bfont);
  KeyboardUBtn[9].init(COL8, ROW1, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "*", 0, 0, bfont);
  KeyboardUBtn[7].init(COL9, ROW1, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "(", 0, 0, bfont);
  KeyboardUBtn[8].init(COL10, ROW1, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, ")", 0, 0, bfont);
  // special chars
  KeyboardUBtn[63].init(COL1, ROW2, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "`", 0, 0, bfont);
  KeyboardUBtn[93].init(COL2, ROW2, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "~", 0, 0, bfont);
  KeyboardUBtn[12].init(COL3, ROW2, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "-", 0, 0, bfont);
  KeyboardUBtn[62].init(COL4, ROW2, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "_", 0, 0, bfont);
  KeyboardUBtn[28].init(COL5, ROW2, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "=", 0, 0, bfont);
  KeyboardUBtn[10].init(COL6, ROW2, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, " +", 0, 0, bfont);
  KeyboardUBtn[58].init(COL7, ROW2, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "[", 0, 0, bfont);
  KeyboardUBtn[60].init(COL8, ROW2, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "]", 0, 0, bfont);
  KeyboardUBtn[90].init(COL9, ROW2, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "{", 0, 0, bfont);
  KeyboardUBtn[92].init(COL10, ROW2, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "}", 0, 0, bfont);
  KeyboardUBtn[59].init(COL1, ROW3, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "\\", 0, 0, bfont);
  KeyboardUBtn[91].init(COL2, ROW3, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "|", 0, 0, bfont);
  KeyboardUBtn[25].init(COL3, ROW3, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, ":", 0, 0, bfont);
  KeyboardUBtn[26].init(COL4, ROW3, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, ";", 0, 0, bfont);
  KeyboardUBtn[1].init(COL5, ROW3, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "''", 0, 0, bfont);
  KeyboardUBtn[6].init(COL6, ROW3, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "'", 0, 0, bfont);
  KeyboardUBtn[11].init(COL7, ROW3, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, ",", 0, 0, bfont);
  KeyboardUBtn[13].init(COL8, ROW3, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, ".", 0, 0, bfont);
  KeyboardUBtn[27].init(COL9, ROW3, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "<", 0, 0, bfont);
  KeyboardUBtn[29].init(COL10, ROW3, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, ">", 0, 0, bfont);
  KeyboardUBtn[14].init(COL5, ROW4, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "/", 0, 0, bfont);
  KeyboardUBtn[30].init(COL6, ROW4, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, " ?", 0, 0, bfont);
  
  KeyboardUBtn[48].init(COL1, ROW2, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "Q", 0, 0, bfont);
  KeyboardUBtn[54].init(COL2, ROW2, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "W", 0, 0, bfont);
  KeyboardUBtn[36].init(COL3, ROW2, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "E", 0, 0, bfont);
  KeyboardUBtn[49].init(COL4, ROW2, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "R", 0, 0, bfont);
  KeyboardUBtn[51].init(COL5, ROW2, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "T", 0, 0, bfont);
  KeyboardUBtn[56].init(COL6, ROW2, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "Y", 0, 0, bfont);
  KeyboardUBtn[52].init(COL7, ROW2, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "U", 0, 0, bfont);
  KeyboardUBtn[40].init(COL8, ROW2, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "I", 0, 0, bfont);
  KeyboardUBtn[46].init(COL9, ROW2, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "O", 0, 0, bfont);
  KeyboardUBtn[47].init(COL10, ROW2, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "P", 0, 0, bfont);
  KeyboardUBtn[32].init(COL1, ROW3, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "A", 0, 0, bfont);
  KeyboardUBtn[50].init(COL2, ROW3, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "S", 0, 0, bfont);
  KeyboardUBtn[35].init(COL3, ROW3, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "D", 0, 0, bfont);
  KeyboardUBtn[37].init(COL4, ROW3, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "F", 0, 0, bfont);
  KeyboardUBtn[38].init(COL5, ROW3, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "G", 0, 0, bfont);
  KeyboardUBtn[39].init(COL6, ROW3, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "H", 0, 0, bfont);
  KeyboardUBtn[41].init(COL7, ROW3, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "J", 0, 0, bfont);
  KeyboardUBtn[42].init(COL8, ROW3, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "K", 0, 0, bfont);
  KeyboardUBtn[43].init(COL9, ROW3, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "L", 0, 0, bfont);
  KeyboardUBtn[57].init(COL2, ROW4, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "Z", 0, 0, bfont);
  KeyboardUBtn[55].init(COL3, ROW4, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "X", 0, 0, bfont);
  KeyboardUBtn[34].init(COL4, ROW4, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "C", 0, 0, bfont);
  KeyboardUBtn[53].init(COL5, ROW4, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "V", 0, 0, bfont);
  KeyboardUBtn[33].init(COL6, ROW4, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "B", 0, 0, bfont);
  KeyboardUBtn[45].init(COL7, ROW4, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "N", 0, 0, bfont);
  KeyboardUBtn[44].init(COL8, ROW4, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "M", 0, 0, bfont);
  KeyboardUBtn[80].init(COL1, ROW2, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "q", 0, 0, bfont);
  KeyboardUBtn[86].init(COL2, ROW2, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "w", 0, 0, bfont);
  KeyboardUBtn[68].init(COL3, ROW2, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "e", 0, 0, bfont);
  KeyboardUBtn[81].init(COL4, ROW2, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "r", 0, 0, bfont);
  KeyboardUBtn[83].init(COL5, ROW2, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "t", 0, 0, bfont);
  KeyboardUBtn[88].init(COL6, ROW2, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "y", 0, 0, bfont);
  KeyboardUBtn[84].init(COL7, ROW2, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "u", 0, 0, bfont);
  KeyboardUBtn[72].init(COL8, ROW2, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "i", 0, 0, bfont);
  KeyboardUBtn[78].init(COL9, ROW2, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "o", 0, 0, bfont);
  KeyboardUBtn[79].init(COL10, ROW2, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "p", 0, 0, bfont);
  KeyboardUBtn[64].init(COL1, ROW3, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "a", 0, 0, bfont);
  KeyboardUBtn[82].init(COL2, ROW3, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "s", 0, 0, bfont);
  KeyboardUBtn[67].init(COL3, ROW3, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "d", 0, 0, bfont);
  KeyboardUBtn[69].init(COL4, ROW3, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "f", 0, 0, bfont);
  KeyboardUBtn[70].init(COL5, ROW3, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "g", 0, 0, bfont);
  KeyboardUBtn[71].init(COL6, ROW3, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "h", 0, 0, bfont);
  KeyboardUBtn[73].init(COL7, ROW3, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "j", 0, 0, bfont);
  KeyboardUBtn[74].init(COL8, ROW3, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "k", 0, 0, bfont);
  KeyboardUBtn[75].init(COL9, ROW3, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "l", 0, 0, bfont);
  KeyboardUBtn[89].init(COL2, ROW4, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "z", 0, 0, bfont);
  KeyboardUBtn[87].init(COL3, ROW4, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "x", 0, 0, bfont);
  KeyboardUBtn[66].init(COL4, ROW4, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "c", 0, 0, bfont);
  KeyboardUBtn[85].init(COL5, ROW4, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "v", 0, 0, bfont);
  KeyboardUBtn[65].init(COL6, ROW4, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "b", 0, 0, bfont);
  KeyboardUBtn[77].init(COL7, ROW4, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "n", 0, 0, bfont);
  KeyboardUBtn[76].init(COL8, ROW4, BTNS, BTNS, rcolor, bcolor, tcolor, kcolor, "m", 0, 0, bfont);
  
  
  KeyboardUBtn[94].init(COL5 + (BTNS /2 ), ROW5, BTNS * 4, BTNS, rcolor, bcolor, tcolor, kcolor, "Space", 0, 0, bfont);
  KeyboardUBtn[95].init(COL9, ROW5, BTNS * 3, BTNS, rcolor, bcolor, tcolor, kcolor, "Caps", 0, 0, bfont);
  KeyboardUBtn[96].init(COL2, ROW5, BTNS * 3, BTNS, rcolor, bcolor, tcolor, kcolor, "$%", 0, 0, bfont);
  KeyboardUBtn[97].init(COL9, BTNM + (BTNS/2), BTNS * 3, BTNS, rcolor, bcolor, tcolor, kcolor, "Back", 0, 0, bfont);
  KeyboardUBtn[98].init(COL3 + (BTNS / 2), ROW6, BTNS * 4, BTNS * 1.2, rcolor, bcolor, tcolor, kcolor, "Done", 0, 0, bfont);
  KeyboardUBtn[99].init(COL7 + (BTNS / 2), ROW6, BTNS * 4, BTNS * 1.2, rcolor, bcolor, tcolor, kcolor, "Cancel", 0, 0, bfont);
  
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
    KeyboardUBtn[i].setCornerRadius(3);
  }
  

  KeyboardUBtn[94].draw();
  KeyboardUBtn[95].draw();
  KeyboardUBtn[96].draw();
  KeyboardUBtn[97].draw();
  KeyboardUBtn[98].draw();
  KeyboardUBtn[99].draw();
  //hide number special characters

  KeyboardUBtn[0].hide();
  KeyboardUBtn[31].hide();
  KeyboardUBtn[2].hide();
  KeyboardUBtn[3].hide();
  KeyboardUBtn[4].hide();
  KeyboardUBtn[61].hide();
  KeyboardUBtn[5].hide();
  KeyboardUBtn[9].hide();
  KeyboardUBtn[7].hide();
  KeyboardUBtn[8].hide();
  //hide KeyboardU special chars
  KeyboardUBtn[63].hide();
  KeyboardUBtn[93].hide();
  KeyboardUBtn[12].hide();
  KeyboardUBtn[62].hide();
  KeyboardUBtn[28].hide();
  KeyboardUBtn[10].hide();
  KeyboardUBtn[58].hide();
  KeyboardUBtn[60].hide();
  KeyboardUBtn[90].hide();
  KeyboardUBtn[92].hide();
  KeyboardUBtn[59].hide();
  KeyboardUBtn[91].hide();
  KeyboardUBtn[25].hide();
  KeyboardUBtn[26].hide();
  KeyboardUBtn[1].hide();
  KeyboardUBtn[6].hide();
  KeyboardUBtn[11].hide();
  KeyboardUBtn[13].hide();
  KeyboardUBtn[27].hide();
  KeyboardUBtn[29].hide();
  KeyboardUBtn[14].hide();
  KeyboardUBtn[30].hide();
  
  //hide upper case
  for (i = 32; i < 58; i++) {
    KeyboardUBtn[i].hide();
  }
  
  //draw numbers
  for (i = 15; i < 25; i++) {
    KeyboardUBtn[i].show();
  }
  
  //draw lower case
  for (i = 64; i < 90; i++) {
    KeyboardUBtn[i].show();
  }
  
  while (KeepIn) {
    if (t->dataAvailable()) {
      ProcessTouch();
      //go thru all the KeyboardUBtn, checking if they were pressed
      for (b = 0; b <= 100; b++) {
        if (ProcessButtonPress(KeyboardUBtn[b])) {
			
          if (b == 95){

            CapsLock = !CapsLock;
            KeyboardUBtn[63].hide();
            KeyboardUBtn[93].hide();
            KeyboardUBtn[12].hide();
            KeyboardUBtn[62].hide();
            KeyboardUBtn[28].hide();
            KeyboardUBtn[10].hide();
            KeyboardUBtn[58].hide();
            KeyboardUBtn[60].hide();
            KeyboardUBtn[90].hide();
            KeyboardUBtn[92].hide();
            KeyboardUBtn[59].hide();
            KeyboardUBtn[91].hide();
            KeyboardUBtn[25].hide();
            KeyboardUBtn[26].hide();
            KeyboardUBtn[1].hide();
            KeyboardUBtn[6].hide();
            KeyboardUBtn[11].hide();
            KeyboardUBtn[13].hide();
            KeyboardUBtn[27].hide();
            KeyboardUBtn[29].hide();
            KeyboardUBtn[14].hide();
            KeyboardUBtn[30].hide();
			
            if (CapsLock) {
			  KeyboardUBtn[95].setColors(rcolor, rcolor, tcolor, kcolor, kcolor, kcolor);
              KeyboardUBtn[95].draw();
			  KeyboardUBtn[96].setColors(rcolor, bcolor, tcolor, kcolor, kcolor, kcolor);
              KeyboardUBtn[96].draw();
              //hide numbers
              for (i = 15; i < 25; i++) {
                KeyboardUBtn[i].hide();
              }
              // hide lower case
              for (i = 64; i < 90; i++) {
                KeyboardUBtn[i].hide();
              }
              // show number special characters
              KeyboardUBtn[0].show();
              KeyboardUBtn[31].show();
              KeyboardUBtn[2].show();
              KeyboardUBtn[3].show();
              KeyboardUBtn[4].show();
              KeyboardUBtn[61].show();
              KeyboardUBtn[5].show();
              KeyboardUBtn[9].show();
              KeyboardUBtn[7].show();
              KeyboardUBtn[8].show();
              //draw upper case
              for (i = 32; i < 58; i++) {
                KeyboardUBtn[i].show();
              }
            } else {
			  KeyboardUBtn[95].setColors(rcolor, bcolor, tcolor, kcolor, kcolor, kcolor);
              KeyboardUBtn[95].draw();
			  KeyboardUBtn[96].setColors(rcolor, bcolor, tcolor, kcolor, kcolor, kcolor);
              KeyboardUBtn[96].draw();
              // hide number special characters
              KeyboardUBtn[0].hide();
              KeyboardUBtn[31].hide();
              KeyboardUBtn[2].hide();
              KeyboardUBtn[3].hide();
              KeyboardUBtn[4].hide();
              KeyboardUBtn[61].hide();
              KeyboardUBtn[5].hide();
              KeyboardUBtn[9].hide();
              KeyboardUBtn[7].hide();
              KeyboardUBtn[8].hide();
              KeyboardUBtn[8].draw();
              // hide upper case
              for (i = 32; i < 58; i++) {
                KeyboardUBtn[i].hide();
              }
              //  show numbers
              for (i = 15; i < 25; i++) {
                KeyboardUBtn[i].show();
                KeyboardUBtn[i].draw();
              }
              // show lower case
              for (i = 64; i < 90; i++) {
                KeyboardUBtn[i].show();
              }
            }
            break;
          }
          if (b == 96) {

            SpecialChar = !SpecialChar;
            if (SpecialChar) {
		
			  KeyboardUBtn[96].setColors(rcolor, rcolor, tcolor, kcolor, kcolor, kcolor);
		      KeyboardUBtn[96].draw();
			  
              //hide everything but special characters
              if (CapsLock) {
                //hide upper case
                for (i = 32; i < 58; i++) {
                  KeyboardUBtn[i].hide();
                }
                //hide number special characters
                KeyboardUBtn[0].show();
                KeyboardUBtn[31].show();
                KeyboardUBtn[2].show();
                KeyboardUBtn[3].show();
                KeyboardUBtn[4].show();
                KeyboardUBtn[61].show();
                KeyboardUBtn[5].show();
                KeyboardUBtn[9].show();
                KeyboardUBtn[7].show();
                KeyboardUBtn[8].show();
                KeyboardUBtn[8].show();
                KeyboardUBtn[63].show();
                KeyboardUBtn[93].show();
                KeyboardUBtn[12].show();
                KeyboardUBtn[62].show();
                KeyboardUBtn[28].show();
                KeyboardUBtn[10].show();
                KeyboardUBtn[58].show();
                KeyboardUBtn[60].show();
                KeyboardUBtn[90].show();
                KeyboardUBtn[92].show();
                KeyboardUBtn[59].show();
                KeyboardUBtn[91].show();
                KeyboardUBtn[25].show();
                KeyboardUBtn[26].show();
                KeyboardUBtn[1].show();
                KeyboardUBtn[6].show();
                KeyboardUBtn[11].show();
                KeyboardUBtn[13].show();
                KeyboardUBtn[27].show();
                KeyboardUBtn[29].show();
                KeyboardUBtn[14].show();
                KeyboardUBtn[30].show();
              } else {
				  
				 
                //show numbers
                for (i = 15; i < 25; i++) {
                  KeyboardUBtn[i].show();
				  
                }
				
                // hide lower case
                for (i = 64; i < 90; i++) {
                  KeyboardUBtn[i].hide();
                }
				
                // show number special characters
                KeyboardUBtn[0].show();
                KeyboardUBtn[1].show();
                KeyboardUBtn[31].show();
                KeyboardUBtn[2].show();
                KeyboardUBtn[3].show();
                KeyboardUBtn[4].show();
                KeyboardUBtn[61].show();
                KeyboardUBtn[5].show();
                KeyboardUBtn[9].show();
                KeyboardUBtn[7].show();
                KeyboardUBtn[8].show();
                // show special chars hide KeyboardU special chars
                KeyboardUBtn[63].show();
                KeyboardUBtn[93].show();
                KeyboardUBtn[12].show();
                KeyboardUBtn[62].show();
                KeyboardUBtn[28].show();
                KeyboardUBtn[10].show();
                KeyboardUBtn[58].show();
                KeyboardUBtn[60].show();
                KeyboardUBtn[90].show();
                KeyboardUBtn[92].show();
                KeyboardUBtn[59].show();
                KeyboardUBtn[91].show();
                KeyboardUBtn[25].show();
                KeyboardUBtn[26].show();
                KeyboardUBtn[1].show();
                KeyboardUBtn[6].show();
                KeyboardUBtn[11].show();
                KeyboardUBtn[13].show();
                KeyboardUBtn[27].show();
                KeyboardUBtn[29].show();
                KeyboardUBtn[14].show();
                KeyboardUBtn[30].show();
              }
            } else {
			  KeyboardUBtn[96].setColors(rcolor, bcolor, tcolor, kcolor, kcolor, kcolor);
              KeyboardUBtn[96].draw();
              // hide no matter what
              KeyboardUBtn[63].hide();
              KeyboardUBtn[93].hide();
              KeyboardUBtn[12].hide();
              KeyboardUBtn[62].hide();
              KeyboardUBtn[28].hide();
              KeyboardUBtn[10].hide();
              KeyboardUBtn[58].hide();
              KeyboardUBtn[60].hide();
              KeyboardUBtn[90].hide();
              KeyboardUBtn[92].hide();
              KeyboardUBtn[59].hide();
              KeyboardUBtn[91].hide();
              KeyboardUBtn[25].hide();
              KeyboardUBtn[26].hide();
              KeyboardUBtn[1].hide();
              KeyboardUBtn[6].hide();
              KeyboardUBtn[11].hide();
              KeyboardUBtn[13].hide();
              KeyboardUBtn[27].hide();
              KeyboardUBtn[29].hide();
              KeyboardUBtn[14].hide();
              KeyboardUBtn[30].hide();
              if (CapsLock) {
                // show upper case
                for (i = 32; i < 58; i++) {
                  KeyboardUBtn[i].show();
                }
              } else {
                // hide all special chars and show lower case and numbers hide number special characters
                KeyboardUBtn[0].hide();
                KeyboardUBtn[31].hide();
                KeyboardUBtn[2].hide();
                KeyboardUBtn[3].hide();
                KeyboardUBtn[4].hide();
                KeyboardUBtn[61].hide();
                KeyboardUBtn[5].hide();
                KeyboardUBtn[9].hide();
                KeyboardUBtn[7].hide();
                KeyboardUBtn[8].hide();
                KeyboardUBtn[8].draw();
                // show numbers
                for (i = 15; i < 25; i++) {
                  KeyboardUBtn[i].show();
                }
                // show lower case
                for (i = 64; i < 90; i++) {
                  KeyboardUBtn[i].show();
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


void KeyboardU::hideInput(){
	hideinput = true;
}
	
void KeyboardU::setInitialText(const char *Text){
	
	uint8_t i;
	
	for (i = 0; i < (MAX_KEYBOARD_CHARS-1); i++){
		inittext[i] = Text[i];
	}
	hasinittext = true;
	
}


void KeyboardU::ProcessTouch() {

	if(t->dataAvailable()){
		t->read();
		BtnX = t->getX();
		BtnY = t->getY();
			
#ifdef debug
     Serial.print(" real coordinates:");
     Serial.print(BtnX);
     Serial.print(" ,");
     Serial.print (BtnY);
#endif

    // different displays may require reversing last 2 args
      BtnX = map(BtnX, 0, 230, 0, 320);
      BtnY = map(BtnY, 28, 390, 0, 240);

#ifdef debug
     Serial.print(" , Mapped coordinates:");
     Serial.print(BtnX);
     Serial.print(" ,");
     Serial.println(BtnY);
    d->fillCircle(BtnX, BtnY, 2, ILI9341_RED);
#endif
  }
}

bool KeyboardU::ProcessButtonPress(Button TheButton) {

  if (TheButton.press(BtnX, BtnY)) {

    TheButton.draw(B_PRESSED);
    while (t->dataAvailable()) {
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