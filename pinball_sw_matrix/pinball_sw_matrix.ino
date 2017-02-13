#include "hw_rules.h"
#include "sw_matrix.h"

byte led;
unsigned long activity_time_out;

const byte ROWS = 8; // in
const byte COLS = 5; // out

byte rowPins[ROWS] = {23, 25, 27, 29, 31, 33, 35, 37}; // in
byte colPins[COLS] = {22, 24, 26, 28, 30}; // out


SwMatrix *matrix = 0;
HwRules *rules = 0; 

void setup() {
  // put your setup code here, to run once:



  pinMode(52, INPUT_PULLUP);
  pinMode(53, OUTPUT);
  digitalWrite(53, HIGH);
  pinMode(51, OUTPUT);
  digitalWrite(51, LOW);

  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(9600);
/*  
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
*/  
  Serial.println("init done!");

  updated();
  //setup_switch_matrix();

  matrix = new SwMatrix(1, ROWS, COLS, rowPins, colPins);
  rules = new HwRules();
  
    
}


void updated() {
  activity_time_out = millis() + 200;
  led = 1;
}


void refresh_led() {
  // blink the led when activity has been detected
  unsigned long t;
  t = millis();
  if (activity_time_out > 0) {
    if (t > activity_time_out) {
      led = 0;
      activity_time_out = 0;
    }
  }
  digitalWrite(LED_BUILTIN, led);
}


void loop() {
  // put your main code here, to run repeatedly:
  byte v;
  int i;
  //read_switchs();
  if (matrix->read())
    updated();

  refresh_led();  
  delay(10);

}




byte switches_id[ROWS][COLS];
byte switches_state[ROWS][COLS];
byte switches_prev_state[ROWS][COLS];


///////////////// switchs matrix

void setup_switch_matrix() {
  byte r, c, v;
  for (c = 0; c < COLS; c++)
    for (r = 0; r < ROWS; r++)
    {
      v = byte(1 + c + r * ROWS);
      //Serial.println(String(v));
      switches_id[r][c] = v;
      switches_state[r][c] = 0;
      switches_prev_state[r][c] = 0;
    }


  // in
  for (r = 0; r < ROWS; r++)
    pinMode(rowPins[r], INPUT_PULLUP);

  // out
  for (c = 0; c < COLS; c++) {
    pinMode(colPins[c], OUTPUT);
    digitalWrite(colPins[c], HIGH);
  }
}

void read_switchs() {
  String s;

  for (byte c = 0; c < COLS; c++) {
    // enable the column
    digitalWrite(colPins[c], LOW);
    for (byte r = 0; r < ROWS; r++) {
      // save previous state
      switches_prev_state[r][c] = switches_state[r][c];
      // read current state
      switches_state[r][c] = byte(digitalRead(rowPins[r]));
      if (switches_prev_state[r][c] != switches_state[r][c]) {
        //Serial.write(sprintf("%d:%d", switches_id[r][c], switches_state[r][c]));
        //s = switches_id[r][c] + String(":") + c + String("/") + r + String(":") + switches_state[r][c] + String(",");
        s = String("S") + switches_id[r][c] + String(":") + switches_state[r][c];
        Serial.println(s);
        updated(); // signify a change / activity to the led
      }
    }
    // disable the column
    digitalWrite(colPins[c], HIGH);
  }
  //Serial.println(".");
}


/*
  mode: INPUT, OUTPUT, or INPUT_PULLUP

*/




