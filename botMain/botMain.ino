// ****************************************************************************************************************** //
// ***** botMain.ino - Main .ino file for arduino (for now, someone else may have a better one)                 ***** //
// ***** INSTRUCTIONS:                                                                                          ***** //
// ***** 1. Ensure all libraries are installed by navigating Sketch > Include Library > Manage Libraries        ***** //
// *****    and searching for the following libraries:  (i)     DHT                                             ***** //
// *****                                                (ii)    Adafuit Unified Sensor                          ***** //
// *****                                                (iii)   Adafruit AM2315                                 ***** //
// ***** 2. 
// ****************************************************************************************************************** //

#include "ObstacleSensor.h"
#include "StackList.h"

// **************** AVS SYSTEM WIDE VARIABLES ******************
float avsHeading_ = 0*(PI/180);  // This will need to be updated with pozyx data
float avsX_ = 0;        // This will need to be updated with pozyx data
float avsY_ = 0;        // This will need to be updated with pozyx data

// **************** ODS SPECIFIC VARIABLES *********************
// Direction variables in radians (relative to AVS heading)
#define LEFT 270*(PI/180)
#define FORWARD 0*(PI/180)
#define RIGHT 90*(PI/180)
#define BACKWARD 180*(PI/180)
#define DIAG_FOR_RIGHT 45*(PI/180)
#define DIAG_FOR_LEFT 315*(PI/180)
#define DIAG_BACK_LEFT 225*(PI/180)
#define DIAG_BACK_RIGHT 135*(PI/180)

// Number of iterations in calculating average distance
#define iterations 5

// Setup Sensor 1 variables
const uint8_t frontTriggerPin = 17;
const uint8_t frontEchoPin = frontTriggerPin;
const float frontXOffset = 0; // cm
const float frontYOffset = 10; // cm
const float frontsensorAngle = FORWARD;
float frontDistanceX;
float frontDistanceY;
float frontMeasuredDistance;

// Setup Sensor 2 variables
const uint8_t leftTriggerPin = 15;
const uint8_t leftEchoPin = leftTriggerPin;
const float leftXOffset = -5; // cm
const float leftYOffset = 2; // cm
const float leftsensorAngle = LEFT;
float leftDistanceX;
float leftDistanceY;
float leftMeasuredDistance;

// Setup Sensor 3 variables
const uint8_t rightTriggerPin = 19;
const uint8_t rightEchoPin = rightTriggerPin;
const float rightXOffset = 5; // cm
const float rightYOffset = 2; // cm
const float rightsensorAngle = RIGHT;
float rightDistanceX;
float rightDistanceY;
float rightMeasuredDistance;

// DHT22 Sensor variables
const uint8_t dhtPin = 14;

// Create sensor objects with relevant info
ObstacleSensor frontSensor(frontTriggerPin, frontEchoPin, frontXOffset, frontYOffset, frontsensorAngle);
ObstacleSensor leftSensor(leftTriggerPin, leftEchoPin, leftXOffset, leftYOffset, leftsensorAngle);
ObstacleSensor rightSensor(rightTriggerPin, rightEchoPin, rightXOffset, rightYOffset, rightsensorAngle);
// *********************** END ODS SPECIFIC VARIABLES **************************************************

void setup() 
{
    Serial.begin(9600);
    ObstacleSensor::calculateSoundCm(dhtPin);
    ObstacleSensor::updateOdsData(avsX_, avsY_, avsHeading_);
}

void loop() 
{
    ObstacleSensor::updateOdsData(avsX_, avsY_, avsHeading_);
    detectAllSensors();
    //detectFrontSensor();
}

