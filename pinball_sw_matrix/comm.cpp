/*
 * hw_rule
 * 
 * !!170203:VG:Creation
 * !!170505:VG:manage duplicate messages
 * !!170611:VG:Remove duplicate check to let retry for some commands
 * 
 */

#include "comm.h"


Comm::Comm(const boolean waitSerial) {
  this->__input.reserve(this->MAXBUF);
  this->__previous_input.reserve(this->MAXBUF);
  this->input.reserve(128);
	this->readReset();
  	Serial.begin(115200);
  	if (waitSerial)
    	// wait for serial port to connect. Needed for native USB port only
  		while (!Serial);

}


void Comm::readReset() {
	this->__input = "";
	this->__ready = false;
}


/*
 * read()
 * Poll the serial port
 * '*' is a char used to fill the buffer (f*ckin debug!)
 * '!' is a secondary separator
 * !!170716:VG:Don't trig procotol on a new empty frame
 * !!170729:VG:return true if a new trame is complete
 */
boolean Comm::read() {
    boolean do_wait = false;
    if (this->__ready)
        return false;
    while (Serial.available() > 0) {
        do_wait = true;
		char in = (char) Serial.read();
		if ((in == '\n') || (in == '\r') || (in == '!')) {
        if (this->__input.length() > 0) {
			      this->__ready = true;
            return true;			    
        }
		}
        else
            if (in != '*')
                this->__input += in;
	}
    if (do_wait)
        delay(5);
  return false;        
}


boolean Comm::readLn() {
	if (this->__ready) {
/*
	    if (this->__input == this->__previous_input) {
	        // duplicated message, skip it...
          this->warning("READLN:DUPLICATE:" + this->__input);
          this->readReset();
	        return false;
	    }
*/	
	    this->__previous_input = this->__input;
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


void Comm::ackFrame(const unsigned int nb, const boolean result) {
	Serial.print(F("ACK:"));
  Serial.print(nb);
	Serial.println(result ? F(";OK"):F(";ko"));
}

		
void Comm::debug(String message) {
  Serial.print(F("DBG:"));
  Serial.println(message);
}

void Comm::info(String message) {
  Serial.print(F("INF:"));
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


