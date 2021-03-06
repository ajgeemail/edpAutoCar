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
        static const int iterations;
        
    private: 
        //! Fire left sensor only
        //! @return 0 or 1 for legal obstacle distance detected
        int detectLeftSensor();

        //! Fire right sensor only
        //! @return 0 or 1 for legal obstacle distance detected
        int detectRightSensor();

        //! Fire front sensor only
        //! @return 0 or 1 for legal obstacle distance detected
        int detectFrontSensor();
        
    public: 
        //! Default constructor - not used currently
        ObstacleDetection();

        //! Constructor which takes in three ObstacleSensor object pointers and navigator object pointer used to 
        //! activate in different sequences as required
        //! @param *frontSensorPtr  Pointer to front sensor Obstacle Sensor Object
        //! @param *leftSensororPtr  Pointer to left sensor Obstacle Sensor Object
        //! @param *rightSensorPtr  Pointer to right sensor Obstacle Sensor Object
        //! @param *navPtr          Pointer to Navigator object in order to call addObstacle function and pass obstalce
        //! details to navigation system
        ObstacleDetection(ObstacleSensor *frontSensorPtr, ObstacleSensor *leftSensorPtr, ObstacleSensor *rightSensorPtr, Navigator *navPtr);

        //! Standard detection function which activates all sensors and converts any
        //! obstacles to grid references. Will be in loop function so constantly 
        //! firing.
        void detectAllSensors();

        //! Filled with dummy obstacles to be outputted to navigation system for testing of OD-NM interface
        void odsToNavTestObstacles();
        
};

#endif
