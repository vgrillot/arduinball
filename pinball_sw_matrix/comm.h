/*
 * comm
 *
 * Manage serial communication with RaspPinball platform
 * 
 * !!170310:VG:Creation
 * !!170312:VG:Add tick() debug info (main loop count by second)
 * !!170505:VG:manage duplicate messages
 * !!170506:VG:Ack frame
 *
 */

#ifndef comm_h
#define comm_h

#include <Arduino.h>


class Comm
{
	private:
		static const int MAXBUF = 512;
		String __previous_input;
		String __input;
		boolean __ready;
		void readReset();
		void write(String s);

	public:
        String input;
        Comm(const boolean waitSerial);

        boolean read();

        boolean readLn();


        void debug(String message);
        void info(String message);
        void warning(String warning);
        void error(String error);

        void tick(unsigned long count);

        void ackFrame(const unsigned int nb, const boolean result);

        void writeSwBeginUpdate();
        void writeSwUpdate(const int swId, const boolean active);
        void writeSwEndUpdate();
};


#endif
