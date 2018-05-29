#include "Navigator.h"

// Class attributes
StackArray <int> moveHistory; 

// Default constructor
Navigator::Navigator()
{
    createMap();
}

int Navigator::convertToArray(int coordinate)
{
    coordinate = HEIGHT + 1 - coordinate;
    return coordinate;
}

int Navigator::milliToGrid(double milliCoord)
{
    int gridCoord = (int) milliCoord/500;
    return gridCoord;
}

// ===================================================== MAZING FUNCTIONS =====================================================

void Navigator::addObstacle(int xPos, int yPos)
{
    int yPosArr = convertToArray(yPos); 
    grid_[yPosArr][xPos][ELEMENT_VALUE] = 255;

    Serial.print("Obstacle added [x][y] = [");
    Serial.print(xPos);
    Serial.print("][");
    Serial.print(yPos);
    Serial.println("]");
}

void Navigator::createMap()
{
    // Additional two units to each dimensions is to create an obstacle boundary wall around the grid
    for(int i = 0; i < HEIGHT + 2; i++)
    {
        for(int j = 0; j < WIDTH + 2; j++)
        {   
            grid_[i][j][ELEMENT_XPOS]  = j;                        
            grid_[i][j][ELEMENT_YPOS]  = convertToArray(i);          // Converts array coord to cartesian y-coord
            grid_[i][j][ELEMENT_VALUE] = 0;                          // 0 is the value for unassigned value
        }
    }
    
    // Assigns wall elements with obstacle priority values
    for(int i = 0; i < WIDTH + 2; i++)
    {
        grid_[0][i][ELEMENT_VALUE]          = 255;                  // 255 is the value for an obstacle
        grid_[i][0][ELEMENT_VALUE]          = 255;
        grid_[HEIGHT + 1][i][ELEMENT_VALUE] = 255;
        grid_[i][WIDTH + 1][ELEMENT_VALUE]  = 255;
    }
}

void Navigator::prioritiseMap(int xPosTarget, int yPosTarget)
{
    // Queue of array coordinates
    // Coordinates will be enqueued as pairs of the x-coordinate and then the y-coordinate
    QueueList <int> gridQueue;
    
    int xPosCurrent;
    int yPosCurrentArr;
    int yPosTargetArr = convertToArray(yPosTarget);                 // Array coord to cartesian y-coord
    
    // Grid square position currently being prioritised
    grid_[yPosTargetArr][xPosTarget][ELEMENT_VALUE] = 1;
    gridQueue.push(xPosTarget);
    gridQueue.push(yPosTargetArr);
    
    while(!gridQueue.isEmpty())
    {
        xPosCurrent = gridQueue.pop();
        yPosCurrentArr = gridQueue.pop();
        
        // Grid squares with cardinal directions from the current grid square
        int xPosEast     = xPosCurrent + 1;
        int xPosWest     = xPosCurrent - 1;
        int yPosNorthArr = yPosCurrentArr - 1;
        int yPosSouthArr = yPosCurrentArr + 1;
        
        // Assigns a value to an unassigned north cell and adds it to the queue
        if(grid_[yPosNorthArr][xPosCurrent][ELEMENT_VALUE] == 0)
        {
            grid_[yPosNorthArr][xPosCurrent][ELEMENT_VALUE] = grid_[yPosCurrentArr][xPosCurrent][ELEMENT_VALUE] + 1;  
            gridQueue.push(xPosCurrent);
            gridQueue.push(yPosNorthArr);
        }
        // Assigns a value to an unassigned east cell and adds it to the queue
        if(grid_[yPosCurrentArr][xPosEast][ELEMENT_VALUE] == 0)
        {
            grid_[yPosCurrentArr][xPosEast][ELEMENT_VALUE] = grid_[yPosCurrentArr][xPosCurrent][ELEMENT_VALUE] + 1;  
            gridQueue.push(xPosEast);
            gridQueue.push(yPosCurrentArr);
        }
        // Assigns a value to an unassigned south cell and adds it to the queue
        if(grid_[yPosSouthArr][xPosCurrent][ELEMENT_VALUE] == 0)
        {
            grid_[yPosSouthArr][xPosCurrent][ELEMENT_VALUE] = grid_[yPosCurrentArr][xPosCurrent][ELEMENT_VALUE] + 1; 
            gridQueue.push(xPosCurrent);
            gridQueue.push(yPosSouthArr);
        }
        // Assigns a value to an unassigned west cell and adds it to the queue
        if(grid_[yPosCurrentArr][xPosWest][ELEMENT_VALUE] == 0)
        {
            grid_[yPosCurrentArr][xPosWest][ELEMENT_VALUE] = grid_[yPosCurrentArr][xPosCurrent][ELEMENT_VALUE] + 1; 
            gridQueue.push(xPosWest);
            gridQueue.push(yPosCurrentArr);
        }   
    }
} 

