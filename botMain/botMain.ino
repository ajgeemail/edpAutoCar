//! \file botMain.ino
//! \brief Top level file for the AVS which integrates all sub-systems on the arduino. 
//!
//! The botMain file will contain the initial setup and loop functions along with all sub-system
//! object initialisations including parameter setting. Ideally all of the sub-systems shall
//! implement separate classes to improve modularity and readability of this top file. Requires
//! specific libraries be downloaded using the Include Library function explained as per INSTRUCTIONS
//! below.

// ****************************************************************************************************************** //
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
//! Heading - to come from pozyx locatio system
float avsHeading_ = 0*(PI/180); // This will need to be updated with pozyx data
float avsX_ = 0;                // This will need to be updated with pozyx data
float avsY_ = 0;                // This will need to be updated with pozyx data
// **************** END AVS SYSTEM WIDE VARIABLES ***************************************************

// *********************** NM SPECIFIC VARIABLES *******************************************************
// Navigation and Maxing System object

Navigator nav;

// *********************** END NM SPECIFIC VARIABLES ***************************************************

// **************** ODS SPECIFIC VARIABLES **********************************************************
// Direction variables in radians (relative to AVS heading)
//! Left direction relative to front of AVS in radians
#define LEFT 270*(PI/180);
//! Forward direction relative to front of AVS in radians
#define FORWARD 0*(PI/180);
//! Right direction relative to front of AVS in radians
#define RIGHT 90*(PI/180);
//! Backward direction relative to front of AVS in radians
#define BACKWARD 180*(PI/180);
//! Forward right diagonal direction relative to front of AVS in radians
#define DIAG_FOR_RIGHT 45*(PI/180);
//! Forward diagonal left direction relative to front of AVS in radians
#define DIAG_FOR_LEFT 315*(PI/180);
//! Backward diagonal left direction relative to front of AVS in radians
#define DIAG_BACK_LEFT 225*(PI/180);
//! Backward diagonal right direction relative to front of AVS in radians
#define DIAG_BACK_RIGHT 135*(PI/180);

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
