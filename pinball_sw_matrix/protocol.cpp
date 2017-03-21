/*
 * protocol
 *
 * Manage protocol exchange with RaspPinball platform
 * 
 * !!170314:VG:Creation
 * 
 */


#include "protocol.h"

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
    
    if (this->__comm->readLn()) {
        this->__input = this->__comm->input;
        if (this->getNextWord() && (this->__nextWord == "RUL")) {
            if (this->getNextWord() && (this->__nextWord == "CLR")) {
                // RUL CLR SW COIL
                if (this->getNextWord())
                    enableSwitchId = this->__nextWord.toInt();
                if (this->getNextWord())
                    coilPin = this->__nextWord.toInt();
                this->__rules->clearRule(coilPin, enableSwitchId);
            }
            else if (this->__nextWord == "ADD") {
                // RUL ADD TYPE SW COIL SW DURATION
                if (this->getNextWord())
                    type = (HwRuleType)(this->__nextWord.toInt());
                if (this->getNextWord())
                    enableSwitchId = this->__nextWord.toInt();
                if (this->getNextWord())
                    coilPin = this->__nextWord.toInt();
                if (this->getNextWord())
                    disableSwitchId = this->__nextWord.toInt();
                if (this->getNextWord())
                    duration = this->__nextWord.toInt();
                this->__rules->addHwRule(type, enableSwitchId, coilPin, disableSwitchId, duration);
            }
        }
        else if (this->__nextWord == "DRV") {
            if (this->getNextWord() && (this->__nextWord == "PUL")) {
                // DRV PUL COIL DURATION
                if (this->getNextWord())
                    coilPin = this->__nextWord.toInt();
                if (this->getNextWord())
                    duration = this->__nextWord.toInt();
                this->__rules->addPulse(coilPin, duration);
            }
            else if (this->__nextWord == "ENB") {
                // DRV ENB
                if (this->getNextWord())
                    coilPin = this->__nextWord.toInt();
                this->__rules->addEnable(coilPin);
            }
            else if (this->__nextWord == "DIS") {
                // DRV DIS
                if (this->getNextWord())
                    coilPin = this->__nextWord.toInt();
                this->__rules->addDisable(coilPin);
            }
        }
    }
}


boolean Protocol::getNextWord() {

}

