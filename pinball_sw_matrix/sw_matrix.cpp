
/*
 * sw_matrix
 * 
 * !!170205:VG:Creation
 * 
 */
 
#include "sw_matrix.h"
#include "hw_rules.h"
#include <Arduino.h>

/*
 * SwMatrix
 * constructor
 * 
 * !!170205:VG:Creation
 * 
 */
SwMatrix::SwMatrix(byte id, byte rowCount, byte colCount, byte *rows, byte *cols) {

  Serial.println("SwMatrix::SwMatrix()");

  // memory allocation
  this->__rowCount = rowCount;
  this->__colCount = colCount;

  this->__rowPins  = (byte *) malloc(rowCount * sizeof(byte));
  memcpy(this->__rowPins, rows, rowCount * sizeof(byte));

  this->__colPins  = (byte *) malloc(colCount * sizeof(byte));
  memcpy(this->__colPins, cols,  colCount * sizeof(byte));

  this->_sw_id    = (byte *) malloc((rowCount * colCount) * sizeof(byte));
  this->sw_state = (byte *) malloc((rowCount * colCount) * sizeof(byte));
  this->_sw_prev_state = (byte *) malloc((rowCount * colCount) * sizeof(byte));

  //TODO: this->init();
}

/*
 * 
 * !!170218:VG:inverse row/col for numering
 */
byte SwMatrix::matrixToId(byte col, byte row) {
//  return row + col * this->__rowCount;
  return col + row * this->__colCount;
}

/*
 * Init()
 * Fill arrays
 * 
 * !!170205:VG:Creation
 * !!170305:VG:Add baseId
 *
 */
void SwMatrix::init(byte* baseId) {
  this->_base = *baseId;
  Serial.println("SwMatrix::init(" + String(this->__colCount) + "," + String(this->__rowCount) +")");
  byte r, c, v;
  for (c = 0; c < this->__colCount; c++)
    for (r = 0; r < this->__rowCount; r++)
    {
      //v = byte(1 + c + r * this->__rowCount);
      v = this->matrixToId(c, r);
      //Serial.println(String(v));
      this->_sw_id[v] = *baseId++;
      this->sw_state[v] = 0;
      this->_sw_prev_state[v] = 0;
    }

  // ininialize input
  for (r = 0; r < this->__rowCount; r++) 
    pinMode(this->__rowPins[r], INPUT_PULLUP);

  // initialize output
  for (c = 0; c < this->__colCount; c++) {
    pinMode(this->__colPins[c], OUTPUT);
    digitalWrite(this->__colPins[c], HIGH);
  }
  
}


/*
 * read()
 * read the matrix and store current switch values
 * 
 * !!170205:VG:Creation
 * 
 */
boolean SwMatrix::read() {
  String s;
  byte id;
  boolean updated = false;

  for (byte c = 0; c < this->__colCount; c++) {
    // enable the column
    digitalWrite(this->__colPins[c], LOW);
    for (byte r = 0; r < this->__rowCount; r++) {
      id = this->matrixToId(c, r);
      // save previous state
      this->_sw_prev_state[id] = sw_state[id];
      // read current state
      this->sw_state[id] = byte(digitalRead(this->__rowPins[r]));
      if (this->_sw_prev_state[id] != this->sw_state[id]) {
        s = String("S") + this->_sw_id[id] 
            + ":ST" + String(c) + ",RT" + String(r)  
            + ":" + this->sw_state[id];
        Serial.println(s);
        updated = true; // signify a change
      }
    }
    // disable the column
    digitalWrite(this->__colPins[c], HIGH);
  }
  return updated;
}





/*
 * isSwitchActive()
 *
 * look into the switch matrix
 * assume for now there is only one switch matrix
 *
 * !!170205:VG:Creation
 * !!170217:VG:Matrix level has been reversed, sw is activated on LOW level
 * !!170219:VG:FIX:swId is a visual ID, not index over array, then -1
 * !!170305:VG:Move from HwRules to SwMAtrix class
 */
boolean SwMatrix::isSwitchActive(byte swId) {
  return this->sw_state[swId - this->_base] == 0;
}
