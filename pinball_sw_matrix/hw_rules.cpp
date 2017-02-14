/*
 * hw_rules
 * 
 * !!170203:VG:Creation
 * 
 */

#include  <Arduino.h>


#include "hw_rules.h"
#include "hw_rule.h"

/*
 * constructor HwRules()
 * !!170204:VG:Creation
 */
HwRules::HwRules() {

  // Clear array
  for (byte i=0; i<MAX_HW_RULES; i++) {
    this->rules[i].type = hw_rule_ndef;
  }
  
  //ok : this->rules[0] = &HwRule(hw_rule_pulse_on_hit_rule);  
}


/*
 * addRule
 * create dynamically a rule to manage 
 * !!170204:VG:Creation
 */
void HwRules::addHwRule(HwRuleType type, int enableSwitchId, int coilPin, int disableSwitchId, unsigned int duration) {
  // search for an empty rule slot
  HwRule *r;
  for (byte i=0; i<MAX_HW_RULES; i++) {
    r = &(this->rules[i]);
    if (r->type == hw_rule_ndef) {
      r->type = type;
      r->enableSwitchId = enableSwitchId;
      r->coilPin = coilPin;
      r->disableSwitchId = disableSwitchId;
	  r->state = hw_rule_state_enabled;

      r->active = false;
      r->enable = true;
      r->timeout = 0;
      r->duration = duration;
      return;
    }    
  }
  // Empty slot not found !
  //TODO: raise error, no more slot for new rules...  
}


/*
 * RunAll()
 * Loop over all the rules and run them
 * !!170205:VG:Creation
 */
void HwRules::runAll(unsigned int time) {
  // save the current time
  this->time = time;
  // parse all rules
  HwRule *r;
  for (byte i=0; i<MAX_HW_RULES; i++) {
    r = &(this->rules[i]);
    if (r->type != hw_rule_ndef) {
      this->runRule(r);
    }    
  }  
}

/*
 * RunRule() 
 * Run the selected rule
 * !!170205:VG:Creation
 * !!170214:VG:Manage a state machine
 */
void HwRules::runRule(HwRule *r) {
	boolean switch_active;
	if(r->type != hw_rule_ndef) {	  
		if (r->state != hw_rule_state_disabled) {
		
			// read the main switch state
			switch_active = this->isSwitchActive(r->enableSwitchId);

			switch(r->state) {
				case hw_rule_state_enabled:
					if (switch_active) {
						// enable the coil
						digitalWrite(r->coilPin, HIGH);
						r->state = hw_rule_state_activated;
					}
					break;
					
				case hw_rule_state_activated:
					// depending on the rule, need to wait a duration or a release
					switch(r->type) { 
					
						// time out:
						case hw_rule_pulse_on_hit_rule:
            case hw_rule_pulse:
							r->state = hw_rule_state_start_duration;
							break;
							
						// wait for release:
						case hw_rule_pulse_on_hit_and_enable_and_release_rule: 
							r->state = hw_rule_state_wait_release;
							break;
							
						// instant release:
						case hw_rule_pulse_on_hit_and_release_rule:
							r->state = hw_rule_state_release;
							break;
							
						// none, stay active until further command:
						case hw_rule_enable:
							// stay !
							break; 
							
						// error, undefined case?
						default: 
							r->state = hw_rule_state_release;
							break;
					}
					break;
					
				case hw_rule_state_wait_release:	
					if (!switch_active) {
						r->state = hw_rule_state_release;
					}
					break;
					
				case hw_rule_state_start_duration:
					r->timeout = time + r->duration;
					r->state = hw_rule_state_wait_duration;
					break;
				
				case hw_rule_state_wait_duration:
					if (time > r->timeout) {
						r->state = hw_rule_state_release;
					}					
					break;
					
				case hw_rule_state_release :
					// disable the coil
					digitalWrite(r->coilPin, LOW);
					r->state = hw_rule_state_enabled;
					r->timeout = 0;
					break;


				
			} //state?

		} //!disable
	} //!ndef
  
}


/*
 * stopAll()
 * Could be used as an emergency stop
 * !!170205:VG:Creation
 */
void HwRules::stopAll() {
  HwRule *r;
  for (byte i=0; i<MAX_HW_RULES; i++) {
    r = &(this->rules[i]);
    if (r->type != hw_rule_ndef) {
      this->stopRule(r);
    }    
  }  
}


/*
 * stopRule()
 * Stop the activated coil
 * !!170205:VG:Creation
 */
void HwRules::stopRule(HwRule *r) { 
  // if the rule has been activated, 
  // then stop the activated coil
  if (r->active) {
    //TODO:
    digitalWrite(r->coilPin, LOW);    
    r->active = false;
    r->timeout = 0;  
  }    
}

/*
 * activeRule()
 * Active the coil and start timers
 */
void HwRules::activeRule(HwRule *r){
  r->active = true;
//  r->activationTime = time; //TODO:global time var?
  digitalWrite(r->coilPin, HIGH);
}


/*
 * isSwitchActive()
 * look into the switch matrix
 * assume for now there is only one switch matrix
 * !!170205:VG:Creation
 */
boolean HwRules::isSwitchActive(byte swId) {
  return this->matrix->sw_state[swId] > 0;
}


/*
 * setSwMatrix
 * connect to the switch matrix
 * only one matrix allowed for now
 * 
 * !!170205:VG:Creation
 * 
 */
void HwRules::setSwMatrix(SwMatrix *matrix) {
  this->matrix = matrix;
}

