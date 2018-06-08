#include "ObstacleDetection.h"

// Number of iterations in calculating average distance
/* static */ const int ObstacleDetection::iterations = 5;

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
    int frontMeasured = 0;
    int leftMeasured = 0;
    int rightMeasured = 0;
    
    // Update Sensor 1 data
    frontMeasured = frontSensorPtr_->activateSensor(iterations);

    // Output obstacle grid reference
    if (frontMeasured == 1)
    {    
        //frontSensorPtr_->printDistance("Front");
        
        // Output obstacle grid location to navigator object (points to nav in botMain)
        if(frontSensorPtr_->gridX_ > 0 && frontSensorPtr_->gridX_ < WIDTH + 1 && frontSensorPtr_->gridY_ > 0 && frontSensorPtr_->gridY_ < HEIGHT + 1)
        {
            navPtr_->addObstacle(frontSensorPtr_->gridX_, frontSensorPtr_->gridY_);
        }
        else if(frontSensorPtr_->gridX_ < 0 && frontSensorPtr_->gridX_ > -1*(WIDTH + 1) && frontSensorPtr_->gridY_ < 0 && frontSensorPtr_->gridY_ > -1*(HEIGHT + 1))
        {
            navPtr_->noObstacle(-1*(frontSensorPtr_->gridX_), -1*(frontSensorPtr_->gridY_));
        }
        else
        {
        }
    }
    else
    {
    }

    // Wait for sensor 1 to finish to begin sensor 2 measurements
    if (frontMeasured == 1 || frontMeasured == 0)
    {
        //frontSensorPtr_->printDistance("Front"); 
        
        // Update Sensor 2 data
        leftMeasured = leftSensorPtr_->activateSensor(iterations);
        
        // Output obstacle grid reference
        if (leftMeasured == 1)
        {
            //leftSensorPtr_->printDistance("Left");

            // Output obstacle grid location to navigator object (points to nav in botMain)
            if(leftSensorPtr_->gridX_ > 0 && leftSensorPtr_->gridX_ < WIDTH + 1 && leftSensorPtr_->gridY_ > 0 && leftSensorPtr_->gridY_ < HEIGHT + 1)
            {
                navPtr_->addObstacle(leftSensorPtr_->gridX_, leftSensorPtr_->gridY_);
            }
            else if(leftSensorPtr_->gridX_ < 0 && leftSensorPtr_->gridX_ > -1*(WIDTH + 1) && leftSensorPtr_->gridY_ < 0 && leftSensorPtr_->gridY_ > -1*(HEIGHT + 1))
            {
                navPtr_->noObstacle(-1*(leftSensorPtr_->gridX_), -1*(leftSensorPtr_->gridY_));
            }
            else
            {
            }
        }
        else
        {
        }
        
        // Wait for sensor 2 to finish to begin sensor 3 measurements
        if (leftMeasured == 1 || leftMeasured == 0)
        {
            //leftSensorPtr_->printDistance("Left");
            
            // Update Sensor 3 data
            rightMeasured = rightSensorPtr_->activateSensor(iterations);
            
            // Output obstacle grid reference
            if (rightMeasured == 1)
            {
                //rightSensorPtr_->printDistance("Right");

                // Output obstacle grid location to navigator object (points to nav in botMain)
                if(rightSensorPtr_->gridX_ > 0 && rightSensorPtr_->gridX_ < WIDTH + 1 && rightSensorPtr_->gridY_ > 0 && rightSensorPtr_->gridY_ < HEIGHT + 1)
                {
                    navPtr_->addObstacle(rightSensorPtr_->gridX_, rightSensorPtr_->gridY_);
                }
                else if(rightSensorPtr_->gridX_ < 0 && rightSensorPtr_->gridX_ > -1*(WIDTH + 1) && rightSensorPtr_->gridY_ < 0 && rightSensorPtr_->gridY_ > -1*(HEIGHT + 1))
                {
                    navPtr_->noObstacle(-1*(rightSensorPtr_->gridX_), -1*(rightSensorPtr_->gridY_));
                }
                else
                {
                }
            }
            else
            {
            }
        }
        else
        {
        }
    }
    else
    {
    }
}

int ObstacleDetection::detectLeftSensor()
{
    auto measured = leftSensorPtr_->activateSensor(iterations);
    leftSensorPtr_->printDistance("Left:  ");
    return measured;
}

int ObstacleDetection::detectRightSensor()
{
    auto measured = rightSensorPtr_->activateSensor(iterations);
    rightSensorPtr_->printDistance("Right: ");
    return measured;
}

int ObstacleDetection::detectFrontSensor()
{
    auto measured = frontSensorPtr_->activateSensor(iterations);
    frontSensorPtr_->printDistance("Front: ");
    return measured;
}

void ObstacleDetection::odsToNavTestObstacles()
{
    for(int i = 0; i < HEIGHT + 2; i++)
    {
        navPtr_->addObstacle(i,i);
        if(test_mode == 1)
        {
            Serial.println("Test uploaded");
        }
    }

    navPtr_->addObstacle(4,6);
    if(test_mode == 1)
    {
        Serial.println("Test uploaded");
    }
}

