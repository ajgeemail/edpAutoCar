#include "ObstacleSensor.h"

#include "DHT.h"

float ObstacleSensor::soundcm_ = 0.0343f;

ObstacleSensor::ObstacleSensor(uint8_t triggerPin, uint8_t echoPin, float offsetX, float offsetY, float directionAngle) :
    triggerPin_(triggerPin),
    echoPin_(echoPin),
    offsetX_(offsetX),
    offsetY_(offsetY),
    directionAngle_(directionAngle),
    sonar_(triggerPin_, echoPin_, maxDistance_)
{

}

/* static */ void ObstacleSensor::calculateSoundCm(uint8_t dhtPin)
{
    // Initialise DHT
    DHT dht(dhtPin, DHT22);
    dht.begin();

    // Delay to allow DHT22 time to stabilise
    delay(1000);

    float hum = dht.readHumidity();      // Get Humidity value
    float temp = dht.readTemperature();  // Get Temperature value

    // Calculate the Speed of Sound in m/s
    float soundsp = 331.4f + (0.606f * temp) + (0.0124f * hum);

    // Convert to cm/ms from m/s
    ObstacleSensor::soundcm_ = soundsp / 10000.0f;

    // Prints the speed of sound calculations
    Serial.println("***** SOUND CALCULATIONS *****");
    Serial.print("Temperature: ");
    Serial.print(temp);
    Serial.print(" Humidity: ");
    Serial.print(hum);
    Serial.print(" Speed of sound (cm/ms): ");
    Serial.println(soundcm_, 6);
}

uint8_t ObstacleSensor::detectObstacles(uint8_t iterations)
{
    // Measure duration    
    duration_ = sonar_.ping_median(iterations);

    // Calculate distance
    distance_ = (duration_ / 2) * soundcm_;

    // Distance valid check
    if (distance_ >= 400 || distance_ <= 2) 
    {
        objXDist_ = 1023.0f;
        objYDist_ = 1023.0f;
        distance_ = 1023.0f;
        return 0;
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
        return 1;
    }
}

void ObstacleSensor::printDistance(String sensorName)
{
    Serial.print(sensorName);
    
    if (this->distance_ == 1023.0f)
    {
        Serial.println("Distance out of range");
    }
    else 
    {
        Serial.print("Total Distance: ");
        Serial.print(this->distance_);
        Serial.print(" cm\t");
        
        Serial.print("X Distance: ");
        Serial.print(this->objXDist_);
        Serial.print(" cm\t");

        Serial.print("Y Distance: ");
        Serial.print(this->objYDist_);
        Serial.println(" cm ");
    }
}
