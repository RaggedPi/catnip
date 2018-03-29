#include "Pet.h"

/**
 * Constructor
 * @param String       name
 * @param byte         petType
 * @param Dispenser dispenser
 */
Pet::Pet(String name, byte petType, Dispenser dispenser) {
    _dispenseCount = 0;     // initialize
    _lastDispenseTime = 0;  // initialize
    _rfid = "";             // initialize
    _name = "";             // initialize
    _dispenseTimes[0] = now();
    _dispenseTimes[1] = (now() + EIGHT_HOURS); 
    _timeWindow = 0;        // initialize

    _name = name;           // sets name
    _petType = petType;     // sets pet type
    _dispenser = dispenser; // sets dispenser
}

/**
 * Dispense
 */
void Pet::dispense() {
    _dispenser.dispense();
}

/**
 * Get last dispense time
 * @return long
 */
long Pet::getLastDispenseTime() {
    return _lastDispenseTime;
}

/**
 * Set last dispense time
 * @param long dispenseTime
 */
void Pet::setLastDispenseTime(long dispenseTime) {
    _lastDispenseTime = dispenseTime;
}

/**
 * Get dispense count
 * @return int
 */
int Pet::getDispenseCount() {
    return _dispenseCount;
}

/**
 * Rest dispense count
 */
void Pet::resetDispenseCount() {
    _dispenseCount = 0;
}

/**
 * Set dispense time window
 * @param int minutes default: 30
 */
void Pet::setDispenseTimeWindow(int minutes=30) {
    _timeWindow = minutes;
}

/**
 * Get dispense time window
 * @return int
 */
int Pet::getDispenseTimeWindow() {
    return _timeWindow;
}

/**
 * Set dispense times
 * @param time_t dispenseTimes[] array
 */
void Pet::setDispenseTimes(time_t dispenseTimes[]) {
    _dispenseTimes[0] = dispenseTimes[0];
    _dispenseTimes[1] = dispenseTimes[1];
}

/**
 * Get dispense times
 * @return time_t array
 */
time_t* Pet::getDispenseTimes() {
    return _dispenseTimes;
}

/**
 * Set RFID
 * @param String rfid
 */
void Pet::setRFID(String rfid) {
    _rfid = rfid;
}

/**
 * Get RFID
 * @return String
 */
String Pet::getRFID() {
    return _rfid;
}

/**
 * Set name
 * @param String name
 */
void Pet::setName(String name) {
    _name = name;
}

/**
 * Get name
 * @return String
 */
String Pet::getName() {
    return _name;
}