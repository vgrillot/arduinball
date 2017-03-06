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
#include "sw_custom.h"


class SwMatrix : public SwCustom
{
  private:
    byte __colCount = 0;
    byte __rowCount = 0;
    byte *__rowPins = 0; //input
    byte *__colPins = 0; //output
  public:
    SwMatrix(byte id, byte rowCount, byte colCount, byte *rows, byte *cols);
    void init(byte* baseId);
    byte matrixToId(byte col, byte row);
    boolean read();    
    boolean isSwitchActive(byte swId);      
};


#endif

