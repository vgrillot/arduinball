/*
 * hw_rule
 * 
 * !!170203:VG:Creation
 * 
 */

#include "comm.h"


Comm::Comm(const boolean waitSerial) {
	this->__input.reserve(this->MAXBUF);
	this->readReset();
  	Serial.begin(9600);
  	if (waitSerial)
    	// wait for serial port to connect. Needed for native USB port only
  		while (!Serial);

}


void Comm::readReset() {
	this->__bufptr = __buffer;
	this->__buflen = 0;
	this->__input = "";
	this->__ready = false;
}


void Comm::read() {
	while (Serial.available() > 0) {
		char in = (char) Serial.read();
		this->__input += in;
		if {in == '\n') {
			this->__ready = true;
			break;
		}
	}
}


boolean Comm::readLn() {
	if (this->__ready) {
		this->input = this->__input;
		this->readReset();
		return true;
	}
	else
		return false;
}


void Comm::tick(unsigned long count) {
	Serial.print(F("TCK:"));
	Serial.println(count);	
}
	
		
void Comm::debug(String message) {
	Serial.print(F("DBG:"));
	Serial.println(message);
}

void Comm::warning(String warning) {
	Serial.print(F("WRN:"));
	Serial.println(warning);
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
	Serial.print(";");
	Serial.println(active ? 0:1);
	//Serial.print(",");
}

void Comm::writeSwEndUpdate() {
	Serial.println(".");
}


