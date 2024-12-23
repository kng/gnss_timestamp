#include <TinyGPSPlus.h>
#include <TimeLib.h>
#include "main.hpp"

unsigned long ppsTime = millis();       // millisecond on pps pulse
TinyGPSPlus gps;
#ifdef GPSHWSERIAL
HardwareSerial& SerialGPS = Serial1;    // Serial port connected to GPS
#else
#include <SoftwareSerial.h>
SoftwareSerial SerialGPS(SWRXPIN, SWTXPIN);  // Soft serial port connected to GPS
#endif
HardwareSerial& Console = Serial;       // Serial port to computer

void setup(void){
  Console.begin(CONSOLE_BAUD);
  SerialGPS.begin(GNSS_BAUD);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(PPS_PIN, INPUT_PULLUP);
  pinMode(TRIGGER_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PPS_PIN), ppsEvent, PPS_MODE);
  attachInterrupt(digitalPinToInterrupt(TRIGGER_PIN), triggerEvent, TRIGGER_MODE);
}

void loop(void){
  // print timestamp if serial data is received
  if(Console.available() > 0){
    printTimestamp();
    while (Console.available()) Console.read();
  }
#ifdef DEBUG
  char str[64];
  sprintf(str, "Valid: %1d, Sats: %02d", gps.satellites.isValid(), gps.satellites.value());
  Console.println(str);
  printTimestamp();
  smartDelay(1000);
#else
  smartDelay(0);
#endif
}

// This custom version of delay() ensures that the gps object is being "fed".
static void smartDelay(unsigned long ms){
  unsigned long start = millis();
  do {
    while (SerialGPS.available())
      gps.encode(SerialGPS.read());
  } while (millis() - start < ms);
}

// print time in ISO 8601 format
// YYYY-MM-DDTHH:MM:SS.fff
void printTimestamp(void){
  char ts[64];
  noInterrupts();
  sprintf(ts, "%04d-%02d-%02dT%02d:%02d:%02d.%03lu", year(), month(), day(), hour(), minute(), second(), (millis() - ppsTime) % 1000);
  interrupts();
  Serial.println(ts);
}

void ppsEvent(void){
  ppsTime = millis();
  if(gps.time.isUpdated()){
#ifdef PPS_AFTER
    setTime(gps.time.hour(), gps.time.minute(), gps.time.second(), gps.date.day(), gps.date.month(), gps.date.year());
#else
    setTime(gps.time.hour(), gps.time.minute(), (gps.time.second() + 1) % 60, gps.date.day(), gps.date.month(), gps.date.year());
#endif
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  }
}

void triggerEvent(){
  printTimestamp();
}
