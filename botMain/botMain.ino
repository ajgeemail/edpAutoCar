#include "ObstacleSensor.h"

// Direction variables in radians
#define LEFT 270*(PI/180)
#define FORWARD 0*(PI/180)
#define RIGHT 90*(PI/180)
#define FOR_DIAG_RIGHT 45*(PI/180)
#define FOR_DIAG_LEFT 315*(PI/180)
#define BACKWARD 180*(PI/180)

// Number of iterations in calculating average distance
#define iterations 5

// Setup Sensor 1 variables
const uint8_t frontTriggerPin = 5;
const uint8_t frontEchoPin = frontTriggerPin;
const float frontXOffset = 0; // cm
const float frontYOffset = 10; // cm
const float frontDirectionAngle = FORWARD;
float frontDistanceX;
float frontDistanceY;
float frontMeasuredDistance;

// Setup Sensor 2 variables
const uint8_t leftTriggerPin = 10;
const uint8_t leftEchoPin = leftTriggerPin;
const float leftXOffset = -5; // cm
const float leftYOffset = 2; // cm
const float leftDirectionAngle = LEFT;
float leftDistanceX;
float leftDistanceY;
float leftMeasuredDistance;

// Setup Sensor 3 variables
const uint8_t rightTriggerPin = 13;
const uint8_t rightEchoPin = leftTriggerPin;
const float rightXOffset = 5; // cm
const float rightYOffset = 2; // cm
const float rightDirectionAngle = RIGHT;
float rightDistanceX;
float rightDistanceY;
float rightMeasuredDistance;

// Create sensor objects with relevant info
ObstacleSensor frontSensor(frontTriggerPin, frontEchoPin, frontXOffset, frontYOffset, frontDirectionAngle);
ObstacleSensor leftSensor(leftTriggerPin, leftEchoPin, leftXOffset, leftYOffset, leftDirectionAngle);
ObstacleSensor rightSensor(rightTriggerPin, rightEchoPin, rightXOffset, rightYOffset, rightDirectionAngle);

void setup() {
    // put your setup code here, to run once:
    Serial.begin(9600);
}

void loop() {
    // Sensor 1 data
    frontSensor.detectObstacles(iterations);
    frontDistanceX = frontSensor.objXDist_;
    frontDistanceY = frontSensor.objYDist_;
    frontMeasuredDistance = frontSensor.distance_;

    // Sensor 2 data
    leftSensor.detectObstacles(iterations);
    leftDistanceX = leftSensor.objXDist_;
    leftDistanceY = leftSensor.objYDist_;
    leftMeasuredDistance = leftSensor.distance_;

    // Sensor 3 data
    rightSensor.detectObstacles(iterations);
    rightDistanceX = rightSensor.objXDist_;
    rightDistanceY = rightSensor.objYDist_;
    rightMeasuredDistance = rightSensor.distance_;

    // Print in Serial monitor for testing
    printDistance("Front: ", frontMeasuredDistance, frontDistanceX, frontDistanceY);
    printDistance("Left: ", leftMeasuredDistance, leftDistanceX, leftDistanceY);
    printDistance("Right: ", rightMeasuredDistance, rightDistanceX, rightDistanceY);
}

// Prints distances of each sensor's data
void printDistance(String sensorName, float measuredDist, float xDist, float yDist)
{
    Serial.print(sensorName);
    
    if (measuredDist == 1023.0f)
    {
        Serial.println("Distance out of range");
    }
    else 
    {
        Serial.print("Total Distance: ");
        Serial.print(measuredDist);
        Serial.print(" cm\t");
        
        Serial.print("X Distance: ");
        Serial.print(xDist);
        Serial.print(" cm\t");

        Serial.print("Y Distance: ");
        Serial.print(yDist);
        Serial.println(" cm ");
        delay(500);
    }
}


