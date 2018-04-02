#ifndef OBSTACLEDETECTION_H
#define OBSTACLEDETECTION_H

// ObstacleDetection object controls the activation patterns of all 
// ObstacleSensor objects and outputs detected obstacle grid references
// to the navigation system.
// INPUTS: 
// Requires information detailing ObstacleSensor objects
//
// OUTPUTS TO:
// Grid reference to navigation system

#include "ObstacleSensor.h"
#include <stdint.h>

class ObstacleDetection
{
    private: 
        // Pointers to sensors
        ObstacleSensor *frontSensorPtr_;
        ObstacleSensor *leftSensorPtr_;
        ObstacleSensor *rightSensorPtr_;
        
    private: 
        // Functions for individual sensor activations
        uint8_t detectLeftSensor();
        uint8_t detectRightSensor();
        uint8_t detectFrontSensor();
        
    public: 
        ObstacleDetection();
        ObstacleDetection(ObstacleSensor *frontSensorPtr, ObstacleSensor *leftSensorPtr, ObstacleSensor *rightSensorPtr);
        void detectAllSensors();
        
};

#endif