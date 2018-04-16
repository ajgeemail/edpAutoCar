#ifndef OBSTACLEDETECTION_H
#define OBSTACLEDETECTION_H

//! \brief ObstacleDetection object controls the activation patterns of all 
//! ObstacleSensor objects and outputs detected obstacle grid references
//! to the navigation system.
//!
//! A single ObstacleDetection object is used per AVS unit, and it contains
//! all of the details of the specific respective ObstacleSensor objects.
//! The ObstacleDetection object has at its disposal a number of ObstacleSensor
//! activation patterns based on the needs of the particular environment and AVS
//! configuration.
//! INPUTS: 
//! Requires information detailing ObstacleSensor objects
//! OUTPUTS TO:
//! Grid reference to navigation system

#include "ObstacleSensor.h"
#include <stdint.h>
#include "Navigator.h"

class ObstacleDetection
{
    private: 
        //! Pointer to front sensor
        ObstacleSensor *frontSensorPtr_;

        //! Pointer to left sensor
        ObstacleSensor *leftSensorPtr_;

        //! Pointer to right sensor
        ObstacleSensor *rightSensorPtr_;

        //! Pointer to the navigation system object
        Navigator *navPtr_;

        //! Number of times the sensors are activated to average distance of detection
        static const uint8_t iterations;
        
    private: 
        //! Fire left sensor only
        //! @return 0 or 1 for legal obstacle distance detected
        uint8_t detectLeftSensor();

        //! Fire right sensor only
        //! @return 0 or 1 for legal obstacle distance detected
        uint8_t detectRightSensor();

        //! Fire front sensor only
        //! @return 0 or 1 for legal obstacle distance detected
        uint8_t detectFrontSensor();

        //! Initalises for the three ObstacleSensor objects with all required data including pin numbers, sensor
        //! angles and offsets
        void setupSensors1();
        
    public: 
        //! Default constructor - not used currently
        ObstacleDetection();

        //! Constructor sets up Navigator object
        //! @param *navPtr          Pointer to Navigator object in order to call addObstacle function and pass obstalce
        //! details to navigation system
        ObstacleDetection(Navigator *navPtr);

        //! Standard detection function which activates all sensors and converts any
        //! obstacles to grid references. Will be in loop function so constantly 
        //! firing.
        void detectAllSensors();

        //! Filled with dummy obstacles to be outputted to navigation system for testing of OD-NM interface
        void odsToNavTestObstacles();
        
};

#endif