// Completes measurements from all sensors *** IN THE FUTURE MIGHT WRITE FUNCTIONS TO USE DIFFERENT COMBINATIONS OF THE SENSORS AS NEEDED ***
void detectAllSensors()
{
    uint8_t frontMeasured = 0;
    uint8_t leftMeasured = 0;
    uint8_t rightMeasured = 0;    
    
    // Sensor 1 data
    frontMeasured = frontSensor.detectObstacles(iterations);
    frontDistanceX = frontSensor.objXDist_;
    frontDistanceY = frontSensor.objYDist_;
    frontMeasuredDistance = frontSensor.distance_;

    // Allocate obstacle location
    if (frontMeasured == 1)
    {
        Serial.print("Front: [x, y] = [");
        Serial.print(frontSensor.objX_);
        Serial.print(", ");
        Serial.print(frontSensor.objY_);
        Serial.println("]");

        int8_t frontObstacleX = (frontSensor.objX_ + 17*frontSensor.unitVect_[0])/50;
        int8_t frontObstacleY = (frontSensor.objY_ + 17*frontSensor.unitVect_[1])/50;

        Serial.print("FrontGrid: [x, y] = [");
        Serial.print(frontObstacleX);
        Serial.print(", ");
        Serial.print(frontObstacleY);
        Serial.println("]");

        //Brady.addObstacle(frontObstacleX, frontObstacleY);
    }

    // Wait for sensor 1 to finish to begin sensor 2 measurements
    if (frontMeasured == 1 || frontMeasured == 0)
    {
        // Sensor 2 data
        leftMeasured = leftSensor.detectObstacles(iterations);
        leftDistanceX = leftSensor.objXDist_;
        leftDistanceY = leftSensor.objYDist_;
        leftMeasuredDistance = leftSensor.distance_;

        // Allocate obstacle location
        if (leftMeasured == 1)
        {
            
            Serial.print("Left: [x, y] = [");
            Serial.print(leftSensor.objX_);
            Serial.print(", ");
            Serial.print(leftSensor.objY_);
            Serial.println("]");

            int8_t leftObstacleX = (leftSensor.objX_ + 17*leftSensor.unitVect_[0])/50;
            int8_t leftObstacleY = (leftSensor.objY_ + 17*leftSensor.unitVect_[1])/50;

            Serial.print("LeftGrid: [x, y] = [");
            Serial.print(leftObstacleX);
            Serial.print(", ");
            Serial.print(leftObstacleY);
            Serial.println("]");

            //Brady.addObstacle(leftObstacleX, leftObstacleY);
        }

        // Wait for sensor 2 to finish to begin sensor 3 measurements
        if (leftMeasured == 1 || leftMeasured == 0)
        {
            // Sensor 3 data
            rightMeasured = rightSensor.detectObstacles(iterations);
            rightDistanceX = rightSensor.objXDist_;
            rightDistanceY = rightSensor.objYDist_;
            rightMeasuredDistance = rightSensor.distance_;

            // Allocate obstacle location
            if (rightMeasured == 1)
            {
                
                Serial.print("Right: [x, y] = [");
                Serial.print(rightSensor.objX_);
                Serial.print(", ");
                Serial.print(rightSensor.objY_);
                Serial.println("]");

                int8_t rightObstacleX = (rightSensor.objX_ + 17*rightSensor.unitVect_[0])/50;
                int8_t rightObstacleY = (rightSensor.objY_ + 17*rightSensor.unitVect_[1])/50;

                Serial.print("RightGrid: [x, y] = [");
                Serial.print(rightObstacleX);
                Serial.print(", ");
                Serial.print(rightObstacleY);
                Serial.println("]");

                //Brady.addObstacle(rightObstacleX, rightObstacleY);
            }
        }
    }

    //frontSensor.printDistance("Front: ");
    //leftSensor.printDistance("Left:  ");
    //rightSensor.printDistance("Right: ");
}

// Detects with only left sensor
uint8_t detectLeftSensor()
{
    uint8_t measured = leftSensor.detectObstacles(iterations);
    leftSensor.printDistance("Left:  ");
    return measured;
}

// Detects with only right sensor
uint8_t detectRightSensor()
{
    uint8_t measured = rightSensor.detectObstacles(iterations);
    rightSensor.printDistance("Right: ");
    return measured;
}

// Detects with only front sensor
uint8_t detectFrontSensor()
{
    uint8_t measured = frontSensor.detectObstacles(iterations);
    frontSensor.printDistance("Front: ");
    return measured;
}


