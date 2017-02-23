 /*
 * board : arduino mega ADK
 * programmer : tiny usb isp
 * 
 */

#include "hw_rules.h"
#include "sw_matrix.h"
#include "sw_linear.h"


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


/*
 * coils
 */
 

/*
 * 22 -> 
 * 
 * 
 * 
 * 
 * 
 */
 
const byte pin_left_bumper_coil 		= 26; //1-orange
const byte pin_right_bumber_coil 	 	= 28; //2-orange
const byte pin_bottom_bumper_coil		= 32; //3-orange //confirmed
const byte pin_right_slingshot_coil		= 30; //4-violet //bad //ok?
const byte pin_left_slingshot_coil	= 34; //5-violet //bad
const byte pin_gate_coil				    = 34; //6-red //ok



const byte pin_left_flip_coil 			= 40; // 1-violet //ok
const byte pin_right_flip_coil 		= 40; // 2-violet //bad
const byte pin_drop_target_reset 		= 42; // 3-orange
const byte pin_sauver_coil 			= 44; // 4-orange --> gate
const byte pin_out_hole				= 46; // 5-orange


SwMatrix *matrix = 0;
SwLinear *linear = 0;
HwRules *rules = 0; 

void setup() {
  // put your setup code here, to run once:

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
  
  
  rules->addHwRule(hw_rule_pulse_on_hit_rule, 31, pin_right_slingshot_coil, 0, 50); 
  rules->addHwRule(hw_rule_pulse_on_hit_rule, 32, pin_left_slingshot_coil, 0, 50); 
  rules->addHwRule(hw_rule_pulse_on_hit_rule, 39, pin_right_bumber_coil, 0, 50); 
  rules->addHwRule(hw_rule_pulse_on_hit_rule, 40, pin_left_bumper_coil, 0, 50); 
  rules->addHwRule(hw_rule_pulse_on_hit_rule, 33, pin_bottom_bumper_coil, 0, 50); 

  
  // flip switch are not connected to matrix sw....
  
  rules->addHwRule(hw_rule_pulse_on_hit_and_release_rule, 18, pin_left_flip_coil, 0, 10); // left flip
  rules->addHwRule(hw_rule_pulse_on_hit_and_release_rule, 19, pin_right_slingshot_coil, 0, 10); // right flip
  

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





