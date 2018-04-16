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
#include "ObstacleDetection.h"
#include "common.h"

// **************** AVS SYSTEM WIDE VARIABLES *******************************************************
float avsHeading_ = 0*(PI/180);  // This will need to be updated with pozyx data
float avsX_ = 0;                // This will need to be updated with pozyx data
float avsY_ = 0;                // This will need to be updated with pozyx data
// **************** END AVS SYSTEM WIDE VARIABLES ***************************************************

// *********************** NM SPECIFIC VARIABLES *******************************************************
// Navigation and Maxing System object

Navigator nav;

// *********************** END NM SPECIFIC VARIABLES ***************************************************

// **************** ODS SPECIFIC VARIABLES **********************************************************
ObstacleSensor* frontSensor;
ObstacleSensor* leftSensor;
ObstacleSensor* rightSensor;

// Obstacle detection system object
ObstacleDetection ods(frontSensor, leftSensor, rightSensor, &nav);
// *********************** END ODS SPECIFIC VARIABLES **************************************************

void setup() 
{
    Serial.begin(9600);
    obsDetectSetup();
    ObstacleSensor::updateOdsData(avsX_, avsY_, avsHeading_);
    nav.testMap();
    
    // Adds a number of dummy obstacle locations for OD-NM interface testing purposes
    //ods.odsToNavTestObstacles();
    //nav.printMap();
}

void loop() 
{
    ObstacleSensor::updateOdsData(avsX_, avsY_, avsHeading_);
    ods.detectAllSensors();
    //testBlueToothGrid();
}

// fake function - to be deleted - just there to see if nav.grid_ array can be accessed from here
void testBlueToothGrid()
{
    //test basic access
    int test = nav.grid_[0][0][0];
}

// Sets initial values for ObstacleSensor objects
void obsDetectSetup()
{
    // Direction variables in radians (relative to AVS heading)
    const float LEFT = 270*(PI/180);
    const float FORWARD = 0*(PI/180);
    const float RIGHT = 90*(PI/180);
    const float BACKWARD = 180*(PI/180);
    const float DIAG_FOR_RIGHT = 45*(PI/180);
    const float DIAG_FOR_LEFT = 315*(PI/180);
    const float DIAG_BACK_LEFT = 225*(PI/180);
    const float DIAG_BACK_RIGHT = 135*(PI/180);

    // Setup Sensor 1 variables
    const uint8_t frontTriggerPin = 17;
    const uint8_t frontEchoPin = frontTriggerPin;
    const float frontXOffset = 0; // cm
    const float frontYOffset = 10; // cm
    const float frontsensorAngle = FORWARD;
    
    // Setup Sensor 2 variables
    const uint8_t leftTriggerPin = 15;
    const uint8_t leftEchoPin = leftTriggerPin;
    const float leftXOffset = -5; // cm
    const float leftYOffset = 2; // cm
    const float leftsensorAngle = LEFT;
    
    // Setup Sensor 3 variables
    const uint8_t rightTriggerPin = 19;
    const uint8_t rightEchoPin = rightTriggerPin;
    const float rightXOffset = 5; // cm
    const float rightYOffset = 2; // cm
    const float rightsensorAngle = RIGHT;
    
    // DHT22 Sensor variables
    const uint8_t dhtPin = 14;

    ObstacleSensor::calculateSoundCm(dhtPin);
    
    // Create sensor objects with relevant info
    frontSensor = new ObstacleSensor(frontTriggerPin, frontEchoPin, frontXOffset, frontYOffset, frontsensorAngle);
    leftSensor = new ObstacleSensor(leftTriggerPin, leftEchoPin, leftXOffset, leftYOffset, leftsensorAngle);
    rightSensor = new ObstacleSensor(rightTriggerPin, rightEchoPin, rightXOffset, rightYOffset, rightsensorAngle);
}
