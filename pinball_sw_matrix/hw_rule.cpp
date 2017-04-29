/*
 * hw_rule
 * 
 * !!170203:VG:Creation
 * 
 */
 
#include "hw_rule.h"


/*
 * rule_type_need_sw()
 * return true if the rule need a switch to run
 * to avoid bad setup
 */
bool rule_type_need_sw(HwRuleType rt) {
  switch(rt) {
    case hw_rule_pulse_on_hit_rule:
    case hw_rule_pulse_on_hit_and_release_rule:
    case hw_rule_pulse_on_hit_and_enable_and_release_rule:
    case hw_rule_pulse_on_hit_and_enable_and_release_and_disable_rule:
      return true;
    case hw_rule_enable:
    case hw_rule_pulse:
      return false;
  }
}




