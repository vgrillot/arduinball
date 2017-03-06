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

#include <arduino.h>
#include "sw_custom.h"


class SwLinear : public SwCustom
{
    
  public:

	SwLinear(byte id, byte count, byte *pins);
    void init(byte* baseId);
    boolean read();
    boolean isSwitchActive(byte swId);	

    
};


#endif

