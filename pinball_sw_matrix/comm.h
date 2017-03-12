/*
 * comm
 *
 * Manage serial communication with RaspPinball platform
 * 
 * !!170310:VG:Creation
 * !!170312:VG:Add tick() debug info (main loop count by second)
 * 
 */

#ifndef comm_h
#define comm_h

#include <Arduino.h>

class Comm
{
	private:
		void write(String s);

		void printf(char *fmt, ...);

	public:
		Comm(const boolean waitSerial);

		void read();
		

		void debug(String message);
		void error(String error);

		void tick(unsigned long count);


		void writeSwBeginUpdate();
		void writeSwUpdate(const int swId, const boolean active);
		void writeSwEndUpdate();

};


#endif