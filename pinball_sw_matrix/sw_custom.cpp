
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
 * 
 */
boolean SwCustom::forceUpdate() {
    this->_comm->debug("Sw:forceUpdate id=" + String(this->_id));
    this->_force_update = true;
    return this->read();
    this->_force_update = false;
}

