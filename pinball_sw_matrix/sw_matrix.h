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


class SwMatrix : public SwCustom
{
  private:
    byte __colCount = 0;
    byte __rowCount = 0;
    byte *__rowPins = 0; //input
    byte *__colPins = 0s; //output

    //byte *sw_id = 0; //[ROWS][COLS];
    //byte *sw_prev_state = 0; //[ROWS][COLS];
    
  public:
    //byte *sw_state = 0; //[ROWS][COLS];

    SwMatrix(byte id, byte rowCount, byte colCount, byte *rows, byte *cols);
    void init();
    byte matrixToId(byte col, byte row);
    boolean read();
    
};


#endif

