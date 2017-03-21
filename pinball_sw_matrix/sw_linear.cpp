
/*
 * sw_matrix
 * 
 * !!170205:VG:Creation
 * 
 */
 
#include "sw_linear.h"
// #include "hw_rules.h"
#include <Arduino.h>

/*
 * SwMatrix
 * constructor
 * 
 * !!170222:VG:Creation
 * !!170305:VG:Removed ids
 * 
 */
SwLinear::SwLinear(byte id, byte count, byte *pins) {
  // memory allocation
  this->_count = count;

  this->_pins  = (byte *) malloc(count);
  memcpy(this->_pins, pins, count);
  this->_sw_id    = (byte *) malloc(count);
  this->sw_state = (byte *) malloc(count);
  this->_sw_prev_state = (byte *) malloc(count);
  this->_bounce = (byte *) malloc(count);
}


/*
 * Init()
 * Fill arrays
 * 
 * !!170222:VG:Creation
 * !!170305:VG:Add baseId
 */
void SwLinear::init(byte* baseId) 
{
  this->_base = *baseId;
	for (byte i = 0; i < this->_count; i++)
	{
    this->_sw_id[i] = (*baseId)++;
		this->sw_state[i] = 0;
		this->_sw_prev_state[i] = 0;
    this->_bounce[i] = 0;
		pinMode(this->_pins[i], INPUT_PULLUP);
	}
}


/*
 * read()
 * read inputs and store current switch values
 * 
 * !!170205:VG:Creation
 * 
 */
boolean SwLinear::read() {
  String s;
  boolean updated = false;
  byte state, prev, curr, bounce;


//  this->_comm->writeSwBeginUpdate();
  for (byte i = 0; i < this->_count; i++) 
  {
      prev = this->_sw_prev_state[i];
      curr = this->sw_state[i];
      // read current state
      state = byte(digitalRead(this->_pins[i]));

      // debounce
      if (state != curr) {
        bounce = this->_bounce[i];
        bounce++;
        if (bounce > 5) {
          this->sw_state[i] = state;
          bounce = 0;
        }
        this->_bounce[i] = bounce;
      }
      if (this->_sw_prev_state[i] != this->sw_state[i]) 
	    {
        // save previous state
        this->_sw_prev_state[i] = sw_state[i];
        s = String("SL") + this->_sw_id[i] + ":" + this->sw_state[i];
        //Serial.println(s);
        this->_comm->writeSwUpdate(this->_sw_id[i], this->sw_state[i]);
        updated = true; // signify a change
      }
  }
//  this->_comm->writeSwEndUpdate();
  return updated;
}



/*
 * isSwitchActive()
 *
 * look into the switch 
 *
 * !!170305:VG:Creation
 */
boolean SwLinear::isSwitchActive(byte swId) {
  return this->sw_state[swId - this->_base] == 0;
}
