#ifndef NAVIGATOR_H
#define NAVIGATOR_H

//! \brief Navigator object controls all aspects of navigation command and 
//! decision making. 
//!
//! Navigator takes information regarding location, heading and obstacle
//! locations to populate a live map utilising a flood fill algorithm to 
//! prioritise movements. It then decides on a movement based on these 
//! priorities and commands the motor control sub-system to make the appropriate
//! movement. It also outputs the map via bluetooth to the GUI including
//! obstacles which are stored as a large number within this map.

#include <Arduino.h>
#include "QueueList.h"
#include "StackArray.h"
#include "common.h"

extern const int HEIGHT, WIDTH, DATA, ELEMENT_XPOS, ELEMENT_YPOS, ELEMENT_VALUE, OBSTACLE_COUNT;

class Navigator
{
    public:
        int  grid_[HEIGHT + 2][WIDTH + 2][DATA];
    
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
        /**
         * Default constructor
         * 
         * @param  xPosBegin   Cartesian x-coordinate of the start grid square
         * @param  yPosBegin   Cartesian y-coordinate of the start grid square
         * @param  xPosTarget  Cartesian x-coordinate of the target grid square
         * @param  yPosTarget  Cartesian y-coordinate of the target grid square
         */
        Navigator(int xPosBegin, int yPosBegin, int xPosEnd, int yPosEnd);
        
        /**
         * Prioritises each grid square using the flood-fill algorithm
         * 
         * @param  xPosTarget  Cartesian x-coordinate of the target grid square
         * @param  yPosTarget  Cartesian y-coordinate of the target grid square
         */
        void prioritiseMap(int xPos, int yPos); 

        /**
         * Clears current grid and reprioritises each grid square using the flood-fill algorithm
         * 
         * @param  xPosTarget  Cartesian x-coordinate of the target grid square
         * @param  yPosTarget  Cartesian y-coordinate of the target grid square
         */
        void reprioritiseMap(int xPos, int yPos); 
        
        /**
         * Adds an obstacle to the map 
         * 
         * @param  xPos  Cartesian x-coordinate of the obstacle grid square
         * @param  yPos  Cartesian y-coordinate of the obstacle grid square
         */
        void addObstacle(int xPos, int yPos);

        /**
         * Removes an obstacle from the map and gives it an element value of 0
         * 
         * @param  xPos  Cartesian x-coordinate of the obstacle grid square
         * @param  yPos  Cartesian y-coordinate of the obstacle grid square
         */
        void noObstacle(int xPos, int yPos);
                
        /**
         * Determines direction of movement with respect to the current grid square and 
         * neighbouring values
         * 
         * @param  xPos         Current horizontal position
         * @param  yPos         Current vertical position
         * @param  xPosTarget   Target horizontal position
         * @param  yPosTarget   Target vertical position
         * @param  heading      Current heading
         * 
         * @return  Integer uniquely mapped to cardinal direction required to advance to target location
         */
        int nextDirection(int xPos, int yPos, int xPosTarget, int yPosTarget, int heading);
        
        /**
        * Determines move required to move to a grid square determined by nextDirection()
        *
        * @param  xPos         Current horizontal position
        * @param  yPos         Current vertical position
        * @param  xPosTarget   Target horizontal position
        * @param  yPosTarget   Target vertical position
        * @param  heading      Current heading
        *
        * @return  Integer uniquely mapped to motor controller move function
        */
        int nextMove(int xPos, int yPos, int xPosTarget, int yPosTarget, int heading);

        /**
        * Determines if the grid squares of the position 
        *
        * @param  xPos         Current horizontal position
        * @param  yPos         Current vertical position
        * @param  heading      Current heading
        *
        * @return  integer uniquely mapped to a clear side
        */
        int sidesClear(int xPos, int yPos, int heading);

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

        /**
        * Tests and prints 
        * 
        * @param  i  Number of iterations to test the conversion of a millimetre location [0, 5000] to grid location [0, 10]
        */
        void testMilliToGrid(int i);
        
        /**
        * Tests moves functions
        */
        void testMove();

        /**
        * Prints movement related data for each move including its number, direction, resultant heading and command 
        * 
        * @param  id         move number
        * @param  direction  movement direction
        * @param  heading    resultant heading after movement
        * @param  command    movement command to be sent to motor controller 
        */
        void printMoveData(int id, int direction, int heading, int command);

        /**
        * Prints movement commands for a required return pathway
        */
        void printReturnMoveData();

                /**
         * Converts millimetre coordinate to grid coordinate
         * 
         * @param   coord   Millimetre coordinate  
         * 
         * @return  Grid coordinate
         */
        int milliToGrid(int milliCoord);
};

#endif