// =================================================== NAVIGATION FUNCTIONS ===================================================

int Navigator::nextDirection(int xPos, int yPos, int xPosTarget, int yPosTarget)
{
    xPos              = milliToGrid(xPos);
    yPos              = milliToGrid(yPos);
    int yPosArr       = convertToArray(yPos);
    int yPosTargetArr = convertToArray(yPosTarget);

    // Grid squares with cardinal directions from the current grid square            
    int eastValue  = grid_[yPosArr][xPos + 1][2];
    int westValue  = grid_[yPosArr][xPos - 1][2];
    int northValue = grid_[yPosArr - 1][xPos][2];
    int southValue = grid_[yPosArr + 1][xPos][2];
    
    if(yPosArr != yPosTargetArr || xPos != xPosTarget)
        if(northValue <= southValue && northValue <= eastValue && northValue <= westValue)         
            return 0;
        else if(southValue <= northValue && southValue <= eastValue && southValue <= westValue)
            return 2;
        else if(eastValue <= southValue && eastValue <= northValue && eastValue <= westValue)         
            return 3;
        else if(westValue <= southValue && westValue <= northValue && westValue <= eastValue)
            return 1;
    return -1;
}

int Navigator::nextMove(int xPos, int yPos, int xPosTarget, int yPosTarget, int heading)
{
    // Angle between direction of next movement and current heading
    int angleDirection = 90 * nextDirection(xPos, yPos, xPosTarget, yPosTarget);       
    int angle          = heading - angleDirection; 
    
    // Ensures that angle is [0, 359] degrees
    angle = (angle + 360) % 360;

    if (angle >= 315 || angle < 45)
    {
        moveHistory.push(1);
        return 1; // forward
    }
    else if (45 <= angle && angle < 135)
    {
        moveHistory.push(4);
        return 4; // right turn
    }
    else if (135 <= angle && angle < 225)
    {    
        moveHistory.push(3);
        return 3; // reverse
    }
    else if (225 <= angle && angle < 315)
    {    
        moveHistory.push(2);
        return 2; // left turn
    }
    return -1;
}

int Navigator::returnMove()
{
    int pathMove = moveHistory.pop();
    if (pathMove == 1)
        return 3; // reverse
    else if (pathMove == 2)
        return 5; // reverse right turn
    else if (pathMove == 3)
        return 1; // forward
    else if (pathMove == 4)
        return 6; // reverse left turn
}

// ================================================== TEST & PRINT FUNCTIONS ==================================================

void Navigator::testMap()
{
    using namespace std;
    testMilliToGrid(20);
    Serial.println("===================================\nTESTING - createMap():");
    createMap();
    printMap();
    Serial.println("\nTESTING - creategrid_:");
    testObstacleData();
    prioritiseMap(9, 9);
    printMap();
    //printSerialData();
}

void Navigator::printMap()
{
    Serial.println("X-COORDINATES:");
    for(int i = 0; i < HEIGHT + 2; i++)
    {
        for(int j = 0; j < WIDTH + 2; j++)
        {
            Serial.print(grid_[i][j][ELEMENT_XPOS]);
            Serial.print("\t");
        }
        Serial.println();
    }
    
    Serial.println("Y-COORDINATES:");
    for(int i = 0; i < HEIGHT + 2; i++)
    {
        for(int j = 0; j < WIDTH + 2; j++)
        {
            Serial.print(grid_[i][j][ELEMENT_YPOS]);
            Serial.print("\t");
        }
        Serial.println();
    }   
    
    Serial.println("VALUES:");
    for(int i = 0; i < HEIGHT + 2; i++)
    {
        for(int j = 0; j < WIDTH + 2; j++)
        {
            Serial.print(grid_[i][j][ELEMENT_VALUE]);
            Serial.print("\t");
        }
        Serial.println();
    }   
}

