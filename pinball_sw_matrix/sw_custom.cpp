
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

