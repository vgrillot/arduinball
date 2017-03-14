/*
 * protocol
 *
 * Manage protocol exchange with RaspPinball platform
 * 
 * Protocol:
 *
 * RaspPinball>> CNF:
 *          CNF : set config
 * 
 * RaspPinball>> HWR:
 *          HWR : set hardwre rule
 * 
 *
 *
 *
 *
 *
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
        Protocol(Comm *comm; HwRules *rules);
        void decode();
}


 #endif
