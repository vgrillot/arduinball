/*
 * hw_rules
 * 
 * !!170203:VG:Creation
 * 
 */

#include "hw_rules.h"


/*
 * constructor HwRules()
 * !!170204:VG:Creation
 */
HwRules::HwRules() {
  // Clear array
  for (byte i=0; i<MAX_HW_RULES; i++) {
    this->__rules[i].type = hw_rule_ndef;
  }
}


/*
 * init the communication driver
 */
void HwRules::setComm(Comm *comm) {
  this->__comm = comm;
}


/*
 * addRule
 * create dynamically a rule to manage 
 * !!170204:VG:Creation
 * !!170219:VG:set pin to output and low by default
 */
void HwRules::addHwRule(HwRuleType type, int enableSwitchId, int coilPin, int disableSwitchId, unsigned int duration) {
  // search for an empty rule slot
  if ((enableSwitchId > 0) && (!this->existsSwitch(enableSwitchId)))
  {
    this->__comm->error("addHwRule:en_sw doesn't exists!!");
    return;
  }
  if (coilPin > 100) 
  {
    this->__comm->error("addHwRule:coil_pin doesn't exists!!");
    return;
  }  
  HwRule *r;
  for (byte i=0; i<MAX_HW_RULES; i++) {
    r = &(this->__rules[i]);
    if ((r->type == hw_rule_ndef) || 
        ((r->coilPin == coilPin) && (r->enableSwitchId == enableSwitchId))) {
      r->id = i;
      r->type = type;
      r->enableSwitchId = enableSwitchId;
      r->coilPin = coilPin;
      r->disableSwitchId = disableSwitchId;
	    r->state = hw_rule_state_enabled;
      r->timeout = 0;
      r->duration = duration;
      pinMode(r->coilPin, OUTPUT);
      digitalWrite(r->coilPin, LOW);

      this->__comm->info("HwRules::addHwRule(id=" + String(r->id) + ",t=" + 
                                              String(r->type) + ",coil=" + 
                                              String(r->coilPin) + ",sw=" + 
                                              String(r->enableSwitchId) +")");     
      return;
    }    
  }
  // Empty slot not found !
  //TODO: raise error, no more slot for new rules...  
  this->__comm->error("addHwRule:NO MORE SLOT FOR RULE !!");
}


/*
 * addInput()
 * register an input "device"
 * !!170223:VG:Creation
 */
void HwRules::addInput(SwCustom *input) {
	this->__inputs[this->__inputCount] = input;
	this->__inputCount++;
  input->setComm(this->__comm);
	//TODO:manage max
}


/*
 * isSwitchActive()
 *
 */
boolean HwRules::isSwitchActive(byte swId) {
  SwCustom *inp;
  for (byte i = 0; i < this->__inputCount; i++)
  {
    inp = this->__inputs[i];
    if (inp->acceptSwId(swId))
      return inp->isSwitchActive(swId);
  } 
  this->__comm->error("HwRules:sw not found:" + String(swId));
  return false;
}

  
/*
 * existsSwitch()
 * return true if the switch id exists in any input
 * !!170419:VG:Creation
 */
boolean HwRules::existsSwitch(byte swId) {
  SwCustom *inp;
  for (byte i = 0; i < this->__inputCount; i++)
  {
    inp = this->__inputs[i];
    if (inp->acceptSwId(swId))
      return true;
  } 
  return false;
}

  

/*
 * readAll()
 * Loop over all inputs and read them
 * !!170223:VG:Creation
 */
boolean HwRules::readAll() {
	SwCustom *inp;
	boolean result;
	result = false;
	for (byte i = 0; i < this->__inputCount; i++)
	{
		inp = (this->__inputs[i]);
		result = inp->read() or result;
	}
	return result;
}	


/*
 * RunAll()
 * Loop over all the rules and run them
 * !!170205:VG:Creation
 */
boolean HwRules::runAll(unsigned int time) {
  // save the current time
  this->_time = time;
  
  // real all switches
  boolean result = this->readAll();
  
  // parse all rules
  HwRule *r;
  for (byte i=0; i<MAX_HW_RULES; i++) {
    r = &(this->__rules[i]);
    if (r->type != hw_rule_ndef) {
      this->runRule(r);
    }    
  }  
	return result;
}


