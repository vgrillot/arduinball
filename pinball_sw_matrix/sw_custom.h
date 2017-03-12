/*
 * sw_custom
 * 
 * custom class for inputs 
 *
 * !!170223:VG:Creation
 * 
 */

#ifndef sw_custom_h
#define sw_custom_h

#include <Arduino.h>
#include "comm.h"

class SwCustom
{
  protected:
    Comm *_comm = 0;
    byte _id = 0; // id of the input
  	byte _base = 0;
    byte _count = 0; 			// nb of input
    byte *_pins = 0; 
    byte *_sw_id = 0; 
    byte *_sw_prev_state = 0; 
    
  public:
    byte *sw_state = 0; //[ROWS][COLS];

    void setComm(Comm *comm);
    virtual void init(byte* baseId);
    virtual boolean read();
    virtual boolean isSwitchActive(byte swId);	
    boolean acceptSwId(byte swId);
};


#endif

