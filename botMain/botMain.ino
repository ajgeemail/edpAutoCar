/**@file */
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
// Direction variables in radians (relative to AVS heading)
#define LEFT 270*(PI/180);
#define FORWARD 0*(PI/180);
#define RIGHT 90*(PI/180);
#define BACKWARD 180*(PI/180);
#define DIAG_FOR_RIGHT 45*(PI/180);
#define DIAG_FOR_LEFT 315*(PI/180);
#define DIAG_BACK_LEFT 225*(PI/180);

// ***** Setup Sensor 1 variables *****
//! Front sensor arduino trigger pin number
const uint8_t frontTriggerPin = 17;
//! Front sensor arduino echo pin number
const uint8_t frontEchoPin = frontTriggerPin;
//! Front sensor x offset on chassis from pozyx location
const float frontXOffset = 0; // cm
//! Front sensor y offset on chassis from pozyx location
const float frontYOffset = 10; // cm
//! Front sensor angle relative to forward of chassis
const float frontsensorAngle = FORWARD;

// ***** Setup Sensor 2 variables *****
//! Left sensor arduino trigger pin number
const uint8_t leftTriggerPin = 15;
//! Left sensor arduino echo pin number
const uint8_t leftEchoPin = leftTriggerPin;
//! Left sensor x offset on chassis from pozyx location
const float leftXOffset = -5; // cm
//! Left sensor y offset on chassis from pozyx location
const float leftYOffset = 2; // cm
//! Left sensor angle relative to forward of chassis
const float leftsensorAngle = LEFT;

// ***** Setup Sensor 3 variables *****
//! Right sensor arduino trigger pin number
const uint8_t rightTriggerPin = 19;
//! Right sensor arduino echo pin number
const uint8_t rightEchoPin = rightTriggerPin;
//! Right sensor x offset on chassis from pozyx location
const float rightXOffset = 5; // cm
//! Right sensor y offset on chassis from pozyx location
const float rightYOffset = 2; // cm
//! Right sensor angle relative to forward of chassis
const float rightsensorAngle = RIGHT;

//! DHT22 Sensor arduino pin number
const uint8_t dhtPin = 14;

// ***** Create sensor objects with relevant info *****
//! Front sensor ObstacleSensor object initialisation - used to detect obstacles at front of AVS
ObstacleSensor frontSensor(frontTriggerPin, frontEchoPin, frontXOffset, frontYOffset, frontsensorAngle);
//! Left sensor ObstacleSensor object initialisation - used to detect obstacles at left of AVS
ObstacleSensor leftSensor(leftTriggerPin, leftEchoPin, leftXOffset, leftYOffset, leftsensorAngle);
//! Right sensor ObstacleSensor object initialisation - used to detect obstacles at right of AVS
ObstacleSensor rightSensor(rightTriggerPin, rightEchoPin, rightXOffset, rightYOffset, rightsensorAngle);


//! Obstacle detection system object initialisation - used to activate ObstacleSensor objects
//! as required and pass on information to the navigation system
ObstacleDetection ods(&frontSensor, &leftSensor, &rightSensor, &nav);
// *********************** END ODS SPECIFIC VARIABLES **************************************************

void setup() 
{
    Serial.begin(9600);
    ObstacleSensor::calculateSoundCm(dhtPin);
    nav.testMap();
    
    // Adds a number of dummy obstacle locations for OD-NM interface testing purposes
    ods.odsToNavTestObstacles();
    nav.printMap();
}

void loop() 
{
    ObstacleSensor::updateOdsData(avsX_, avsY_, avsHeading_);
    ods.detectAllSensors();
    //testBlueToothGrid();
}

//! fake function - to be deleted - just there to see if nav.grid_ array can be accessed from here
void testBlueToothGrid()
{
    //test basic access
    int test = nav.grid_[0][0][0];
}
