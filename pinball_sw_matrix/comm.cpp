/*
 * hw_rule
 * 
 * !!170203:VG:Creation
 * 
 */

#include "comm.h"


Comm::Comm(const boolean waitSerial) {
  Serial.begin(9600);
  if (waitSerial)
    // wait for serial port to connect. Needed for native USB port only
  	while (!Serial);
}


void Comm::read() {

}

void Comm::printf(char *fmt, ...) {

}
		
		
void Comm::debug(String message) {
	Serial.print(F("DBG:"));
	Serial.println(message);
}

void Comm::error(String error) {
	Serial.print(F("ERR:"));
	Serial.println(error);
}

void Comm::writeSwBeginUpdate() {
	Serial.print(F("SWU:"));
}

void Comm::writeSwUpdate(const int swId, const boolean active) {
  Serial.print(F("SWU:"));
	Serial.print(swId);
	Serial.print("=");
	Serial.println(active ? 0:1);
	//Serial.print(",");
}

void Comm::writeSwEndUpdate() {
	Serial.println(".");
}


