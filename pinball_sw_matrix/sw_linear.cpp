
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
 * 
 */
SwLinear::SwLinear(byte id, byte count, byte *pins, byte *ids) {

  Serial.println("SwLinear::SwLinear()");

  // memory allocation
  this->_count = count;

  this->_pins  = (byte *) malloc(count);
  memcpy(this->pins, pins, count;
  this->_sw_id    = (byte *) malloc(count);
  this->sw_state = (byte *) malloc(count);
  this->_sw_prev_state = (byte *) malloc(count);
  this->init();
}


/*
 * Init()
 * Fill arrays
 * 
 * !!170222:VG:Creation
 */
void SwLinear::init() 
{
	Serial.println("SwLinear::init(" + String(this->_count) + ")");
	for (byte i = 0; i < this->_count; i++)
	{
		this->sw_state[i] = 0;
		this->_sw_prev_state[i] = 0;
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

  for (byte i = 0; i < this->_count; i++) 
  {
	// enable the column
      // save previous state
      this->_sw_prev_state[i] = sw_state[i];
      // read current state
      this->_sw_state[i] = byte(digitalRead(this->_pins[i]));
      if (this->_sw_prev_state[i] != this->sw_state[i]) 
	  {
        s = String("S") + this->_sw_id[i] 
            + ":" + this->sw_state[i];
        Serial.println(s);
        updated = true; // signify a change
      }
  }
  return updated;
}