/*
 * RunRule() 
 * Run the selected rule
 * !!170205:VG:Creation
 * !!170214:VG:Manage a state machine
 */
void HwRules::runRule(HwRule *r) {
	byte old_state;
	boolean switch_active;
	
	if(r->type != hw_rule_ndef) {	  
		old_state = r->state;
		if (r->state != hw_rule_state_disabled) {
		
			// read the main switch state
      if (r->enableSwitchId != 0) {
			  switch_active = this->isSwitchActive(r->enableSwitchId);
      }

			switch(r->state) {
				case hw_rule_state_enabled: //1
					if ((r->enableSwitchId == 0) || (switch_active)) {
						// enable the coil
						digitalWrite(r->coilPin, HIGH);
						r->state = hw_rule_state_activated;
					}
					break;
					
				case hw_rule_state_activated: //2
					// depending on the rule, need to wait a duration or a release
					switch(r->type) { 
					
						// time out:
						case hw_rule_pulse_on_hit_rule:
						case hw_rule_pulse:
							r->state = hw_rule_state_start_duration;
							break;
							
						// wait for release:
						case hw_rule_pulse_on_hit_and_enable_and_release_rule: 
						case hw_rule_pulse_on_hit_and_enable_and_release_and_disable_rule:
							r->state = hw_rule_state_wait_release; //TODO:rule to re-check
							break;
							
						// instant release:
						case hw_rule_pulse_on_hit_and_release_rule:
							r->state = hw_rule_state_wait_release;
//							r->state = hw_rule_state_release;
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
					
				case hw_rule_state_wait_release: //3
					if (!switch_active) {
						r->state = hw_rule_state_release;
					}
					break;
					
				case hw_rule_state_start_duration: //4
					r->timeout = this->_time + r->duration;
					r->state = hw_rule_state_wait_duration;
					break;
				
				case hw_rule_state_wait_duration: //5
					if (this->_time > r->timeout) {
            if (r->type == hw_rule_pulse)
              r->state = hw_rule_state_clear;
            else
						  r->state = hw_rule_state_release;
					}					
					break;
					
				case hw_rule_state_release : //6
					// disable the coil
					digitalWrite(r->coilPin, LOW);
					r->state = hw_rule_state_wait_final_release; //hw_rule_state_enabled;
					r->timeout = 0;
					break;

        case hw_rule_state_wait_final_release: //7
          if (!switch_active) {
            r->state = hw_rule_state_enabled;
          }
          break;

        case hw_rule_state_clear:
          // disable the coil
          digitalWrite(r->coilPin, LOW);
          r->type = hw_rule_ndef;
          r->state = hw_rule_state_disabled; // should not be used
          break;

			} //state?
		} //!disable
		
		if (r->state != old_state) {
			// too verbose ! 
			this->__comm->debug(String(this->_time) + ":" + String(r->id) + ":C" + String(r->coilPin) + ":" + String(old_state) + "->" +  String(r->state));
		}
		
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
    r = &(this->__rules[i]);
    if ((r->type != hw_rule_ndef) && (r->state != hw_rule_state_disabled)) {
		  r->state = hw_rule_state_release;
    }    
  }  
}


void HwRules::addPulse(int coilPin, int duration) {
  this->addHwRule(hw_rule_pulse, 0, coilPin, 0, duration); 
}

void HwRules::addEnable(int coilPin){
  this->addHwRule(hw_rule_enable, 0, coilPin, 0, 0); 
}

/*
 * addDisable()
 * Stop a coil activated by addEnable()
 * !!150418:VG:Creation
 */
void HwRules::addDisable(int coilPin){
  //search the enabled rule
  HwRule *r;
  for (byte i=0; i<MAX_HW_RULES; i++) {
    r = &(this->__rules[i]);
    if ((r->type == hw_rule_enable) && (r->coilPin == coilPin)) {
      r->state = hw_rule_state_release;
    }    
  }    
}


/*
 * clearRule()
 * search a matching rule and disable it
 */
void HwRules::clearRule(int coilPin, int enableSwitchId) {
  //search the enabled rule
  HwRule *r;
  for (byte i=0; i<MAX_HW_RULES; i++) {
    r = &(this->__rules[i]);
    if ((r->enableSwitchId == enableSwitchId) && (r->coilPin == coilPin)) {
      r->state = hw_rule_state_clear;
    }    
  }      
}




