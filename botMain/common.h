#ifndef COMMON_H
#define COMMON_H

//! \brief Contains variables common to multiple classes allowing easy sharing of this
//! information rather than passing the variables or doubling up on them.

static const int HEIGHT = 10;
static const int WIDTH = 10; 
static const int DATA = 3;
const int ELEMENT_XPOS = 0;                        // Grid square's cartesian x-coord
const int ELEMENT_YPOS = 1;                        // Grid square's cartesian y-coord
const int ELEMENT_VALUE = 2;                       // Grid square's weighting value

#endif
