	/*
 * sw_matrix
 * 
 * ROWS are output
 * COLS are input
 * 
 * !!170205:VG:Creation
 * 
 */

#ifndef sw_matrix_h
#define sw_matrix_h

#include <Arduino.h>

/*
 * 
const byte ROWS = 8; // in
const byte COLS = 5; // out

byte rowPins[ROWS] = {23, 25, 27, 29, 31, 33, 35, 37}; // in
byte colPins[COLS] = {22, 24, 26, 28, 30}; // out

byte switches_id[ROWS][COLS];
byte switches_state[ROWS][COLS];
byte switches_prev_state[ROWS][COLS];

 */


class SwMatrix
{
  private:
    byte colCount = 0;
    byte rowCount = 0;
    byte *rowPins = 0; //[ROWS]; //input
    byte *colPins = 0; //[COLS]; //output

    byte *sw_id = 0; //[ROWS][COLS];
    byte *sw_prev_state = 0; //[ROWS][COLS];
    
  public:
    byte *sw_state = 0; //[ROWS][COLS];

    SwMatrix(byte id, byte rowCount, byte colCount, byte *rows, byte *cols);
    void init();
    byte matrixToId(byte col, byte row);
    boolean read();
    
};


#endif

