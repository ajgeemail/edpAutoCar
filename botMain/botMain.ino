#include "ObstacleSensor.h"

uint8_t triggerPin = 10;
uint8_t echoPin = 10;

ObstacleSensor frontSensor(triggerPin, echoPin, 0, 0, 0);
uint8_t iterations = 5;

void setup() {
    // put your setup code here, to run once:
    
}

void loop() {
    // put your main code here, to run repeatedly:
    frontSensor.detectObstacles(iterations);
}
