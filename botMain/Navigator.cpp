#include "Navigator.h"

// Default constructor
Navigator::Navigator()
{
    createMap();
}

void Navigator::createMap()
{
	  // the additional two units to each dimensions is to create an obstacle boundary 
	  // wall around the grid
  	for(int i = 0; i < HEIGHT + 2; i++)
  	{
    		for(int j = 0; j < WIDTH + 2; j++)
    		{   
      			grid_[i][j][ELEMENT_XPOS] = j;                     	 
      			grid_[i][j][ELEMENT_YPOS] = convertToArray(i);   	     // converts array coord to cartesian y-coord
      			grid_[i][j][ELEMENT_VALUE] = 0;                      // 0 is the value for unassigned value
    		}
  	}
  	
  	// assigns wall elements with obstacle priority values
  	for(int i = 0; i < WIDTH + 2; i++)
  	{
    		grid_[0][i][ELEMENT_VALUE] = 255;						  	      // 255 is the value for an obstacle
    		grid_[i][0][ELEMENT_VALUE] = 255;
    		grid_[HEIGHT + 1][i][ELEMENT_VALUE] = 255;
    		grid_[i][WIDTH + 1][ELEMENT_VALUE] = 255;
  	}
}

int Navigator::convertToArray(int coordinate)
{
  	coordinate = HEIGHT + 1 - coordinate;
  	return coordinate;
}

void Navigator::testMap()
{
  	using namespace std;
  	Serial.println("===================================\nTESTING - createMap():");
  	createMap();
  	printMap();
  	Serial.println("\nTESTING - creategrid_:");
  	//testObstacleData();
  	printMap();
  	Serial.println("===================================");
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
