#include "ObstacleDetection.h"

// Number of iterations in calculating average distance
/* static */ const uint8_t ObstacleDetection::iterations = 5;

ObstacleDetection::ObstacleDetection(ObstacleSensor *frontSensorPtr, ObstacleSensor *leftSensorPtr, ObstacleSensor *rightSensorPtr, Navigator *navPtr) :
    frontSensorPtr_(frontSensorPtr),
    leftSensorPtr_(leftSensorPtr),
    rightSensorPtr_(rightSensorPtr),
    navPtr_(navPtr)
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
    
    // Update Sensor 1 data
    frontMeasured = frontSensorPtr_->activateSensor(iterations);
    //frontDistanceX = frontSensorPtr_->objXDist_;
    //frontDistanceY = frontSensorPtr_->objYDist_;
    //frontMeasuredDistance = frontSensorPtr_->distance_;

    // Output obstacle grid reference
    if (frontMeasured == 1)
    {
        frontSensorPtr_->printDistance("Front");        

        // Output obstacle grid location to navigator object (points to nav in botMain)
        navPtr_->addObstacle(frontSensorPtr_->gridX_, frontSensorPtr_->gridY_);
    }

    // Wait for sensor 1 to finish to begin sensor 2 measurements
    if (frontMeasured == 1 || frontMeasured == 0)
    {
        // Update Sensor 2 data
        leftMeasured = leftSensorPtr_->activateSensor(iterations);
        //leftDistanceX = leftSensorPtr_->objXDist_;
        //leftDistanceY = leftSensorPtr_->objYDist_;
        //leftMeasuredDistance = leftSensorPtr_->distance_;
        
        // Output obstacle grid reference
        if (leftMeasured == 1)
        {
            leftSensorPtr_->printDistance("Left");

            // Output obstacle grid location to navigator object (points to nav in botMain)
            navPtr_->addObstacle(leftSensorPtr_->gridX_, leftSensorPtr_->gridY_);
        }
        
        // Wait for sensor 2 to finish to begin sensor 3 measurements
        if (leftMeasured == 1 || leftMeasured == 0)
        {
            // Update Sensor 3 data
            rightMeasured = rightSensorPtr_->activateSensor(iterations);
            //rightDistanceX = rightSensorPtr_->objXDist_;
            //rightDistanceY = rightSensorPtr_->objYDist_;
            //rightMeasuredDistance = rightSensorPtr_->distance_;
            
            // Output obstacle grid reference
            if (rightMeasured == 1)
            {
                rightSensorPtr_->printDistance("Right");

                // Output obstacle grid location to navigator object (points to nav in botMain)
                navPtr_->addObstacle(rightSensorPtr_->gridX_, rightSensorPtr_->gridY_);
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

void ObstacleDetection::odsToNavTestObstacles()
{
    navPtr_->addObstacle(0,0);
    navPtr_->addObstacle(0,10);
    navPtr_->addObstacle(10,10);
    navPtr_->addObstacle(10,0);
    navPtr_->addObstacle(5,3);
    navPtr_->addObstacle(8,8);
    navPtr_->addObstacle(3,7);
    navPtr_->addObstacle(1,6);
    navPtr_->addObstacle(9,2);
}

