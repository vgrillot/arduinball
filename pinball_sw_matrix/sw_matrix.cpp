
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
  this->rowCount = rowCount;
  this->colCount = colCount;

  this->rowPins  = (byte *) malloc(rowCount * sizeof(byte));
  memcpy(this->rowPins, rows, rowCount * sizeof(byte));
  this->colPins  = (byte *) malloc(colCount * sizeof(byte));
  memcpy(this->colPins, cols,  colCount * sizeof(byte));
  this->sw_id    = (byte *) malloc((rowCount * colCount) * sizeof(byte));
  this->sw_state = (byte *) malloc((rowCount * colCount) * sizeof(byte));
  this->sw_prev_state = (byte *) malloc((rowCount * colCount) * sizeof(byte));
  this->init();
}

/*
 * 
 */
byte SwMatrix::matrixToId(byte col, byte row) {
  return row + col * this->rowCount;
}

/*
 * Init()
 * Fill arrays
 * 
 * !!170205:VG:Creation
 */
void SwMatrix::init() {

  Serial.println("SwMatrix::init(" + String(this->colCount) + "," + String(this->rowCount) +")");
  byte r, c, v;
  for (c = 0; c < this->colCount; c++)
    for (r = 0; r < this->rowCount; r++)
    {
      //v = byte(1 + c + r * this->rowCount);
      v = this->matrixToId(c, r);
      Serial.println(String(v));
      this->sw_id[v] = v;
      this->sw_state[v] = 0;
      this->sw_prev_state[v] = 0;
    }


  // ininialize input
  for (r = 0; r < this->rowCount; r++) 
    pinMode(rowPins[r], INPUT_PULLUP);
  

  // initialize output
  for (c = 0; c < this->colCount; c++) {
    pinMode(colPins[c], OUTPUT);
    digitalWrite(colPins[c], HIGH);
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

  for (byte c = 0; c < this->colCount; c++) {
    // enable the column
    digitalWrite(this->colPins[c], LOW);
    for (byte r = 0; r < this->rowCount; r++) {
      id = this->matrixToId(c, r);
      // save previous state
      this->sw_prev_state[id] = sw_state[id];
      // read current state
      this->sw_state[id] = byte(digitalRead(this->rowPins[r]));
      if (this->sw_prev_state[id] != this->sw_state[id]) {
        s = String("S") + this->sw_id[id] + String(":") + this->sw_state[id];
        Serial.println(s);
        updated = true; // signify a change
      }
    }
    // disable the column
    digitalWrite(this->colPins[c], HIGH);
  }
  return updated;
}


