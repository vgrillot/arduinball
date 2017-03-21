/*
 * protocol
 *
 * Manage protocol exchange with RaspPinball platform
 * 
 * Protocol:
 *
 * RaspPinball >> Arduino
 *      IDENTITY
 *          >> I?
 *          << I=version
 *      RULES CLEAR
 *          >> C?<coil>
 *          << 
 *      RULES ADD
 *          >> A?<coil>:<>:<>
 *          << 
 *      DRIVER PULSE
 *          >>P?<coil>:
 *      DRIVER ENABLE
 *          >>E?<coil>
 *          <<
 *      DRIVER DISABLE
 *          >>D?<coil>
 *          <<
 *      SWITCH UPDATE     
 *          >>
 *          <<S=<sw>:<state>
 *
 *
 *
 * !!170314:VG:Creation
 *
 */

#ifndef protocol_h
#define protocol_h

#include <Arduino.h>
#include "comm.h"
#include "hw_rules.h"

class Protocol {
    private:
        Comm *__comm;
        HwRules *__rules;
        String __input;
        String __nextWord;
        boolean getNextWord();
    public:
        Protocol(Comm *comm, HwRules *rules);
        boolean run();

};


#endif
