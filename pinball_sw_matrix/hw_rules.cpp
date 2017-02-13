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
      r->active = false;
      r->enable = true;
      r->activationTime = 0;
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
 */
void HwRules::runRule(HwRule *r) {
  if(r->type != hw_rule_ndef) {
    // when a rule is enable, it can be triggered again,
    // only if not already active
    if (r->enable && !r->active) {
      if (this->isSwitchActive(r->enableSwitchId)) {
        r->active = true;
        r->activationTime = time; //TODO
        digitalWrite(r->coilPin, HIGH);
      }
    }
  
    // when a rule has been activated, it must be monitored until end of coil activation
    // even if this rule is already disabled
    if (r->active) {
      if (r->activationTime + r->duration > this->time) {
        this->stopRule(r);
      }      
    }    
  }
  
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
    r->activationTime = 0;  
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

