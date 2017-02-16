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
#include "sw_matrix.h" 

const byte MAX_HW_RULES = 20;

/*
 * Hardware rules classes
 */

class HwRules
{
  private:
    HwRule rules[MAX_HW_RULES];
    SwMatrix *matrix;
    unsigned int _time;

    boolean isSwitchActive(byte swId);

  public:
    HwRules();
    void setSwMatrix(SwMatrix *matrix);

    void addHwRule(HwRuleType type, int enableSwitchId, int coilPin, int disableSwitchId, unsigned int duration);
    void addPulse(int coilPin, int duration);
    void addEnable();

    void runAll(unsigned int time);
    void runRule(HwRule *r);
    void stopAll();
};

#endif

