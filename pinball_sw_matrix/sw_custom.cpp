
/*
 * sw_custom
 * 
 * !!170225:VG:Creation
 * 
 */
 
#include "sw_custom.h"

/*
 * save the comm driver
 */
void SwCustom::setComm(Comm *comm) {
	this->_comm = comm;
  this->_force_update = false;
}

/*
 * acceptSwId
 *
 * return True if the swid is a part of this input
 *
 *!!170307:VG:Creation
 *
 */
boolean SwCustom::acceptSwId(byte swId) {
	return ((swId >= this->_base) && (swId < this->_base + this->_count));
}


/*
 * forceUpdate()
 * read all inputs and send update to master
 * 
 * !!170723:VG:Creation
 * !!181107:VG:FIX: return was called before setting _force_update to false
 * 
 */
boolean SwCustom::forceUpdate() {
    boolean result;
    this->_force_update = true;
    result = this->read();
    this->_force_update = false;
    return result;
}

