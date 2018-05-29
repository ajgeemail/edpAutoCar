#ifndef OBSTACLESENSOR_H
#define OBSTACLESENSOR_H

//! \brief ObstacleSensor object contains all angles and distances/locations of a particular sensor
//! allowing for distance calculations from the pozyx locator to be conducted.
//!
//! The ObstacleSensor object represents a single sensor on the AVS and as such
//! multiple ObstacleSensor objects will exist in the botMain and ObstacleDetection
//! modules. The ObstacleSensor class controls all of the sensor calculations
//! to determine the distance to an obstacle and its grid reference. Once grid
//! reference is determined the ObstacleDetection object will confirm its
//! accuracy and legality.
//! INPUTS: 
//! Still require pozyx heading angle (interface between sensor and pozyx)
//! Humidity and temperature details from DHT-22 for improved accuracy
//! Interfaces with HC-SR04 sensors via NewPing class
//! OUTPUTS TO:
//! Objects used by ObstacleDetection class to output grid references to nav.


#include "NewPing.h"
class ObstacleSensor
{
    private :
        //! Pin definitions
        uint8_t triggerPin_, echoPin_;

        //! Stores calculated speed of sound in cm/ms - defaults to speed at 20 degrees Celcius
        static float soundcm_;

        //! Maximum distance of sensor (i.e. 400cm)
        const unsigned int maxDistance_ = 400;  

        //! Stores First HC-SR04 pulse duration value
        unsigned long duration_;

        //! objGridRef_[0] = x coordinate, objGridRef_[1] = y coordinate
        float objGridRef_[2];                   

        //! AVS pozyx heading data
        static float heading_;

        //! AVS pozyx x position
        static int xPos_;

        //! AVS pozyx y position
        static int yPos_;
    
    public :
        //! Locations and directions on vehicle relative to Posyx sensor or center or some other reference
        float offsetX_, offsetY_, sensorAngle_;

        //! Angle of sensor relative to the grid (i.e. sensorAngle_ + heading_)
        float sensorGridAngle_;

        //! Unit vector of vector from sensor to detected obstacle (i.e xComp + yComp)
        int8_t unitVect_[2];

        //! Distance from sensor to object detected
        float distance_;

        //! Distances from pozyx to object detected
        float objXDist_, objYDist_;

        //! Grid coordinates at location object detected
        float objX_, objY_;

        //! Grid reference of obstacle detected converted from coordinates above
        int gridX_, gridY_;

        //! Bias for grid reference conversion
        static int8_t bias_;

        //! NewPing object causes HC-SR04 sensor pulses and enable
        NewPing sonar_;
    
    public :
        //! Default constructor - not used
        ObstacleSensor();

        //! Constructor reads in pin details as well as relative to car location and direction details
        //! @param triggerPin   Arduino pin number of sensor trigger
        //! @param echoPin      Arduino pin number of sensor echo
        //! @param offsetX      Left or right offset from pozyx placement on AVS chassis
        //! @param offsetY      Forward or backward offset from pozyx placement on AVS chassis
        //! @param sensorAngle  Angle sensor is placed at relative to forward direction of AVS chassis
        ObstacleSensor(uint8_t triggerPin, uint8_t echoPin, float offsetX, float offsetY, float sensorAngle);

        //! Calculates the speed of sound based on humidity and temperature found by DHT22 sensor with paramater dhtPin
        //! which is the arduino pin assigned to be the data input of the dht. Will be used in setup function so 
        //! only accessed once at the start. This could be changed if conditions are expected to vary throughout
        //! use time.
        //! @param dhtPin Arduino connected pin number of DHT sensor
        static void calculateSoundCm(uint8_t dhtPin);

        //! Calculates x and y components to obstacle found by this sensor averaged iterations times
        //! by taking into account the heading of the vehicle, the offset of the sensor relative to the
        //! pozyx tag, the angle of the sensor relative to the vehicle and the distance the sensor
        //! calculates relative to itself. Returns 0 if no obstacle detected, 1 if obstacle detected
        //! @param iterations   Number of iterations sensor measures distance to calculate average over
        //! @return 0 or 1 for legal obstacle distance detected
        uint8_t activateSensor(uint8_t iterations);

        //! Prints on serial monitor the detected sensor distance, and converted x and y components relative to pozyx system
        //! @param sensorName   Name of sensor to be printed
        void printDistance(String sensorName);

        // Prints speed of sound details
        //! @param temp Calculated temperature
        //! @param hum  Calcualted humidity
        static void printSound(float temp, float hum);

        //! Updates all sensor data. Will be in loop function so constnatly firing
        //! @param x        Current X location of pozyx
        //! @param y        Current Y location of pozyx
        //! @param heading  Current heading of pozyx
        static void updateOdsData(int x, int y, int heading)
        {
            xPos_ = x/10; // cm
            yPos_ = y/10; // cm
            heading_ = heading;
        }        
};

#endif
