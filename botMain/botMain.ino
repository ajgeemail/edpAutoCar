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
#include <Arduino.h>
#include "Navigator.h"
#include "ObstacleDetection.h"
#include "common.h"
#include "ObstacleSensor.h"

// **************** AVS SYSTEM WIDE VARIABLES *******************************************************

// **************** END AVS SYSTEM WIDE VARIABLES ***************************************************

// *********************** NM SPECIFIC VARIABLES *******************************************************
//! Navigation and Maxing System object
Navigator nav;

// *********************** END NM SPECIFIC VARIABLES ***************************************************

// **************** ODS SPECIFIC VARIABLES **********************************************************
//! Obstacle detection system object initialisation - used to activate ObstacleSensor objects
//! as required and pass on information to the navigation system
ObstacleDetection ods(&nav);


// *********************** END ODS SPECIFIC VARIABLES **************************************************


// *********************** LOCATION SYSTEM SPECIFIC VARIABLES ******************************************
int x_loc = 0;
int y_loc = 0;
int heading = 0;
// *********************** END LOCATION SYSTEM SPECIFIC VARIABLES **************************************

void setup() 
{
    Serial.begin(9600);
    while(!Serial);
    nav.testMap();

    // DHT22 Sensor arduino pin number = 14
    ObstacleSensor::calculateSoundCm(14);
    // Adds a number of dummy obstacle locations for OD-NM interface testing purposes
    ods.odsToNavTestObstacles();
    nav.printMap();
}

void loop() 
{
    ObstacleSensor::updateOdsData(x_loc, y_loc, heading);
    ods.detectAllSensors();
}

