
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
  // memory allocation
  this->_id = id;

  this->__rowCount = rowCount;
  this->__colCount = colCount;
  this->_count = rowCount * colCount;

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
void SwMatrix::init(byte *baseId) {
  this->_base = *baseId;
  this->_comm->debug("SwMatrix::init(" + String(this->__colCount) + "," + String(this->__rowCount) +")");
  byte r, c, v;
  for (c = 0; c < this->__colCount; c++)
    for (r = 0; r < this->__rowCount; r++)
    {
      v = this->matrixToId(c, r);
      this->_comm->debug(String(v));
      this->_sw_id[v] = v + 1;
      (*baseId)++;
      this->_comm->debug(String(this->_sw_id[v]));
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
//  this->_comm->writeSwBeginUpdate();
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
        s = String("SM") + this->_sw_id[id] 
            + ":ST" + String(c) + ",RT" + String(r)  
            + ":" + this->sw_state[id];
        Serial.println(s);
        //this->_comm->writeSwUpdate(this->_sw_id[id], this->sw_state[id]);            
        updated = true; // signify a change
      }
    }
    // disable the column
    digitalWrite(this->__colPins[c], HIGH);
  }
//  this->_comm->writeSwEndUpdate();
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
