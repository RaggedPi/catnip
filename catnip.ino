/*
  Timed Pet Feeder Library Example
  Written by david durost <david.durost@gmail.com>
*/
/* Includes */
#include <SoftwareSerial.h>     // serial library
#include <Servo.h>              // servo library
#include "Feeder/Feeder.h"             // pet feeder library
#include <Wire.h>               // one wire library
#include <Time.h>               // time library
#include <DS3231.h>             // rtc library
#include <MFRC522.h>            // rfid library

#define SDA              4      // SDA/ SS
#define SDL              5      // SDL
#define CS               10     // Chipselect
#define LED              13     // LED pin
#define RESET            9      // Reset pin
#define CAT_SERVO_PIN    6      // digital pin
#define DOG_SERVO_PIN    7      // digital pin
#define CAT              0      // delimeter
#define DOG              1      // delimeter
#define VEDA             0      // delimeter
#define TOODLES          1      // delimeter
#define BING             2      // delimeter
#define MOXIE            3      // delimeter
#define ELLIE            4      // delimeter
#define EIGHT_HOURS      7200000 // ms

/* Structs */
typedef struct {
    String name;
    String rfid;
    byte type;
    int feedings = 0;
    unsigned long lastFeedTime;
} Pet;

RTClib rtc;                     // real time clock
MFRC522 rfidReader(SDA, RESET); // rfid rfidReader
Servo servos[2] = { Servo(), Servo() }; // servos
Feeder feeders[2] = { Feeder(), Feeder() };              // feeders
DateTime cTime;                 // current time
unsigned long uTime;            // current millis
String uid = "";                // rfid string
Pet pets[3];                    // pets
time_t feedTimes[3];            // time windows

/**
 * Build time
 * @param  byte hh
 * @param  byte mm
 * @param  byte ss
 */
time_t tmBuildTime(byte hh=hour(), byte mm=minute(), byte ss=0) {
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
    feeders[pet.type].dispense();
    pet.feedings++;
    pet.lastFeedTime = rtc.now().unixtime();
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
        if (uid.equals(pets[i].rfid)) {
            if ((0 == pets[i].feedings) || 
                (EIGHT_HOURS < (uTime - pets[i].lastFeedTime)))
            dispense(pets[i]);
        }
    }
}

/**
 * Reset feed counts
 */
void resetFeedCounts() {
    for (byte i=0; i<sizeof(pets); i++) {
        pets[i].feedings = 0;
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
    feedTimes[0] = tmBuildTime(6,0);
    feedTimes[1] = tmBuildTime(18,0);
    
    // servos
    servos[CAT].attach(CAT_SERVO_PIN);
    servos[DOG].attach(DOG_SERVO_PIN);
    
    // feeders
    feeders[CAT] = new Feeder(servos[CAT]);
    feeders[DOG] = new Feeder(servos[DOG]);
    
    // pets
    pets[VEDA].name = "Veda";
    pets[VEDA].type = CAT;
    pets[VEDA].rfid = "XXX";
    pets[TOODLES].name = "Toodles";
    pets[TOODLES].type = CAT;
    pets[TOODLES].rfid = "XXX";
    pets[BING].name = "Bing";
    pets[BING].type = CAT;
    pets[BING].rfid = "XXX";
    pets[MOXIE].name = "Moxie";
    pets[MOXIE].type = DOG;
    pets[MOXIE].rfid = "XXX";
    pets[ELLIE].name = "Ellie";
    pets[ELLIE].type = DOG;
    pets[ELLIE].rfid = "XXX";
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