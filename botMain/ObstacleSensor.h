#ifndef OBSTACLESENSOR_H
#define OBSTACLESENSOR_H

#include "NewPing/src/NewPing.h"
class ObstacleSensor
{
    private :
        // Pin definitions
        uint8_t triggerPin_, echoPin_;
        uint8_t dhtPin = 7;
        
        static float hum_;    // Stores humidity value in percent
        static float temp_;   // Stores temperature value in Celcius
        static float soundsp_;  // Stores calculated speed of sound in M/S
        const static float soundcm_ = 0.0343;  // Stores calculated speed of sound in cm/ms
        const static unsigned int maxDistance_ = 400; // Maximum distance of 400cm
        
        unsigned long duration_; // Stores First HC-SR04 pulse duration value
        float distance_; // Stores calculated distance in cm for First Sensor
    
    public :
        // Locations and directions on vehicle relative to Posyx sensor or center or some other reference
        float relativeX_, relativeY_, relativeDir_;

        NewPing sonar_;
    
    public :
        ObstacleSensor(uint8_t triggerPin, uint8_t echoPin, float relativeX, float relativeY, float relativeDir);

        // Calculates the speed of sound based on humidity and temperature found by DHT22 sensor
        static void calculateSoundCm();

        // Returns distance to obstacle found by this sensor averaged iterations times
        float detectObstacles(byte iterations);
};

#endif
