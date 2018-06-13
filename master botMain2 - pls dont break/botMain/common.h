#ifndef COMMON_H
#define COMMON_H

//! \brief Contains variables common to multiple classes allowing easy sharing of this
//! information rather than passing the variables or doubling up on them.

static const int HEIGHT  = 10;
static const int WIDTH   = 10; 
static const int DATA    = 4;
const int ELEMENT_XPOS   = 0;                        // Grid square's cartesian x-coord
const int ELEMENT_YPOS   = 1;                        // Grid square's cartesian y-coord
const int ELEMENT_VALUE  = 2;                        // Grid square's weighting value
const int OBSTACLE_COUNT = 3;                        // Grid square's weighting value

// ****************** MODES *****************************************************************
//! test_mode changes the type of test output provided to user. USB serial monitor, BT
//! serial monitor and operational mode (i.e. no test data and 10 sec delay enforced after
//! initial setup complete.
const int test_mode = 2;    // Serial and BT prints:  1 = Test prints through serial
                            //                        2 = Test prints through BT
                            //                        0 = Operational mode     

#endif
