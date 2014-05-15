#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>

// start 
RF24 radio(8, 9); //ce cs  / 3 4  / 14 15
RF24Network network(radio);

// constants
const uint16_t thisNode = 1;//<nodeID>;
const uint16_t masterNode = 0;
const unsigned long interval = 2000; //ms

// pin assignment
// LED
const int blueLed = 10;
const int redLed = 7;

// Sensors
const int lightSensor = A0;
const int tempSensor = A1;

// GPIO
const int GPIO1 = A4;
const int GPIO2 = A5;
const int GPIO3 = 3;
const int GPIO4 = 4;

const int GPIO5 = A3;
const int GPIO6 = A2;
const int GPIO7 = 0;
const int GPIO8 = 1;

struct payloadSensors
{
    float temperature;
    float light;
};

void setup(void)
{
    pinMode(blueLed, OUTPUT);     
    pinMode(redLed, OUTPUT);     

   // pinMode(lightSensor, INPUT);     
   // pinMode(tempSensor, INPUT);  

    pinMode(GPIO1, OUTPUT);  
    pinMode(GPIO2, OUTPUT);
    pinMode(GPIO3, OUTPUT);
    pinMode(GPIO4, OUTPUT);
    pinMode(GPIO5, OUTPUT);
    pinMode(GPIO6, OUTPUT);
    pinMode(GPIO7, OUTPUT);
    pinMode(GPIO8, OUTPUT);

    digitalWrite(blueLed, HIGH);
    digitalWrite(redLed, HIGH);

    SPI.begin();
    radio.begin();
    network.begin(90, thisNode);

    delay(1000);
    digitalWrite(blueLed, LOW);
    digitalWrite(redLed, LOW);
}

void loop(void)
{
    network.update();

    payloadSensors payload = { temperature(), light() };
    RF24NetworkHeader header(masterNode);
    bool ok = network.write(header,&payload,sizeof(payload));
    if (ok)
        digitalWrite(blueLed, HIGH);
    else
        digitalWrite(redLed, HIGH);
    delay(1000);

    digitalWrite(blueLed, LOW);
    digitalWrite(redLed, LOW);
}

float temperature() 
{
    //return (5.0 * analogRead(tempSensor) * 100.0) / 1024;
    return analogRead(tempSensor) * (5.0 / 1023.0);
}

float light()
{
    //return analogRead(lightSensor);
    return analogRead(lightSensor) * (5.0 / 1023.0);
}
