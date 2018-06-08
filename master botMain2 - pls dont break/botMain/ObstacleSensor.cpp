#include "ObstacleSensor.h"
#include "common.h"

#include "DHT.h"

/*static */ float ObstacleSensor::soundcm_ = 0.0343f;
/*static */ float ObstacleSensor::heading_;
/*static */ int ObstacleSensor::xPos_;
/*static */ int ObstacleSensor::yPos_;

// Max error + bias must be less than 35cm to remain in correct grid reference 
// since this is the size of the obstacle. Hence error of 16cm and projection
// bias of 17cm remains under this value.
/*static */ int ObstacleSensor::bias_ = 170;

ObstacleSensor::ObstacleSensor(int triggerPin, int echoPin, float offsetX, float offsetY, float sensorAngle) :
    triggerPin_(triggerPin),
    echoPin_(echoPin),
    offsetX_(offsetX),
    offsetY_(offsetY),
    sensorAngle_(sensorAngle),
    sonar_(triggerPin_, echoPin_, maxDistance_)
{
    
}

/* static */ void ObstacleSensor::calculateSoundCm(int lm35Pin)
{
    /*
    // Initialise DHT
    DHT dht(lm35Pin, DHT22);
    dht.begin();

    // Delay to allow DHT22 time to stabilise
    delay(1000);

    float hum = dht.readHumidity();      // Get Humidity value
    float temp = dht.readTemperature();  // Get Temperature value

    // Calculate the Speed of Sound in m/s
    float soundsp = 331.4f + (0.606f * temp) + (0.0124f * hum);

    // Convert to cm/ms from m/s
    ObstacleSensor::soundcm_ = soundsp / 10000.0f;
    */
    /*
    analogReference(INTERNAL);
    int raw = analogRead(lm35Pin);
    float temp = raw / 9.31;
    float hum = 0;

    // Calculate the Speed of Sound in m/s
    float soundsp = 331.4f + (0.606f * temp);
    ObstacleSensor::soundcm_ = soundsp / 10000.0f;

    // Print speed of sound details
    printSound(temp, hum);
    */
}

/* static */ void ObstacleSensor::printSound(float temp, float hum)
{
    if(test_mode == 1)
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
}

int ObstacleSensor::activateSensor(int iterations)
{
    // Measure duration    
    duration_ = sonar_.ping_median(iterations);

    // Calculate distance
    distance_ = 10*(duration_ / 2) * soundcm_;

    // Distance valid check
    if (distance_ >= 700 || distance_ <= 2) 
    {
        /*
        objXDist_ = 1023.0f;
        objYDist_ = 1023.0f;
        distance_ = 1023.0f;
        objX_ = 1023.0f;
        objY_ = 1023.0f;
        */
        
        // Currently heading defined as angle bot's front is facing, relative to y direction of grid
        // Calculate the x and y components of measured distance based on angle of sensor
        xDistComp_ = bias_*sin(sensorAngle_ + heading_);
        yDistComp_ = bias_*cos(sensorAngle_ + heading_);
        
        unitVect_[0] = xDistComp_/bias_;
        unitVect_[1] = yDistComp_/bias_;

        // Calculate offset vector from pozyx
        float offsetDist = sqrt(offsetX_*offsetX_ + offsetY_*offsetY_);
        float offsetAngle = atan2(offsetX_, offsetY_);

        // Calculate x and y offset components including heading changes
        float xOffsetComp = offsetDist*sin(offsetAngle + heading_);
        float yOffsetComp = offsetDist*cos(offsetAngle + heading_);

        // Calculate x and y components of distance from pozyx sensor
        objXDist_ = xDistComp_ + xOffsetComp;
        objYDist_ = yDistComp_ + yOffsetComp;

        // Calculates x and y coordinates by summing distance from pozyx with pozyx coordinates
        objX_ = objXDist_ + xPos_;
        objY_ = objYDist_ + yPos_;

        // Convert x and y obstacle coordinate to grid reference
        gridX_ = -1*((objX_ + bias_*unitVect_[0])/500 + 1);
        gridY_ = -1*((objY_ + bias_*unitVect_[1])/500 + 1);

        // If obstacle is located in current location project to outside it
        if(gridX_ == -1*(xPos_/500 + 1) && gridY_ == -1*(yPos_/500 + 1))
        {
            //Serial.println("gridX_: " + String(gridX_) + "\tgridY_: " + String(gridY_) + "\tunitVect_[0]: " + String(unitVect_[0]) + "\tunitVect_[1]: " + String(unitVect_[1]));
            gridX_ = gridX_ - unitVect_[0];
            gridY_ = gridY_ - unitVect_[1];
        }
        
        return 1;
    }
    else 
    {
        // Currently heading defined as angle bot's front is facing, relative to y direction of grid
        // Calculate the x and y components of measured distance based on angle of sensor
        xDistComp_ = distance_*sin(sensorAngle_ + heading_);
        yDistComp_ = distance_*cos(sensorAngle_ + heading_);

        unitVect_[0] = xDistComp_/distance_;
        unitVect_[1] = yDistComp_/distance_;

        // Calculate offset vector from pozyx
        float offsetDist = sqrt(offsetX_*offsetX_ + offsetY_*offsetY_);
        float offsetAngle = atan2(offsetX_, offsetY_);

        // Calculate x and y offset components including heading changes
        float xOffsetComp = offsetDist*sin(offsetAngle + heading_);
        float yOffsetComp = offsetDist*cos(offsetAngle + heading_);

        // Calculate x and y components of distance from pozyx sensor
        objXDist_ = xDistComp_ + xOffsetComp;
        objYDist_ = yDistComp_ + yOffsetComp;

        // Calculates x and y coordinates by summing distance from pozyx with pozyx coordinates
        objX_ = objXDist_ + xPos_;
        objY_ = objYDist_ + yPos_;

        // Convert x and y obstacle coordinate to grid reference
        gridX_ = (objX_ + bias_*unitVect_[0])/500 + 1;
        gridY_ = (objY_ + bias_*unitVect_[1])/500 + 1;

        // If obstacle is located in current location project to outside it
        if(gridX_ == (xPos_/500 + 1) && gridY_ == (yPos_/500 + 1))
        {
            //Serial.println("gridX_: " + String(gridX_) + "\tgridY_: " + String(gridY_) + "\tunitVect_[0]: " + String(unitVect_[0]) + "\tunitVect_[1]: " + String(unitVect_[1]));
            gridX_ = gridX_ + unitVect_[0];
            gridY_ = gridY_ + unitVect_[1];
        }
        return 1;
    }
}

void ObstacleSensor::printDistance(String sensorName)
{
    if(test_mode == 1)
    {
        Serial.print(sensorName);
        Serial.print(": ");
        
        if (this->distance_ == 1023.0f)
        {
            Serial.println("Distance out of range");
        }
        else 
        {
            Serial.print("xPos_: ");
            Serial.print(this->xPos_);
            Serial.print(" cm\t");
    
            Serial.print("yPos_: ");
            Serial.print(this->yPos_);
            Serial.print(" cm\t ");     
            
            Serial.print("Distance: ");
            Serial.print(this->distance_);
            Serial.print(" cm\t");
    
            Serial.print("xDistComp_: ");
            Serial.print(this->xDistComp_);
            Serial.print(" cm\t");
    
            Serial.print("yDistComp_: ");
            Serial.print(this->yDistComp_);
            Serial.print(" cm\t ");
            
            Serial.print("objXDist_: ");
            Serial.print(this->objXDist_);
            Serial.print(" cm\t");
    
            Serial.print("objYDist_: ");
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
}
