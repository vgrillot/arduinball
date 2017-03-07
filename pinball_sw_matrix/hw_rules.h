/*
 * hw_rules
 * 
 * !!170203:VG:Creation
 * 
 */

#ifndef hw_rules_h
#define hw_rules_h

#include <Arduino.h>
#include "hw_rule.h"
#include "sw_custom.h"


const byte MAX_HW_RULES = 20;
const byte MAX_SW_INPUT = 6;

/*
 * Hardware rules classes
 */

class HwRules
{
  private:
    HwRule __rules[MAX_HW_RULES];
	SwCustom * __inputs[MAX_SW_INPUT];
	byte __inputCount = 0;

    //SwMatrix *matrix;
    unsigned int _time;

    boolean isSwitchActive(byte swId);

  public:
    HwRules();
	
    void addInput(SwCustom *input);

    void addHwRule(HwRuleType type, int enableSwitchId, int coilPin, int disableSwitchId, unsigned int duration);
    void addPulse(int coilPin, int duration);
    void addEnable();

	void readAll();
	
    void runAll(unsigned int time);
    void runRule(HwRule *r);
    void stopAll();
};

#endif

