/*
  Row-Column Scanning an 3x3 LED matrix with X-Y input
 
 This example controls an 3x3 LED matrix using two analog inputs
 
 created 27 May 2009
 modified 30 Aug 2011
 by Tom Igoe
 
 This example works for the Lumex  LDM-24433NI Matrix. See 
 http://sigma.octopart.com/140413/datasheet/Lumex-LDM-24433NI.pdf
 for the pin connections
 
 For other LED cathode column matrixes, you should only need to change 
 the pin numbers in the row[] and column[] arrays
 
 rows are the anodes
 cols are the cathodes
 ---------
 
 Pin numbers:
 Matrix:
 * Digital pins 2 through 13,
 * analog pins 2 through 5 used as digital 16 through 19
 Potentiometers:
 * center pins are attached to analog pins 0 and 1, respectively
 * side pins attached to +5V and ground, respectively.
 
 This example code is in the public domain.
 
 http://www.arduino.cc/en/Tutorial/RowColumnScanning
 
 see also http://www.tigoe.net/pcomp/code/category/arduinowiring/514 for more
 */

#define FIRST { \
    {0, 0, 0},  \
    {0, 1, 0}, \
    {0, 0, 0} \
}

#define SECOND { \
    {1, 0, 1},  \
    {1, 0, 1}, \
    {1, 1, 1} \
}
#define THIRD { \
    {1, 1, 0},  \
    {1, 0, 1}, \
    {1, 1, 1} \
}
#define FORTH { \
    {1, 1, 1},  \
    {1, 0, 0}, \
    {1, 1, 1} \
}

#define FIFTH { \
    {1, 1, 1},  \
    {1, 0, 1}, \
    {1, 1, 0} \
}

#define SIXTH { \
    {1, 1, 1},  \
    {1, 0, 1}, \
    {1, 0, 1} \
}

#define SEVENTH { \
    {1, 1, 1},  \
    {1, 0, 1}, \
    {0, 1, 1} \
}

#define EIGHT{ \
    {1, 1, 1},  \
    {0, 0, 1}, \
    {1, 1, 1} \
}

#define NINETH { \
    {0, 1, 1},  \
    {1, 0, 1}, \
    {1, 1, 1} \
}
const int pin[6] = {0, 1, 2, 3, 4, 5};

// 2-dimensional array of row pin numbers:
const int row[3] = {pin[0], pin[1], pin[2]};

// 2-dimensional array of column pin numbers:
const int col[3] = {pin[3], pin[4], pin[5]  };


// 2-dimensional array of pixels:
int pixels[3][3];           

// cursor position:
int x = 1;
int y = 1;

int time = 0;

const int patternCount = 8;
int patternIndex = 0;
byte patterns[patternCount][3][3] = {
 SECOND, THIRD, FORTH, FIFTH, SIXTH,SEVENTH, EIGHT, NINETH};




void setup() {
  // initialize the I/O pins as outputs
  // iterate over the pins:
  for (int thisPin = 0; thisPin < 3; thisPin++) {
    // initialize the output pins:
    pinMode(col[thisPin], OUTPUT); 
    pinMode(row[thisPin], OUTPUT);  
    // take the col pins (i.e. the cathodes) high to ensure that
    // the LEDS are off: 
    digitalWrite(col[thisPin], HIGH);    
  }

  // initialize the pixel matrix:
  for (int x = 0; x < 3; x++) {
    for (int y = 0; y < 3; y++) {
      pixels[x][y] = HIGH;
    }
  }
}

void loop() {
  // read input:
  time++;
  if(time > 10000){
    time = 0;
    patternIndex++;    
  }
  if(patternIndex == patternCount){
    patternIndex = 0;
  }
    // draw the screen:
  displayPattern(patterns[patternIndex]);
}


void displayPattern(byte pattern[3][3]) {
  // iterate over the rows (anodes):
  for (int thisRow = 0; thisRow < 3; thisRow++) {
    // take the row pin (anode) high:
    digitalWrite(row[thisRow], HIGH);
    // iterate over the cols (cathodes):
    for (int thisCol = 0; thisCol < 3; thisCol++) {
      // get the state of the current pixel;
      int thisPixel =  HIGH;
      if(pattern[thisRow][thisCol] == 1){
        thisPixel = LOW; 
      };
      // when the row is HIGH and the col is LOW,
      // the LED where they meet turns on:
      digitalWrite(col[thisCol], thisPixel);
      // turn the pixel off:
      if (thisPixel == LOW) {
        digitalWrite(col[thisCol], HIGH);
      }
    }
    // take the row pin low to turn off the whole row:
    digitalWrite(row[thisRow], LOW);
  }
}
