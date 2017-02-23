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


class SwMatrix
{
  private:
    byte colCount = 0;
    byte rowCount = 0;
    byte *rowPins = 0; //input
    byte *colPins = 0s; //output

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

