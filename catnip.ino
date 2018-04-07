/*
  Timed Pet Dispenser Library Example
  Written by david durost <david.durost@gmail.com>
*/
/* Includes */
#include <SoftwareSerial.h>         // serial library
#include <Servo.h>                  // servo library
#include "Dispenser/Dispenser.h"    // dispenser library
#include "Pet.h"                    // pet library
#include <Wire.h>                   // one wire library
#include <Time.h>                   // time library
#include "DS3231/DS3231.h"          // rtc library
#include <MFRC522.h>                // rfid library

#define SDA              4          // SDA/ SS
#define SDL              5          // SDL
#define CS               10         // Chipselect
#define LED              13         // LED pin
#define RESET            9          // Reset pin

#define CAT_SERVO_PIN    6          // digital pin
#define DOG_SERVO_PIN    7          // digital pin
#define PUPPY_SERVO_PIN  8          // digital pin

#define VEDA             0          // delimeter
#define TOODLES          1          // delimeter
#define BING             2          // delimeter
#define MOXIE            3          // delimeter
#define ELLIE            4          // delimeter

#define EIGHT_HOURS      7200000    // ms
#define THIRTY_MINUTES   1800000    // ms

/* Variables */
RTClib rtc;                         // read time clock
MFRC522 rfidReader(SDA, RESET);     // rfid rfidReader
Servo servos[3] = {                 // servos
    Servo(), 
    Servo(),
    Servo() 
}; 
Dispenser dispensers[3] = {         // dispensers
    Dispenser(), 
    Dispenser(),
    Dispenser() 
};                              
DateTime cTime;                     // current time
unsigned long uTime;                // current millis
String uid = "";                    // rfid string
Pet pets[5] = {                     // pets
    Pet("Veda", CAT, dispensers[CAT]),
    Pet("Toodles", CAT, dispensers[CAT]),
    Pet("Bing", CAT, dispensers[CAT]),
    Pet("Moxie", DOG, dispensers[DOG]),
    Pet("Ellie", PUPPY, dispensers[PUPPY])
};
time_t feedTimes[2];                // time windows

/**
 * Build time
 * @param  byte hh
 * @param  byte mm
 * @param  byte ss
 */
time_t buildTime(byte hh=hour(), byte mm=minute(), byte ss=0) {
    tmElements_t tmSet;
    tmSet.Year = 2018 - 1970;
    tmSet.Month = 2;
    tmSet.Day = 25;
    tmSet.Hour = hh;
    tmSet.Minute = mm;
    tmSet.Second = ss;

    return makeTime(tmSet);
}

/**
 * In range
 * @param  int val
 * @param  int minimum
 * @param  int maximum
 * @return bool
 */
bool inRange(int val, int minimum, int maximum) {
    return ((minimum <= val) && (val <= maximum));
}

/**
 * Dispense
 * @param  Pet pet
  */
void dispense(Pet pet) {
    pet.dispense();
    pet.setLastDispenseTime(uTime);
}

/**
 * RFID scan
 * @return bool
 */
bool rfidScan() {
    MFRC522::MIFARE_Key key;
    for (byte i=0; i<6; i++)    key.keyByte[i] = 0xFF;
    
    if (! rfidReader.PICC_IsNewCardPresent())   return false;
    if (! rfidReader.PICC_ReadCardSerial()) return false;

    for (byte i=0; i<rfidReader.uid.size; i++) {
        uid.concat(String(rfidReader.uid.uidByte[i] < 0x10 ? "0" : " "));
        uid.concat(String(rfidReader.uid.uidByte[i], HEX));
    }
    return true;
}

/**
 * RFID check
  */
void rfidCheck() {
    for (byte i=0; i<sizeof(pets); i++) {
        if (uid.equals(pets[i].getRFID())) {
            if ((0 == pets[i].getDispenseCount()) || 
              inRange(uTime, feedTimes[0], (feedTimes[0] + pets[i].getDispenseTimeWindow())) ||
              inRange(uTime, feedTimes[1], (feedTimes[1] + pets[i].getDispenseTimeWindow())))
                dispense(pets[i]);
        }
    }
}

/**
 * Reset feed counts
 */
void resetFeedCounts() {
    for (byte i=0; i < sizeof(pets); i++) {
        pets[i].resetDispenseCount();
    }
}

/**
 * Setup
 */
void setup() {
    Serial.begin(9600);
    SPI.begin();
    Wire.begin();
    rfidReader.PCD_Init();
    
    // feed times
    feedTimes[0] = buildTime(6,0);
    feedTimes[1] = buildTime(18,0);
    
    // servos
    servos[CAT].attach(CAT_SERVO_PIN);
    servos[DOG].attach(DOG_SERVO_PIN);
    servos[PUPPY].attach(PUPPY_SERVO_PIN);
    
    // dispensers
    dispensers[CAT] = new Dispenser(servos[CAT]);
    dispensers[DOG] = new Dispenser(servos[DOG]);
    dispensers[PUPPY] = new Dispenser(servos[PUPPY]);

    // pets
    pets[VEDA].setDispenseTimeWindow(THIRTY_MINUTES);
    pets[VEDA].setDispenseTimes(feedTimes);
    pets[VEDA].setRFID("XXX");
    pets[TOODLES] = Pet("Toodles", CAT, dispensers[CAT]);
    pets[TOODLES].setDispenseTimeWindow(THIRTY_MINUTES);
    pets[TOODLES].setDispenseTimes(feedTimes);
    pets[TOODLES].setRFID("XXX");    
    pets[BING] = Pet("Bing", CAT, dispensers[CAT]);
    pets[BING].setDispenseTimeWindow(THIRTY_MINUTES);
    pets[BING].setDispenseTimes(feedTimes);
    pets[BING].setRFID("XXX");        
    pets[MOXIE] = Pet("Moxie", DOG, dispensers[DOG]);
    pets[MOXIE].setDispenseTimeWindow(THIRTY_MINUTES);
    pets[MOXIE].setDispenseTimes(feedTimes);
    pets[MOXIE].setRFID("XXX");
    pets[ELLIE] = Pet("Ellie", PUPPY, dispensers[PUPPY]);
    pets[ELLIE].setDispenseTimeWindow(THIRTY_MINUTES);
    pets[ELLIE].setDispenseTimes(feedTimes);
    pets[ELLIE].setRFID("XXX");
}

/** 
 * Loop
 */
void loop() {
    cTime = now();
    uTime = cTime.unixtime();
    if (rfidScan()) rfidCheck();
    if (0 == hour())    resetFeedCounts();
}