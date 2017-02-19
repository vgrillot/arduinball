 /*
 * board : arduino mega ADK
 * programmer : tiny usb isp
 * 
 */

#include "hw_rules.h"
#include "sw_matrix.h"

byte led;
   unsigned long activity_time_out;

/*
const byte ROWS = 8; // in
const byte COLS = 5; // out
byte rowPins[ROWS] = {23, 25, 27, 2!9, 31, 33, 35, 37}; // in
byte colPins[COLS] = {22, 24, 26, 28, 30}; // out
*/
const byte COLS = 8; // in
const byte ROWS  = 5; // out
byte colPins[COLS] = {23, 25, 27, 29, 31, 33, 35, 37}; // in
byte rowPins[ROWS] = {39, 41, 43, 45, 47}; // out


SwMatrix *matrix = 0;
HwRules *rules = 0; 

void setup() {
  // put your setup code here, to run once:


/*
  pinMode(52, INPUT_PULLUP);
  pinMode(53, OUTPUT);
  digitalWrite(53, HIGH);
  pinMode(51, OUTPUT);
  digitalWrite(51, LOW);
*/

  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("init done!");

  updated();
  //setup_switch_matrix();

  // main object creation
  matrix = new SwMatrix(1, ROWS, COLS, &rowPins[0], &colPins[0]);
  rules = new HwRules();
  rules->setSwMatrix(matrix);
  // add some hardcoded rules for test purposes
  rules->addHwRule(hw_rule_pulse_on_hit_rule, 31, 26, 0, 10); // right slingshot
  rules->addHwRule(hw_rule_pulse_on_hit_rule, 32, 28, 0, 10); // left slingshot: 
  rules->addHwRule(hw_rule_pulse_on_hit_rule, 39, 30, 0, 10); // right bumper
  rules->addHwRule(hw_rule_pulse_on_hit_rule, 40, 32, 0, 10); // left bumper  : ok
  rules->addHwRule(hw_rule_pulse_on_hit_rule, 33, 40, 0, 10); // bottom bumper : 

  rules->addHwRule(hw_rule_pulse_on_hit_and_release_rule, 18, 34, 0, 10); // fake target/flip: 
  rules->addHwRule(hw_rule_pulse_on_hit_and_release_rule, 19, 36, 0, 10); // fake target/flip :
  rules->addHwRule(hw_rule_pulse_on_hit_and_release_rule, 20, 38, 0, 10); // fake target/flip :
  
  rules->addHwRule(hw_rule_pulse_on_hit_rule, 28, 44, 0, 100); // fake star 2: 
  rules->addHwRule(hw_rule_pulse_on_hit_rule, 29, 46, 0, 100); // fake star 3 : 
  rules->addHwRule(hw_rule_pulse_on_hit_rule, 26, 42, 0, 100); // fake star 5 : 
  
  
//  rules->addHwRule(hw_rule_pulse_on_hit_and_release_rule, 8, 52, 0, 10); // ex of flip
//  rules->addHwRule(hw_rule_pulse_on_hit_and_release_rule, 8, 52, 0, 10); // ex of flip

  Serial.println("rules done!");
    
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
  unsigned long t;
  t = millis();

  //read_switchs();
  if (matrix->read())
    updated();
  //run rules
  rules->runAll(t);

  refresh_led();  
}





