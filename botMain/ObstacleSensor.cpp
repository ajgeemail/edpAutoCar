#include "ObstacleSensor.h"

#include "DHT.h"

/*static */ float ObstacleSensor::soundcm_ = 0.0343f;
/*static */ float ObstacleSensor::yaw_;
/*static */ float ObstacleSensor::xPos_;
/*static */ float ObstacleSensor::yPos_;

// Max error + bias must be less than 35cm to remain in correct grid reference 
// since this is the size of the obstacle. Hence error of 16cm and projection
// bias of 17cm remains under this value.
/*static */ int8_t ObstacleSensor::bias_ = 17;

ObstacleSensor::ObstacleSensor(uint8_t triggerPin, uint8_t echoPin, float offsetX, float offsetY, float sensorAngle) :
    triggerPin_(triggerPin),
    echoPin_(echoPin),
    offsetX_(offsetX),
    offsetY_(offsetY),
    sensorAngle_(sensorAngle),
    sonar_(triggerPin_, echoPin_, maxDistance_)
{
    Serial.println("Obstacle Sensor Constructor");
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

    // Print speed of sound details
    printSound(temp, hum);
}

/* static */ void ObstacleSensor::printSound(float temp, float hum)
{
    // Prints the speed of sound calculations
    Serial.println("***** SOUND CALCULATIONS *****");
    Serial.print("Temperature: ");
    Serial.print(temp);
    Serial.print(" Humidity: ");
    Serial.print(hum);
    Serial.print(" Speed of sound (cm/ms): ");
    Serial.println(ObstacleSensor::soundcm_, 6);
}

uint8_t ObstacleSensor::activateSensor(uint8_t iterations)
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
        objX_ = 1023.0f;
        objY_ = 1023.0f;
        return 0;
    }
    else 
    {
        // Currently yaw defined as angle bot's front is facing, relative to y direction of grid
        // Calculate the x and y components of measured distance based on angle of sensor
        float xDistComp = distance_*sin(sensorAngle_ + yaw_);
        float yDistComp = distance_*cos(sensorAngle_ + yaw_);

        unitVect_[0] = xDistComp/distance_;
        unitVect_[1] = yDistComp/distance_;

        // Calculate offset vector from pozyx
        float offsetDist = sqrt(offsetX_*offsetX_ + offsetY_*offsetY_);
        float offsetAngle = atan2(offsetX_, offsetY_);

        // Calculate x and y offset components including yaw changes
        float xOffsetComp = offsetDist*sin(offsetAngle + yaw_);
        float yOffsetComp = offsetDist*cos(offsetAngle + yaw_);

        // Calculate x and y components of distance from pozyx sensor
        objXDist_ = xDistComp + xOffsetComp;
        objYDist_ = yDistComp + yOffsetComp;

        // Calculates x and y coordinates by summing distance from pozyx with pozyx coordinates
        objX_ = objXDist_ + xPos_;
        objY_ = objYDist_ + yPos_;

        // Convert x and y obstacle coordinate to grid reference
        gridX_ = (objX_ + bias_*unitVect_[0])/50 + 1;
        gridY_ = (objY_ + bias_*unitVect_[1])/50 + 1;
        
        return 1;
    }
}

void ObstacleSensor::printDistance(String sensorName)
{
    Serial.print(sensorName);
    Serial.print(": ");
    
    if (this->distance_ == 1023.0f)
    {
        Serial.println("Distance out of range");
    }
    else 
    {
        Serial.print("Distance: ");
        Serial.print(this->distance_);
        Serial.print(" cm\t");
        
        Serial.print("X Dist: ");
        Serial.print(this->objXDist_);
        Serial.print(" cm\t");

        Serial.print("Y Dist: ");
        Serial.print(this->objYDist_);
        Serial.print(" cm\t ");

        Serial.print("Coord [x, y] = [");
        Serial.print(this->objX_);
        Serial.print(", ");
        Serial.print(this->objY_);
        Serial.print("]\t");
               
        Serial.print("Grid: [x, y] = [");
        Serial.print(this->gridX_);
        Serial.print(", ");
        Serial.print(this->gridY_);
        Serial.println("]");
    }
}
