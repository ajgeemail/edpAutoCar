#ifndef OBSTACLESENSOR_H
#define OBSTACLESENSOR_H

// ObstacleSensor object contains all angles and distances/locations of a particular sensor
// allowing for distance calculations from the pozyx locator to be conducted.
// INPUTS: 
// Still require pozyx yaw angle (interface between sensor and pozyx)
// Humidity and temperature details from DHT-22 for improved accuracy
// Interfaces with HC-SR04 sensors via NewPing class
//
// OUTPUTS TO:
// Objects used by ObstacleDetection class to output grid references to nav.


#include "NewPing.h"
class ObstacleSensor
{
    private :
        // Pin definitions
        uint8_t triggerPin_, echoPin_;

        // Stores calculated speed of sound in cm/ms
        static float soundcm_;
        
        const unsigned int maxDistance_ = 400;  // Maximum distance of 400cm
        
        unsigned long duration_;                // Stores First HC-SR04 pulse duration value
        float objGridRef_[2];                   // objGridRef_[0] = x coordinate, objGridRef_[1] = y coordinate

        // AVS pozyx data
        static float yaw_;
        static float xPos_;
        static float yPos_;
    
    public :
        // Locations and directions on vehicle relative to Posyx sensor or center or some other reference
        float offsetX_, offsetY_, sensorAngle_;

        // Angle of sensor relative to the grid (i.e. sensorAngle_ + yaw_)
        float sensorGridAngle_;

        // Unit vector of vector from sensor to detected obstacle (i.e xComp + yComp)
        int8_t unitVect_[2];

        // Distance from sensor to object detected
        float distance_;

        // Distances from pozyx to object detected
        float objXDist_, objYDist_;

        // Grid coordinates at location object detected
        float objX_, objY_;

        // Grid reference of obstacle detected converted from coordinates above
        int gridX_, gridY_;

        // Bias for grid reference conversion
        static int8_t bias_;

        // NewPing object causes HC-SR04 sensor pulses and enable
        NewPing sonar_;
    
    public :
        // Default constructor - not used
        ObstacleSensor();

        // Constructor reads in pin details as well as relative to car location and direction details
        ObstacleSensor(uint8_t triggerPin, uint8_t echoPin, float offsetX, float offsetY, float sensorAngle);

        // Calculates the speed of sound based on humidity and temperature found by DHT22 sensor with paramater dhtPin
        // which is the arduino pin assigned to be the data input of the dht. Will be used in setup function so 
        // only accessed once at the start. This could be changed if conditions are expected to vary throughout
        // use time.
        static void calculateSoundCm(uint8_t dhtPin);

        // Calculates x and y components to obstacle found by this sensor averaged iterations times
        // by taking into account the yaw of the vehicle, the offset of the sensor relative to the
        // pozyx tag, the angle of the sensor relative to the vehicle and the distance the sensor
        // calculates relative to itself. Returns 0 if no obstacle detected, 1 if obstacle detected
        uint8_t activateSensor(uint8_t iterations);

        // Prints on serial monitor the detected sensor distance, and converted x and y components relative to pozyx system
        void printDistance(String sensorName);

        // Prints speed of sound details
        static void printSound(float temp, float hum);

        // Updates all sensor data. Will be in loop function so constnatly firing
        static void updateOdsData(float x, float y, float heading)
        {
            xPos_ = x;
            yPos_ = y;
            yaw_ = heading;
        }

        

        
};

#endif
