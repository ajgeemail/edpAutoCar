#ifndef OBSTACLESENSOR_H
#define OBSTACLESENSOR_H

#include "NewPing.h"
class ObstacleSensor
{
    private :
        // Pin definitions
        uint8_t triggerPin_, echoPin_;
        uint8_t dhtPin = 7;
        
        static float hum_;                      // Stores humidity value in percent
        static float temp_;                     // Stores temperature value in Celcius
        static float soundsp_;                  // Stores calculated speed of sound in M/S
        const float soundcm_ = 0.0343;          // Stores calculated speed of sound in cm/ms
        const unsigned int maxDistance_ = 400;  // Maximum distance of 400cm
        
        unsigned long duration_;                // Stores First HC-SR04 pulse duration value
        float objGridRef_[2];                   // objGridRef_[0] = x coordinate, objGridRef_[1] = y coordinate
    
    public :
        // Locations and directions on vehicle relative to Posyx sensor or center or some other reference
        float offsetX_, offsetY_, directionAngle_;

        // Distance from sensor to object detected
        float distance_;

        // Distances from pozyx to object detected
        float objXDist_, objYDist_;

        // NewPing object causes HC-SR04 sensor pulses and enable
        NewPing sonar_;
    
    public :
        // Default constructor - not used
        ObstacleSensor();

        // Constructor reads in pin details as well as relative to car location and direction details
        ObstacleSensor(uint8_t triggerPin, uint8_t echoPin, float relativeX, float relativeY, float relativeDir);

        // Calculates the speed of sound based on humidity and temperature found by DHT22 sensor
        static void calculateSoundCm();

        // Returns distance to obstacle found by this sensor averaged iterations times
        void detectObstacles(uint8_t iterations);
};

#endif
