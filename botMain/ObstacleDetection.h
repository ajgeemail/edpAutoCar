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
#include "Navigator.h"

class ObstacleDetection
{
    private: 
        // Pointers to sensors
        ObstacleSensor *frontSensorPtr_;
        ObstacleSensor *leftSensorPtr_;
        ObstacleSensor *rightSensorPtr_;

        // Pointer to the navigation system object
        Navigator *navPtr_;

        // Number of times the sensors are activated to average distance of detection
        static const uint8_t iterations;
        
    private: 
        // Functions for individual sensor activations
        uint8_t detectLeftSensor();
        uint8_t detectRightSensor();
        uint8_t detectFrontSensor();
        
    public: 
        // Default constructor
        ObstacleDetection();

        // Constructor which takes in three ObstacleSensor object pointers
        ObstacleDetection(ObstacleSensor *frontSensorPtr, ObstacleSensor *leftSensorPtr, ObstacleSensor *rightSensorPtr, Navigator *navPtr);

        // Standard detection function which activates all sensors and converts any
        // obstacles to grid references. Will be in loop function so constantly 
        // firing.
        void detectAllSensors();

        // Filled with dummy obstacles to be outputted to navigation system for testing of OD-NM interface
        void odsToNavTestObstacles();
        
};

#endif
