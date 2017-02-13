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
  hw_rule_pulse_on_hit_rule = 1,
  hw_rule_pulse_on_hit_and_release_rule = 2,
  hw_rule_pulse_on_hit_and_enable_and_release_rule = 3,
  hw_rule_pulse_on_hit_and_enable_and_release_and_disable_rule = 4,
  hw_rule_enable = 5,
  hw_rule_pulse = 6
};


typedef struct {
    // type : define the type of the rule
    HwRuleType type;
    
    // is the rule enabled, if not, do nothing on Run()
    boolean enable;    
    
    // active : true if some coild is activated
    boolean active;
    
    // activationTime : when the rule has been activated
    unsigned long activationTime;
    
    // duration: how long the coil must be activated
    unsigned long duration;
    
    // enableSwitchId : trigger switch
    int enableSwitchId;
    
    // coilPin : coil to be activated
    int coilPin;  
    
    // disableSwitchId: used by pulse_on_hit_and_enable_and_release_and_disable_rule
    int disableSwitchId ;

    
} HwRule;




#endif

