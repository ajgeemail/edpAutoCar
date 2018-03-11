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
        // Calculate the x and y components of measured distance based on angle of sensor
        float xComp = distance_*sin(directionAngle_);
        float yComp = distance_*cos(directionAngle_);

        // Calculate x and y components of distance from pozyx sensor
        objXDist_ = xComp + offsetX_;
        objYDist_ = yComp + offsetY_;
    }
}
