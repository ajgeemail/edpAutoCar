#include "ObstacleSensor.h"

//#include "DHT.h"

ObstacleSensor::ObstacleSensor(uint8_t triggerPin, uint8_t echoPin, float offsetX, float offsetY, float directionAngle) :
    triggerPin_(triggerPin),
    echoPin_(echoPin),
    offsetX_(offsetX),
    offsetY_(offsetY),
    directionAngle_(directionAngle),
    sonar_(triggerPin_, echoPin_, maxDistance_)
{

}

/* static */ void ObstacleSensor::calculateSoundCm()
{
    /*
    // Initialise DHT
    DHT dht(dhtPin, DHT22);
    dht.begin();

    // Delay to allow DHT22 time to stabilise
    delay(1000);

    hum_ = dht.readHumidity();  // Get Humidity value
    temp_ = dht.readTemperature();  // Get Temperature value

    // Calculate the Speed of Sound in m/s
    soundsp_ = 331.4 + (0.606 * temp_) + (0.0124 * hum_);

    // Convert to cm/ms from m/s
    soundcm_ = soundsp_ / 10000;
    */
}

void ObstacleSensor::detectObstacles(uint8_t iterations)
{
    // Measure duration    
    duration_ = sonar_.ping_median(iterations);

    // Calculate distance
    distance_ = (duration_ / 2) * soundcm_;

    // Distance valid check
    if (distance_ >= 400 || distance_ <= 2) 
    {
        return 1023.0f;
    }
    else 
    {
        // Currently yaw defined as angle bot's front is facing, relative to y direction of grid
        float yaw = 0*(PI/180);    // ***** Change to yaw = getYaw() from pozyx *****
        // Calculate the x and y components of measured distance based on angle of sensor
        float xDistComp = distance_*sin(directionAngle_ + yaw);
        float yDistComp = distance_*cos(directionAngle_ + yaw);

        // Calculate offset vector from pozyx
        float offsetDist = sqrt(offsetX_*offsetX_ + offsetY_*offsetY_);
        float offsetAngle = atan2(offsetX_, offsetY_);

        // Calculate x and y offset components including yaw changes
        float xOffsetComp = offsetDist*sin(offsetAngle + yaw);
        float yOffsetComp = offsetDist*cos(offsetAngle + yaw);

        // Calculate x and y components of distance from pozyx sensor
        objXDist_ = xDistComp + xOffsetComp;
        objYDist_ = yDistComp + yOffsetComp;
    }
}
