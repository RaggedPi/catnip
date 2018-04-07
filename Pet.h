#ifndef PET_H
#define PET_H

#if defined(ARDUINO) && ARDUINO >= 100
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif

#include <Time.h>
#include <Dispenser.h>

/* Delimiters */
#define NONE -1                     // delimter   
#define DOG 0                       // delimiter
#define CAT 1                       // delimiter
#define PUPPY 2                     // delimiter
#define KITTEN 3                    // delimiter
/* Timers */
#define EIGHT_HOURS      7200000    // ms
#define THIRTY_MINUTES   1800000    // ms

class Pet {
    public:
        // Constructor
        Pet(String name, byte type, Dispenser dispenser);
        // Last dispense
        long getLastDispenseTime();
        void setLastDispenseTime(long dispenseTime);
        // Dispense
        void dispense();
        // Dispense count
        int getDispenseCount();
        void resetDispenseCount();
        // Dispense time window
        void setDispenseTimeWindow(int minutes);
        int getDispenseTimeWindow();
        // Dispense times
        void setDispenseTimes(time_t dispenseTimes[]);
        time_t* getDispenseTimes();
        // RFID
        void setRFID(String rfid);
        String getRFID();
        // name
        void setName(String name);
        String getName();
    private:
        Dispenser       _dispenser;         // dispenser
        int             _timeWindow;        // duration
        unsigned long   _lastDispenseTime;  // timestamp
        time_t          _dispenseTimes[2];  // time objects
        byte            _petType;           // delimiter
        String          _rfid;              // rfid
        int             _dispenseCount;     // counter
        String          _name;              // name
};

#endif