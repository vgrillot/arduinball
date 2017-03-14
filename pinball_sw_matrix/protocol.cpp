/*
 * protocol
 *
 * Manage protocol exchange with RaspPinball platform
 * 
 * !!170314:VG:Creation
 * 
 */


#include "protocol.h"

Protocol::Protocol(Comm *comm; HwRules *rules) {
    this->__input.reserve(255);
    this->__nextWord.reserve(255);
    this->comm = comm;
    this->rules = rules;
}


/*
 * run()
 * to be called from the main loop 
 * manage incoming message
 *
 */
boolean Protocol::run() {
    if (this->__comm->readLn()) {
        this->__input = this->__comm->input;
        if ((this->getNextWord() && (this->__nextWord == "RUL")) {
            if (this->getNextWord() && (this->__nextWord == "CLR")) {
                // RUL CLR
            }
            else if (this->__nextWord == "ADD") {
                // RUL ADD
            }
        }
        else if (this->__nextWord == "DRV") {
            if (this->getNextWord() && (this->__nextWord == "PUL")) {
                // DRV PUL
            }
            else if (this->__nextWord == "ENB") {
                // DRV ENB
            }
            else if (this->__nextWord == "DIS") {
                // DRV DIS
            }
        }
    }
}


boolean Protocol::getNextWord() {

}


boolean Protocol::decode() {


}