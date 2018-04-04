#ifndef NAVIGATOR_H
#define NAVIGATOR_H

#include <stdint.h>

class Navigator
{
    private:
        int8_t map[][][] ;
        const uint8_t ELEMENT_XPOS = 0;                         // grid square's cartesian x-coord
        const uint8_t ELEMENT_YPOS = 1;                         // grid square's cartesian y-coord
        const uint8_t ELEMENT_VALUE = 2;                        // grid square's weighting value
        const uint8_t HEIGHT = 10;                              // Number of grid references high
        const uint8_t LENGTH = 10;                              // Number of grid references long

    private: 
        /**
         * Creates a map that with a given length, height and a boundary wall of obstacles 
         * assigning an initial weighting of -1
         * 
         * @param  height  Height of the grid to be traversed
         * @param  length  Length of the grid to be traversed
         */
        void createMap(uint8_t height, uint8_t length);
    
        /**
         * Converts y grid coordinate to y array coordinate
         * 
         * @param  coord   Grid coordinate  
         * 
         * @return  Array coordinate
         */
        uint8_t convertToArray(uint8_t coord);
    
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

    public:
        // Default constructor
        Navigator();
    
        /**
         * Adds an obstacle to the map 
         * 
         * @param  xPos  Cartesian x-coordinate of the obstacle grid square
         * @param  yPos  Cartesian y-coordinate of the obstacle grid square
         */
        void addObstacle(uint8_t xPos, uint8_t yPos);
    
};

#endif
