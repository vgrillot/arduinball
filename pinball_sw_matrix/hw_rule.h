/*
 * hw_rule
 * 
 * !!170203:VG:Creation
 * 
 */

#ifndef hw_rule_h
#define hw_rule_h

#include <Arduino.h>

/*
 * Hardware rules classes
 */
enum HwRuleType: byte { 
  hw_rule_ndef = 0, 
	// 0: nothing
  hw_rule_pulse_on_hit_rule = 1,
	// 1: instant pulse + duration
  hw_rule_pulse_on_hit_and_release_rule = 2,
	// 2: instant pulse + instant release (ex:flip)
  hw_rule_pulse_on_hit_and_enable_and_release_rule = 3,
  hw_rule_pulse_on_hit_and_enable_and_release_and_disable_rule = 4,
  hw_rule_enable = 5,
  hw_rule_pulse = 6
};


/*
 * Hardware rule state machine
 */
enum HwRuleState: byte {
  hw_rule_state_disabled = 0,		// rule can't be activated
  hw_rule_state_enabled = 1,         // rule is waiting for a trigger
  hw_rule_state_activated = 2,         // rule has just been triggered and coil activated
  hw_rule_state_wait_release = 3,   // rule is waiting for the switch release
  hw_rule_state_start_duration = 4, // rule is still active, start the timeout
  hw_rule_state_wait_duration = 5,  // rule is still active, waiting for the timeout
  hw_rule_state_release = 6,  			// coil set to OFF
  hw_rule_state_wait_final_release = 7, // ensure sw is deactivated
  hw_rule_state_clear = 8 // to be disable and then clear
};


/*
 * HwRule
 */
typedef struct {
    // id 
    byte id;
    
    // type : define the type of the rule    
    HwRuleType type;

    // state : state machine of the rule
	  HwRuleState state;
            
    // timeout : when the rule must be released
    unsigned long timeout;
    
    // duration: how long the coil must be activated
    unsigned long duration;
    
    // enableSwitchId : trigger switch
    int enableSwitchId;
    
    // coilPin : coil to be activated
    int coilPin;  
    
    // disableSwitchId: used by pulse_on_hit_and_enable_and_release_and_disable_rule
    int disableSwitchId;

    
} HwRule;




#endif

