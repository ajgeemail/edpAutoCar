#ifndef NAVIGATOR_H
#define NAVIGATOR_H

#include <Arduino.h>
#include "common.h"

extern const int HEIGHT, WIDTH, DATA, ELEMENT_XPOS, ELEMENT_YPOS, ELEMENT_VALUE;

class Navigator
{
    public:
        int grid_[HEIGHT][WIDTH][DATA];

    private: 
        /**
         * Creates a map that with a given length, height and a boundary wall of obstacles 
         * assigning an initial weighting of -1
         * 
         * @param  height  Height of the grid to be traversed
         * @param  length  Length of the grid to be traversed
         */
        void createMap();
        
        /**
         * Converts y grid coordinate to y array coordinate
         * 
         * @param  coord   Grid coordinate  
         * 
         * @return  Array coordinate
         */
        int convertToArray(int coord);
    
        

    public:
        // Default constructor
        Navigator();
    
        /**
         * Adds an obstacle to the map 
         * 
         * @param  xPos  Cartesian x-coordinate of the obstacle grid square
         * @param  yPos  Cartesian y-coordinate of the obstacle grid square
         */
        void addObstacle(int xPos, int yPos);

        /**
         * Tests the createMap() function with a length of 10 and height of 10.
         * Tests the prioritiseMap() function with a target location at 9, 9
         */
        void testMap();
    
        /**
         * Prints the grid with the associated priority value for each grid square
         */
        void printMap();
    
        /**
        * Adds obstacles from hardcoded obstacle position data
        */
        void testObstacleData();
    
};

#endif
