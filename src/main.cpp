#include <TinyGPSPlus.h>
#include <TimeLib.h>

// enable console debugging, will print some messages
//#define DEBUG

// enable if PPS pulse is after the time message, else it will add +1s
//#define PPS_AFTER

#ifndef LED_BUILTIN
#define LED_BUILTIN 13
#endif

static const uint32_t GPSBaud = 9600;   // 4800 or 9600
static int ppsPin = 2;                  // arduino pin number, activate on rising
unsigned long ppsTime = millis();       // millisecond on pps pulse
static int triggerPin = 3;              // pin to trigger sending timestamp, activate on falling

TinyGPSPlus gps;
#ifdef GPSHWSERIAL
HardwareSerial& SerialGPS = Serial1;    // Serial port connected to GPS
#else
#include <SoftwareSerial.h>
SoftwareSerial SerialGPS(4, 5);         // Soft serial port connected to GPS
#endif
HardwareSerial& Console = Serial;       // Serial port to computer

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
void printTimestamp(){
  char ts[64];
  sprintf(ts, "%04d-%02d-%02dT%02d:%02d:%02d.%03lu", year(), month(), day(), hour(), minute(), second(), (millis() - ppsTime) % 1000);
  Serial.println(ts);
}

void ppsEvent(void){
  ppsTime = millis();
  if(gps.time.isUpdated())
#ifdef PPS_AFTER
    setTime(gps.time.hour(), gps.time.minute(), gps.time.second(), gps.date.day(), gps.date.month(), gps.date.year());
#else
    setTime(gps.time.hour(), gps.time.minute(), (gps.time.second() + 1) % 60, gps.date.day(), gps.date.month(), gps.date.year());
#endif
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
}

void triggerEvent(){
  printTimestamp();
}

void setup(){
  Console.begin(115200);
  SerialGPS.begin(GPSBaud);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(ppsPin, INPUT_PULLUP);
  pinMode(triggerPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(ppsPin), ppsEvent, RISING);
  attachInterrupt(digitalPinToInterrupt(triggerPin), triggerEvent, FALLING);
}

void loop(){
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
