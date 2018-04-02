#include "ObstacleDetection.h"

// Number of iterations in calculating average distance
uint8_t iterations = 5;

ObstacleDetection::ObstacleDetection(ObstacleSensor *frontSensorPtr, ObstacleSensor *leftSensorPtr, ObstacleSensor *rightSensorPtr) :
    frontSensorPtr_(frontSensorPtr),
    leftSensorPtr_(leftSensorPtr),
    rightSensorPtr_(rightSensorPtr)
{
    
}

// Completes measurements from all sensors *** IN THE FUTURE MIGHT WRITE FUNCTIONS TO USE DIFFERENT COMBINATIONS OF THE SENSORS AS NEEDED ***
void ObstacleDetection::detectAllSensors()
{
    uint8_t frontMeasured = 0;
    uint8_t leftMeasured = 0;
    uint8_t rightMeasured = 0;

    // Currently front, left and right DistanceX, Y and MeasuredDistance unused but they
    // are filled with the relative distance to obstacle from pozyx. Could be used later
    /*
    float frontDistanceX;
    float frontDistanceY;
    float frontMeasuredDistance;

    float leftDistanceX;
    float leftDistanceY;
    float leftMeasuredDistance;

    float rightDistanceX;
    float rightDistanceY;
    float rightMeasuredDistance;
    */
    
    // Sensor 1 data
    frontMeasured = frontSensorPtr_->activateSensor(iterations);
    //frontDistanceX = frontSensorPtr_->objXDist_;
    //frontDistanceY = frontSensorPtr_->objYDist_;
    //frontMeasuredDistance = frontSensorPtr_->distance_;

    // Allocate obstacle location
    if (frontMeasured == 1)
    {
        Serial.print("Front: [x, y] = [");
        Serial.print(frontSensorPtr_->objX_);
        Serial.print(", ");
        Serial.print(frontSensorPtr_->objY_);
        Serial.println("]");

        int8_t frontObstacleX = (frontSensorPtr_->objX_ + 17*frontSensorPtr_->unitVect_[0])/50;
        int8_t frontObstacleY = (frontSensorPtr_->objY_ + 17*frontSensorPtr_->unitVect_[1])/50;

        Serial.print("FrontGrid: [x, y] = [");
        Serial.print(frontObstacleX);
        Serial.print(", ");
        Serial.print(frontObstacleY);
        Serial.println("]");

        //Brady.addObstacle(frontObstacleX, frontObstacleY);
    }

    // Wait for sensor 1 to finish to begin sensor 2 measurements
    if (frontMeasured == 1 || frontMeasured == 0)
    {
        // Sensor 2 data
        leftMeasured = leftSensorPtr_->activateSensor(iterations);
        //leftDistanceX = leftSensorPtr_->objXDist_;
        //leftDistanceY = leftSensorPtr_->objYDist_;
        //leftMeasuredDistance = leftSensorPtr_->distance_;

        // Allocate obstacle location
        if (leftMeasured == 1)
        {
            
            Serial.print("Left: [x, y] = [");
            Serial.print(leftSensorPtr_->objX_);
            Serial.print(", ");
            Serial.print(leftSensorPtr_->objY_);
            Serial.println("]");

            int8_t leftObstacleX = (leftSensorPtr_->objX_ + 17*leftSensorPtr_->unitVect_[0])/50;
            int8_t leftObstacleY = (leftSensorPtr_->objY_ + 17*leftSensorPtr_->unitVect_[1])/50;

            Serial.print("LeftGrid: [x, y] = [");
            Serial.print(leftObstacleX);
            Serial.print(", ");
            Serial.print(leftObstacleY);
            Serial.println("]");

            //Brady.addObstacle(leftObstacleX, leftObstacleY);
        }

        // Wait for sensor 2 to finish to begin sensor 3 measurements
        if (leftMeasured == 1 || leftMeasured == 0)
        {
            // Sensor 3 data
            rightMeasured = rightSensorPtr_->activateSensor(iterations);
            //rightDistanceX = rightSensorPtr_->objXDist_;
            //rightDistanceY = rightSensorPtr_->objYDist_;
            //rightMeasuredDistance = rightSensorPtr_->distance_;

            // Allocate obstacle location
            if (rightMeasured == 1)
            {
                
                Serial.print("Right: [x, y] = [");
                Serial.print(rightSensorPtr_->objX_);
                Serial.print(", ");
                Serial.print(rightSensorPtr_->objY_);
                Serial.println("]");

                int8_t rightObstacleX = (rightSensorPtr_->objX_ + 17*rightSensorPtr_->unitVect_[0])/50;
                int8_t rightObstacleY = (rightSensorPtr_->objY_ + 17*rightSensorPtr_->unitVect_[1])/50;

                Serial.print("RightGrid: [x, y] = [");
                Serial.print(rightObstacleX);
                Serial.print(", ");
                Serial.print(rightObstacleY);
                Serial.println("]");

                //Brady.addObstacle(rightObstacleX, rightObstacleY);
            }
        }
    }

    //*frontSensorPtr_->printDistance("Front: ");
    //*leftSensorPtr_->printDistance("Left:  ");
    //*rightSensorPtr_->printDistance("Right: ");
}

// Detects with only left sensor
uint8_t ObstacleDetection::detectLeftSensor()
{
    uint8_t measured = leftSensorPtr_->activateSensor(iterations);
    leftSensorPtr_->printDistance("Left:  ");
    return measured;
}

// Detects with only right sensor
uint8_t ObstacleDetection::detectRightSensor()
{
    uint8_t measured = rightSensorPtr_->activateSensor(iterations);
    rightSensorPtr_->printDistance("Right: ");
    return measured;
}

// Detects with only front sensor
uint8_t ObstacleDetection::detectFrontSensor()
{
    uint8_t measured = frontSensorPtr_->activateSensor(iterations);
    frontSensorPtr_->printDistance("Front: ");
    return measured;
}


