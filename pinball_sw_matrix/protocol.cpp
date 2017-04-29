/*
 * protocol
 *
 * Manage protocol exchange with RaspPinball platform
 * 
 * !!170314:VG:Creation
 * 
 */


#include "protocol.h"
#include "hw_rules.h"

Protocol::Protocol(Comm *comm, HwRules *rules) {
    this->__input.reserve(255);
    this->__nextWord.reserve(255);
    this->__comm = comm;
    this->__rules = rules;
}


/*
 * run()
 * to be called from the main loop 
 * manage incoming message
 *
 */
boolean Protocol::run() {
    HwRuleType type;
    int enableSwitchId; 
    int coilPin;
    int disableSwitchId;
    unsigned int duration;
    int i = 0;
    boolean result;
    result = false;
    if (this->__comm->readLn()) {
        this->__input = this->__comm->input;
        this->__comm->debug("PROTOCOL:" + this->__input);
        
        if (this->getNextWord()) {
            if (this->__nextWord == "RC") {
                // RUL CLR COIL SW
                if (this->getNextWord()) {
                    coilPin = this->__nextWord.toInt();
                if (this->getNextWord()) {
                    enableSwitchId = this->__nextWord.toInt();
                result = this->__rules->clearRule(coilPin, enableSwitchId);
                }}
            }            
            else if (this->__nextWord == "RA") {
                // RUL ADD TYPE SW COIL SW DURATION
                if (this->getNextWord()) {
                    type = (HwRuleType)(this->__nextWord.toInt());
                if (this->getNextWord()) {
                    coilPin = this->__nextWord.toInt();
                if (this->getNextWord()) {
                    enableSwitchId = this->__nextWord.toInt();
                if (this->getNextWord()) {
                    disableSwitchId = this->__nextWord.toInt();
                if (this->getNextWord()) {
                    duration = this->__nextWord.toInt();
                result = this->__rules->addHwRule(type, enableSwitchId, coilPin, disableSwitchId, duration);
                }}}}}
            }        
            else if (this->__nextWord == "DP") {
                // DRV PUL COIL DURATION
                if (this->getNextWord()) {
                    coilPin = this->__nextWord.toInt();
                if (this->getNextWord()) {
                    duration = this->__nextWord.toInt();
                result = this->__rules->addPulse(coilPin, duration);
                }}
            }
            else if (this->__nextWord == "DE") {
                // DRV ENB
                if (this->getNextWord()) {
                    coilPin = this->__nextWord.toInt();
                result = this->__rules->addEnable(coilPin);
                }
            }
            else if (this->__nextWord == "DD") {
                // DRV DIS
                if (this->getNextWord()) {
                    coilPin = this->__nextWord.toInt();
                result = this->__rules->addDisable(coilPin);
                }
            }
            else
                this->__comm->debug("Unknown command " + this->__nextWord);
        }
        this->__input = "";  //clean unread buffer...
        this->__nextWord = "";
         if (!result) 
            this->__comm->error("PROTOCOL:command run failed");
     }
     return result;
}


/*
 * getNextWord()
 * return true if a word is found in __input buffer
 * set __nextWord value
 * !!170419:VG:Creation
 */
boolean Protocol::getNextWord() {
  if (this->__input.length() > 0) {
    int i = this->__input.indexOf(":");
    if (i == -1) {
      this->__nextWord = this->__input;
      this->__input = ""; 
    }
    else
    {
      this->__nextWord = this->__input.substring(0, i);
      this->__input = this->__input.substring(i+1);
    }
    return true;
  }
  else
    return false;
}

