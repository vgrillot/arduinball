/*
 * hw_rules
 * 
 * !!170203:VG:Creation
 * !!181104:VG:Add verbose debug output
 * !!181104:VG:Add global watchdog
 * 
 */


#ifndef hw_rules_h
#define hw_rules_h

#include <Arduino.h>
#include "hw_rule.h"
#include "sw_custom.h"
#include "comm.h"

const byte MAX_HW_RULES = 20;
const byte MAX_SW_INPUT = 6;

/*
 * Hardware rules classes
 */

class HwRules
{
  private:
    Comm *__comm;
    HwRule __rules[MAX_HW_RULES];
    SwCustom * __inputs[MAX_SW_INPUT];
    byte __inputCount = 0;
    unsigned int _time;
    unsigned int __watchdogTimeOut;

    boolean isSwitchActive(byte swId);
    boolean existsSwitch(byte swId);
    
    void runRule(HwRule *r);

  public:
    HwRules();
	
    void setComm(Comm *comm);

    void addInput(SwCustom *input);

    boolean addHwRule(HwRuleType type, int enableSwitchId, int coilPin, int disableSwitchId, unsigned int duration);
    boolean addPulse(int coilPin, int duration);
    boolean addEnable(int coilPin);
    boolean addDisable(int coilPin);

    boolean clearRule(int coilPin, int enableSwitchId);
    void  clearAllRules();

    boolean readAll(boolean force_update);
	
    boolean runAll(unsigned int time);
    void stopAll();

    void debugAll();
    void checkWatchdog(boolean changed);
    
    boolean initPlatform();
    boolean haltPlatform();
    
};

#endif

