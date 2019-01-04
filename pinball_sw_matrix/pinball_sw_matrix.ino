/*
 * board : arduino mega ADK
 * programmer : tiny usb isp
 * 
 */

#include "hw_rules.h"
#include "sw_matrix.h"
#include "sw_linear.h"
#include "comm.h"
#include "protocol.h"


byte led;
unsigned long activity_time_out;
unsigned long period_time_out;
unsigned long period_count;
const unsigned long period_interval = 5000;

const byte COLS = 8; // in
const byte ROWS  = 5; // out
byte colPins[COLS] = {23, 25, 27, 29, 31, 33, 35, 37}; // in
byte rowPins[ROWS] = {39, 41, 43, 45, 47}; // outj

const byte LINEAR = 4;
byte linearPins[LINEAR] = {49, 51, 48, 50};

/*
 * coils
 */
 
const byte pin_left_bumper_coil 		= 46; //1-orange
const byte pin_right_bumber_coil 	 	= 30; //2-orange
const byte pin_bottom_bumper_coil		= 32; //3-orange //confirmed
const byte pin_right_slingshot_coil		= 28; //4-violet 
const byte pin_left_slingshot_coil		= 44; //5-violet 
const byte pin_gate_coil				= 34; //6-red //ok

const byte pin_left_flip_coil 			= 40; // 1-violet //ok
const byte pin_right_flip_coil 			= 38; // 2-violet //bad

const byte pin_drop_target_reset_coil 	= 36; // 3-orange
const byte pin_saucer_coil 				= 26; // 4-orange 
const byte pin_out_hole					= 42; // 5-orange

boolean fake_rules_done = false;
unsigned long time_before_fake_rules = 0;

SwMatrix *matrix = 0;
SwLinear *linear = 0;
HwRules *rules = 0; 
Comm *comm = 0;
Protocol *protocol = 0;

/*
 *
 *  S E T U P
 *
 */

void setup() {

  period_count = 0;
  pinMode(LED_BUILTIN, OUTPUT);

  time_before_fake_rules = millis() + 10000;
  
  comm = new Comm(true);
  comm->debug("Init...");

  updated();


  // main object creation
  byte baseId = 0;
  matrix = new SwMatrix(1, ROWS, COLS, &rowPins[0], &colPins[0]); // TODO: +baseId
  linear = new SwLinear(2, LINEAR, &linearPins[0]); // TODO: +baseId

  rules = new HwRules();
  rules->setComm(comm);
  rules->addInput(matrix);
  rules->addInput(linear);
  baseId = 1;
  matrix->init(&baseId);
  baseId = 101;
  linear->init(&baseId);

  protocol = new Protocol(comm, rules);
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


void fake_rules_init() {
    // add some hardcoded rules for test purposes at arduino start
    fake_rules_done = true;
    time_before_fake_rules = 0; // disable the timer 
    
    rules->addHwRule(hw_rule_pulse_on_hit_rule, 31, pin_right_slingshot_coil, 0, 50); //ok
    rules->addHwRule(hw_rule_pulse_on_hit_rule, 32, pin_left_slingshot_coil, 0, 50); //ok
    rules->addHwRule(hw_rule_pulse_on_hit_rule, 39, pin_right_bumber_coil, 0, 50); //ok
    rules->addHwRule(hw_rule_pulse_on_hit_rule, 40, pin_left_bumper_coil, 0, 50); //ok
    rules->addHwRule(hw_rule_pulse_on_hit_rule, 33, pin_bottom_bumper_coil, 0, 50); //ok
    
    rules->addHwRule(hw_rule_pulse_on_hit_and_release_rule, 101, pin_left_flip_coil, 0, 10); // left flip
    rules->addHwRule(hw_rule_pulse_on_hit_and_release_rule, 102, pin_right_flip_coil, 0, 10); // right flip
    
    rules->addHwRule(hw_rule_pulse_on_hit_rule, 23, pin_drop_target_reset_coil, 0, 50); 
    rules->addHwRule(hw_rule_pulse_on_hit_rule, 22, pin_saucer_coil, 0, 50); //center target
    rules->addHwRule(hw_rule_pulse_on_hit_and_release_rule, 26, pin_gate_coil, 0, 50); //star 5
    rules->addHwRule(hw_rule_pulse_on_hit_rule, 103, pin_out_hole, 0, 50); //real coin button
    comm->debug("rules done!");      
}


void cancel_fake_rules() {
    // Clear previous fake rules installed
    time_before_fake_rules = 0; // disable the fake timer
    if (fake_rules_done) {
        // rules have been setted, need to clear all
        rules->clearAllRules();
        //190104:VG:FIX:cancel was always called...
        fake_rules_done = False
    }
}


void loop() {
  unsigned long t;
  t = millis();
  
  //run rules
  if (rules->runAll(t))
    updated();

  if (comm->read()) {
    cancel_fake_rules();
  }
  protocol->run();

  period_count++;
  if (t > period_time_out) {
    period_time_out = t + 5000;
    period_count = period_count * 1000 / period_interval;
    comm->tick(period_count);
    period_count = 0;

    //!!181104:VG:Add verbose debug
    rules->debugAll();

    // timer to set fake rules
    if ((time_before_fake_rules > 0) && (t > time_before_fake_rules) && (!fake_rules_done))
    {
      fake_rules_init();
    }
  }
  refresh_led();  

}





