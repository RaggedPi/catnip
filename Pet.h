#ifndef PET_H
#define PET_H

#include "Arduino.h"

#include <Time.h>
#include "Dispenser/Dispenser.h"

#define NONE
#define DOG 0
#define CAT 1
#define PUPPY 2
#define KITTEN 3

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
        void restDispenseCount();
        // Dispense time window
        void setDispenseTimeWindow(int minutes);
        int getDispenseTimeWindow();
        // Dispense times
        void setDispenseTimes(time_t dispenseTimes[]);
        time_t getDispenseTimes();
        // RFID
        void setRFID(String rfid);
        String getRFID();
        // name
        void setName(String name);
        String getName();
    private:
        Dispenser _dispenser;
        int _timeWindow;
        unsigned long _lastDispenseTime;
        time_t _dispenseTimes[2];
        byte _petType;
        String _rfid;
        int _dispenseCount;
        String _name;
};

#endif