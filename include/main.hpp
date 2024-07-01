#ifndef MAIN_HPP
#define MAIN_HPP

// enable console debugging, will print some debug messages
//#define DEBUG

// enable if PPS pulse is after the time message, else it will add +1s
//#define PPS_AFTER

// pins that does not need to be defined
// TX: 0
// RX: 1

#ifndef GNSS_BAUD
#define GNSS_BAUD 9600                  // default baud rate from/to GNSS module: 4800 or 9600
#endif

#ifndef CONSOLE_BAUD
#define CONSOLE_BAUD 115200             // default baud rate from/to host computer
#endif

#ifndef LED_BUILTIN
#define LED_BUILTIN 13                  // default LED pin
#endif

#ifndef PPS_PIN
#define PPS_PIN 2                       // default PPS input pin
#endif

#ifndef PPS_MODE
#define PPS_MODE RISING                 // default PPS mode: RISING/FALLING
#endif

#ifndef TRIGGER_PIN
#define TRIGGER_PIN 3                   // default Trigger input pin
#endif

#ifndef TRIGGER_MODE
#define TRIGGER_MODE FALLING            // default trigger mode: RISING/FALLING
#endif

#ifndef SWTXPIN
#define SWTXPIN 4                       // default TX pin for SoftwareSerial (if used)
#endif                                  // will use pin 0 if HardwareSerial

#ifndef SWRXPIN
#define SWRXPIN 5                       // default RX pin for SoftwareSerial (if used)
#endif                                  // will use pin 1 if HardwareSerial

static void smartDelay(unsigned long ms);
void printTimestamp(void);
void ppsEvent(void);
void triggerEvent();
void setup(void);
void loop(void);
#endif