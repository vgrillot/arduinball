/*
 * sw_linear
 * 
 * when swtich are connected to input without matrix 
 *
 * !!170222:VG:Creation
 * 
 */

#ifndef sw_linear_h
#define sw_linear_h

#include <Arduino.h>


class SwLinear
{
  private:
    byte _count = 0;
    byte *_pins = 0; 
    byte *_sw_id = 0; 
    byte *_sw_prev_state = 0; 
    
  public:
    byte *sw_state = 0; //[ROWS][COLS];

	SwLinear(byte id, byte count, byte *pins, byte *ids);
    void init();
    boolean read();
    
};


#endif