void Navigator::testObstacleData()
{
    addObstacle(3, 3);
    addObstacle(3, 4);
    addObstacle(8, 2);
    addObstacle(9, 2);
    addObstacle(10, 2);
    addObstacle(7, 4);
    addObstacle(6, 5);
    addObstacle(7, 5);
    addObstacle(8, 5);
    addObstacle(1, 7);
    addObstacle(2, 7);
    addObstacle(3, 7);
    addObstacle(4, 7);
    addObstacle(5, 7);
    addObstacle(6, 7);
    addObstacle(4, 10);
    addObstacle(5, 10);
    addObstacle(6, 10);
    addObstacle(7, 10);
}

void Navigator::testMilliToGrid(int i)
{
    int id = 0;
    
    Serial.println("===================================");
    Serial.println("TESTING - milliToGrid():");
    
    while(id < i)
    {
        double num = (double) random(0, 5000);
        Serial.print(num);
        Serial.print("\tmm\t--->\t");
        Serial.println(milliToGrid(num));
        id++;
    }
}

void Navigator::testMove()
{
    int xPosTarget = 9;
    int yPosTarget = 9;
    int id = 1; // is the move's number ID
    int command;
    int direction;
    
    // initial position (in millimetres) with heading
    int xPos    = 500;  // grid coord = 1
    int yPos    = 500;  // grid coord = 1
    int yPosArr = convertToArray(yPos);
    int heading = 0;

    Serial.println("===================================");  
    
    do
    {
        direction = nextDirection(xPos, yPos, xPosTarget, yPosTarget);
        command   = nextMove(xPos, yPos, xPosTarget, yPosTarget, heading);

        // updates position to simulate correct movement by one grid square
        if(direction == 0)
            yPos += 500;
        else if(direction == 3)
            xPos += 500;
        else if(direction == 1)
            xPos -= 500;
        else if(direction == 2)
            yPos -= 500;
        
        // updates heading to simulate correct turns  
        if(command == 2)
            heading += 90;
        else if(command == 3)
            heading += 180;
        else if(command == 4)
            heading += 270;  
        
        printMoveData(id, direction, heading, command);
        
        yPosArr = convertToArray(yPos);
        id++;
    }
    while(grid_[yPosArr][xPos][2] != 1);
    
    Serial.println("------------------- RETURNING -------------------");
    printReturnMoveData();
    Serial.println("===================================");  
}

void Navigator::printMoveData(int id, int direction, int heading, int command)
{
    heading %= 360;     // ensures that the heading = [0, 359] 
    
    if(id == 1)
    {
        Serial.println("TESTING - move():");
        Serial.println("NO. \tDIRECTION \tHEADING (deg) \tCOMMAND");
    }
    
    Serial.print(id);
    
    // prints direction
    if (direction == 0)
    {
        Serial.print("\tup\t\t");
        Serial.print(heading);
    }
    else if (direction == 1)
    {
        Serial.print("\tleft\t\t");
        Serial.print(heading);
    }
    else if (direction == 2)
    {
        Serial.print("\tdown\t\t");
        Serial.print(heading);
    }
    else if (direction == 3)
    {
        Serial.print("\tright\t\t");
        Serial.print(heading);
    }
    
    // prints movement    
    if (command == 1)
        Serial.print("\t\tforward");
    else if (command == 2)
        Serial.print("\t\tleft turn");
    else if (command == 3)
        Serial.print("\t\treverse");
    else if (command == 4)
        Serial.print("\t\tright turn");    
    
    Serial.println();
}

void Navigator::printReturnMoveData()
{
    int command;
    int id = 1;
    
    Serial.println("NO. \tCOMMAND");
    
    while (!moveHistory.isEmpty())
    {
        command = returnMove();
        Serial.print(id);
        if (command == 1)
           Serial.print("\tforward");
        else if (command == 3)
            Serial.print("\treverse");
        else if (command == 5)
            Serial.print("\treverse right turn");
        else if (command == 6)
            Serial.print("\treverse left turn");  
        
        Serial.println();
        id++;  
    }
}
