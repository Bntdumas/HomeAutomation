#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>

#include "my_settings.h"

#define ENABLE_LEDS // %LEDS_ENABLED%

// start 
RF24 radio(8, 9); //ce cs  / 3 4  / 14 15
RF24Network network(radio);

// Time to read the network 4 times and write one time 
const unsigned long readWriteCycle = %READ_WRITE_CIRCLE%; //ms
int currentIteration = 0;


// pin assignment
// LED
const int blueLed = 10;
const int redLed = 7;

// Sensors
// %SENSORS%

// GPIO
const int GPIO1 = A4;
const int GPIO2 = A5;
const int GPIO3 = 3;
const int GPIO4 = 4;

const int GPIO5 = A3;
const int GPIO6 = A2;
const int GPIO7 = 0;
const int GPIO8 = 1;


void setup(void)
{
    pinMode(blueLed, OUTPUT);     
    pinMode(redLed, OUTPUT);     

    // %PINMODE%
    
#if ENABLE_LEDS
    digitalWrite(blueLed, HIGH);
    digitalWrite(redLed, HIGH);
#endif

    SPI.begin();
    radio.begin();
    network.begin(90, thisNode);

    delay(1000);
    
#if ENABLE_LEDS
    digitalWrite(blueLed, LOW);
    digitalWrite(redLed, LOW);
#endif
}

void loop(void)
{
    network.update();
    
    // Receive data
    if (currentIteration >= 4) {
         while ( network.available() )
         {
            RF24NetworkHeader header;
            payloadInput payload;
            network.read(header,&payload,sizeof(payload));
            updateOutputs(payload);
#if ENABLE_LEDS
            digitalWrite(blueLed, HIGH);   
#endif 
        }
        currentIteration = 0;
    }
    
    // Send data
    payloadOutput payload = generatePayload();
    RF24NetworkHeader header(masterNode);
    bool ok = network.write(header,&payload,sizeof(payload));
#if ENABLE_LEDS
    if (ok)
        digitalWrite(blueLed, HIGH);
#endif

    delay(readWriteCycle / 5);
#if ENABLE_LEDS
    digitalWrite(blueLed, LOW);
    digitalWrite(redLed, LOW);
#endif
    currentIteration++;
}

void updateOutputs(payload) 
{
    // %UPDATE_OUTPUTS%
}

payloadOutput generatePayload()
{
    // %GENERATE_PAYLOAD%
}


// %FUNCTIONS%

