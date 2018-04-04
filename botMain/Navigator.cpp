#include "navigator.h"

void Navigator::createMap(uint8_t height, uint8_t length)
{
	// the additional two units to each dimensions is to create an obstacle boundary 
	// wall around the grid
	map = new int[height + 2][length + 2][3];
	
	for(int i = 0; i < length + 2; i++)
	{
		for(int j = 0; j < height + 2; j++)
		{
			map[i][j][ELEMENT_XPOS] = j;                     	 
			map[i][j][ELEMENT_YPOS] = height - i + 1;   	     // converts array coord to cartesian y-coord
			map[i][j][ELEMENT_VALUE] = -1;                    	 // -1 is the value for unassigned value
		}
	}
	
	// assigns wall elements with obstacle priority values
	for(int i = 0; i < length + 2; i++)
	{
		map[0][i][ELEMENT_VALUE] = 127;						  	// 127 is the value for an obstacle
		map[i][0][ELEMENT_VALUE] = 127;
		map[height + 1][i][ELEMENT_VALUE] = 127;
		map[i][length + 1][ELEMENT_VALUE] = 127;
	}
	
	delete [] map;
}

void Navigator::addObstacle(uint8_t xPos, uint8_t yPos)
{
	uint8_t yPosArr = convertToArray(yPos); 
	map[yPosArr][xPos][ELEMENT_VALUE] = 127;
}

uint8_t Navigator::convertToArray(uint8_t coord)
{
	coord = height + 1 - coord;
	return coord;
}

void Navigator::testMap()
{
	using namespace std;
	cout << "===================================\nTESTING - createMap:" << endl;
	createMap(10, 10);
	printMap();
	cout << "\nTESTING - createMap:" << endl;
	testObstacleData();
	printMap();
	cout << "===================================" << endl;
}

void Navigator::printMap()
{
	cout << "X-COORDINATES:" << endl;
	for(int i = 0; i < HEIGHT; i++)
	{
		for(int j = 0; j < LENGTH; j++)
		{
			cout << map[i][j][ELEMENT_XPOS] << "\t";
		}
		cout << endl;
	}
	
	cout << "Y-COORDINATES:" << endl;
	for(int i = 0; i < HEIGHT; i++)
	{
		for(int j = 0; j < LENGTH; j++)
		{
			cout << map[i][j][ELEMENT_YPOS] << "\t";
		}
		cout << endl;
	}   
	
	cout << "VALUES:" << endl;
	for(int i = 0; i < HEIGHT; i++)
	{
		for(int j = 0; j < LENGTH; j++)
		{
			cout << map[i][j][ELEMENT_VALUE] << "\t";
		}
		cout << endl;
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
