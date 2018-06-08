//! \file botMain.ino
//! \brief Top level file for the AVS which integrates all sub-systems on the arduino. 
//!
//! The botMain file will contain the initial setup and loop functions along with all sub-system
//! object initialisations including parameter setting. Ideally all of the sub-systems shall
//! implement separate classes to improve modularity and readability of this top file. Requires
//! specific libraries be downloaded using the Include Library function explained as per INSTRUCTIONS
//! below.

// ****************************************************************************************************************** //
// ***** INSTRUCTIONS:                                                                                          ***** //
// ***** 1. Ensure all libraries are installed by navigating Sketch > Include Library > Manage Libraries        ***** //
// *****    and searching for the following libraries:  (i)     DHT                                             ***** //
// *****                                                (ii)    Adafuit Unified Sensor                          ***** //
// *****                                                (iii)   Adafruit AM2315                                 ***** //
// *****                                                (iv)    Pozyx                                           ***** //
// *****                                                (iv)    adafrui motor shield v2                         ***** //
// ***** 2. 
// ****************************************************************************************************************** //

#include "ObstacleSensor.h"
#include "ObstacleDetection.h"
#include "common.h"
#include <Pozyx.h>
#include <Pozyx_definitions.h>
#include <Servo.h> 
#include <Adafruit_MotorShield.h>
#include <SoftwareSerial.h>
SoftwareSerial BT(19, 18); // Creates virtual serial on pins on (RX) | (TX) --> bluetooth pins

// **************** LOC SPECIFIC VARIABLES *******************************************************
const int basicAvgAmt = 50; //varying this changes the number of pozyx readings averaged

const int   num_to_avg             = 5;                                         // Number of position readings to average out for the output of position data
const int   num_anchors            = 4;                                         // Number of anchors
uint16_t    anchors[num_anchors] = {0x687c, 0x6821, 0x6827, 0x6851};            // the network id of the anchors
int32_t     anchors_x[num_anchors] = {0, 0, 4998, 4985};                        // anchor x-coorindates in mm
int32_t     anchors_y[num_anchors] = {0, 5005, 4985, 0};                        // anchor y-coordinates in mm
int32_t     heights[num_anchors] = {810, 1720, 765, 1715};                      // anchor z-coordinates in mm


int algorithm = POZYX_POS_ALG_UWB_ONLY;                                       // Positioning algorithm to use. try POZYX_POS_ALG_TRACKING for fast moving objects.
int dimension = POZYX_2D;                                                     // Positioning dimension ie 2D, 2_5D and 3D
int32_t height    = 0;                                                            // z position of pozyx in mm

int x_loc = 1750;                                                 //The measured x location in mm from coordinate (0,0)
int y_loc = 3250;                                                 //The measured y location in mm from coordinate (0,0)
int heading = 90;                                               //The magnetic heading in degrees
int headingOffset = 0;                                            // heading offset to facilitate grid north NEW31/05

int x_loc_prev = 0;                                            //Previous x loc for Goughy
int y_loc_prev = 0;                                            //Previous y loc for Goughy

String result = ";911;";                                              //TX bluetooth string, starts with semicolon
// *********************** NM SPECIFIC VARIABLES *******************************************************

int  xPosStart     = 2;                                                           // Starting cartesian x-coordinate in grid units
int  yPosStart     = 9;                                                           // Starting cartesian y-coordinate in grid units
int  xPosTarget    = 7;                                                           // Target cartesian x-coordinate in grid units
int  yPosTarget    = 2;                                                           // Target cartesian y-coordinate in grid units
bool returnToStart = false;                                                       // Differentiates return trip from target location

Navigator nav(xPosStart, yPosStart, xPosTarget, yPosTarget);

// **************** ODS SPECIFIC VARIABLES *******************************************************
// Direction variables in radians (relative to AVS heading)
//! Left direction relative to front of AVS in radians
#define LEFT_DIR 270*(PI/180);
//! Forward direction relative to front of AVS in radians
#define FORWARD_DIR 0*(PI/180);
//! Right direction relative to front of AVS in radians
#define RIGHT_DIR 90*(PI/180);
//! Backward direction relative to front of AVS in radians
#define BACKWARD_DIR 180*(PI/180);
//! Forward right diagonal direction relative to front of AVS in radians
#define DIAG_FOR_RIGHT 45*(PI/180);
//! Forward diagonal left direction relative to front of AVS in radians
#define DIAG_FOR_LEFT 315*(PI/180);
//! Backward diagonal left direction relative to front of AVS in radians
#define DIAG_BACK_LEFT 225*(PI/180);
//! Backward diagonal right direction relative to front of AVS in radians
#define DIAG_BACK_RIGHT 135*(PI/180);

// ***** Setup Sensor 1 variables *****
//! Front sensor arduino trigger pin number
const int frontTriggerPin = 35; // red
//! Front sensor arduino echo pin number
const int frontEchoPin = 37; // black
//! Front sensor x offset on chassis from pozyx location
const float frontXOffset = 0; // mm
//! Front sensor y offset on chassis from pozyx location
const float frontYOffset = 130; // mm
//! Front sensor angle relative to forward of chassis
const float frontsensorAngle = FORWARD_DIR;

// ***** Setup Sensor 2 variables *****
//! Left sensor arduino trigger pin number
const int leftTriggerPin = 31; // blue
//! Left sensor arduino echo pin number
const int leftEchoPin = 33; 
//! Left sensor x offset on chassis from pozyx location
const float leftXOffset = 80; // mm
//! Left sensor y offset on chassis from pozyx location
const float leftYOffset = 130; // mm
//! Left sensor angle relative to forward of chassis
const float leftsensorAngle = LEFT_DIR;

// ***** Setup Sensor 3 variables *****
//! Right sensor arduino trigger pin number
const int rightTriggerPin = 39; // yellow
//! Right sensor arduino echo pin number
const int rightEchoPin = 41; // brown -> orange
//! Right sensor x offset on chassis from pozyx location
const float rightXOffset = 80; // mm
//! Right sensor y offset on chassis from pozyx location
const float rightYOffset = 130; // mm
//! Right sensor angle relative to forward of chassis
const float rightsensorAngle = RIGHT_DIR;

//! Temperature Sensor arduino pin number
const int tempPin = A0;

// ***** Create sensor objects with relevant info *****
//! Front sensor ObstacleSensor object initialisation - used to detect obstacles at front of AVS
ObstacleSensor frontSensor(frontTriggerPin, frontEchoPin, frontXOffset, frontYOffset, frontsensorAngle);
//! Left sensor ObstacleSensor object initialisation - used to detect obstacles at left of AVS
ObstacleSensor leftSensor(leftTriggerPin, leftEchoPin, leftXOffset, leftYOffset, leftsensorAngle);
//! Right sensor ObstacleSensor object initialisation - used to detect obstacles at right of AVS
ObstacleSensor rightSensor(rightTriggerPin, rightEchoPin, rightXOffset, rightYOffset, rightsensorAngle);
//ObstacleSensor rightSensor1(rightTriggerPin, rightEchoPin, rightXOffset, rightYOffset, rightsensorAngle);
//ObstacleSensor rightSensor2(rightTriggerPin, rightEchoPin, rightXOffset, rightYOffset, rightsensorAngle);


//! Obstacle detection system object initialisation - used to activate ObstacleSensor objects
//! as required and pass on information to the navigation system
ObstacleDetection ods(&frontSensor, &leftSensor, &rightSensor, &nav);

// **************** MC SPECIFIC VARIABLES ********************************************************
//--------------------------------CGK------------------------------------------------------------
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *myMotor = AFMS.getMotor(1);
Servo servo1;                     //cgk name the first servo motor                        servo1
int cgk_straight = 122;           //cgk straight for the servo 0-255                      127
int cgk_back_little = 370;        //cgk used for the tight turning distance               370
int cgk_fwd_little = 330;         //cgk used for the tight turning distance
                                  //cgk also used to bring car to front of grid           330
int cgk_fwd_end_turn = 350;       //cgk used for the last move in the turn                350
int cgk_turn_delay = 1000;        //cgk time required to wait for a turn to complete
                                  //cgk the time is used for a full 1->254 ie 
                                  //cgk hard left to hard right turn                      1000
int cgk_brake_time = 100;         //cgk time to apply a reverse direction dc motor
                                  //cgk move to act like a brake                          100
int cgk_motor_speed = 210;        //cgk PWM dc motor speed 0-255                          210
int cgk_fwd_one = 470;            //cgk distance to move forward 500mm                    500
int cgk_back_one = 500;           //cgk distance to move backward 500mm                   550
int cgk_first_angle_divider = 2;  //cgk divide difference in heading and course 
                                  //cgk to determine correction angle on first
                                  //cgk part of fwd movement                              2                    
// **************** END SPECIFIC VARIABLES **************************************************

void setup() 
{
    pinMode( 19, INPUT_PULLUP ); // fix Serial1
    Serial.begin(115200);
    BT.begin(9600);
    
    Serial.print("This is not an entire joke");
    // *** POZYX SETUP *** //
    if(Pozyx.begin() == POZYX_FAILURE)
    {
        if(test_mode == 1)
        {
            Serial.println(F("ERROR: Unable to connect to POZYX shield"));
            Serial.println(F("Reset required"));
            delay(100);
            abort();
        }
        else if(test_mode == 2)
        {
            BT.println(F("ERROR: Unable to connect to POZYX shield"));
            BT.println(F("Reset required"));
            delay(100);
            abort();
        }
    }
    // Clear all previous devices in the device list
    Pozyx.clearDevices(NULL);
    // Calibration
    setAnchorsManual();
    // Sets the positioning algorithm
    Pozyx.setPositionAlgorithm(algorithm, dimension, NULL);
    //delay(1000); // Delay not required

    // sets the heading offset
    headingInit();                    //NEW 31/05
    
    //delay(1000); //delay not required

    // *** ODS SETUP *** //
    //ObstacleSensor::calculateSoundCm(tempPin);
    ObstacleSensor::updateOdsData(x_loc, y_loc, heading);
    // *** END ODS SETUP *** //
    
    // *** N&M SETUP *** //
    // *** END N&M SETUP *** //
    // *** MC SETUP *** //
    AFMS.begin();
    servo1.attach(7);  // Attach a servo to pin #7
    // *** END MC SETUP *** //
    if(test_mode == 0)
    {
        delay(10000);       // Delay at end of setup to give time to power on
    }
}

void loop() 
{
    // *** LOC LOOP *** //
    getCoordinatesV2(100);
    getHeading();
    if(test_mode == 1)
    {
        Serial.print("Heading: "); Serial.println(heading);
        Serial.println("PositionX: " + String(x_loc) + "\tgridX: " + String(x_loc/500 + 1));
        Serial.println("PositionY: " + String(y_loc) + "\tgridY: " + String(y_loc/500 + 1));
    }
    else if(test_mode == 2)
    {
        BT.print("Heading: "); Serial.println(heading);
        BT.println("PositionX: " + String(x_loc) + "\tgridX: " + String(x_loc/500 + 1));
        BT.println("PositionY: " + String(y_loc) + "\tgridY: " + String(y_loc/500 + 1));
    }
    // *** ODS LOOP *** //
    ObstacleSensor::updateOdsData(x_loc, y_loc, heading);
    int probeNum = 2;
    for(int i = 0; i < probeNum; i++)
    {
        ods.detectAllSensors();
    }
    if(test_mode == 1)
    {
        frontSensor.printDistance("Front");
        leftSensor.printDistance("Left");
        rightSensor.printDistance("Right");
    }
    else if(test_mode == 2)
    {
        btPrintSensorDistances("Front", &frontSensor);
        btPrintSensorDistances("Left", &leftSensor);
        btPrintSensorDistances("Right", &rightSensor);
    }

    // *** N&M LOOP *** // 
    if(returnToStart)
    {
        if(test_mode == 1)
        {
            Serial.println("Returning to start");
        }
        else if(test_mode == 2)
        {
            BT.println("Returning to start");
        }
        xPosTarget = xPosStart;
        yPosTarget = yPosStart;
    }
    
    nav.reprioritiseMap(xPosTarget, yPosTarget);

    if(test_mode == 1)
    {
        nav.printMap();
    }
    else if(test_mode == 2)
    {
        btPrintMap();
    }
    int x_loc_grid = nav.milliToGrid(x_loc); 
    int y_loc_grid = nav.milliToGrid(y_loc); 

    
    if(x_loc_grid == xPosTarget && y_loc_grid == yPosTarget)
    {
        moveForward();
        returnToStart = true;
    }
    /*
    {
        Serial.print("I'm in the target location at ["); Serial.print(x_loc_grid); Serial.print("]["); Serial.print(y_loc_grid); Serial.print("]");
        int noSides = nav.sidesClear(x_loc, y_loc, heading);
        while(noSides != 1 && noSides != 2)
        {
            Serial.print("Movement: Returning reverse");
            moveBackward();
            noSides = nav.sidesClear(x_loc, y_loc, heading); 
        }

        if(noSides == 2)
        {
            Serial.print("\tMovement: Returning left turn");
            turnLeft();
            Serial.print("\tMovement: Returning reverse");
            moveBackward();
            Serial.print("\tMovement: Returning right turn");
            turnLeft();
            returnToStart = true;
        }
        else if(noSides == 1)
        {
            Serial.print("\tMovement: Returning right turn");
            turnRight();
            Serial.print("\tMovement: Returning reverse");
            moveBackward();
            Serial.print("\tMovement: Returning right turn");
            turnRight();
            returnToStart = true;
        }
    }
        */
    else
    {   
        int command = nav.nextMove(x_loc, y_loc, xPosTarget, yPosTarget, heading);
        if (command == 1)
        {
            if(test_mode == 1)
            {
                Serial.print("\tMovement: Forward");
            }
            else if(test_mode == 2)
            {
                BT.print("\tMovement: Forward");
            }
            moveForward();
        }         
        else if (command == 2)
        {
            if(test_mode == 1)
            {
                Serial.print("\tMovement: Right turn");
            }
            else if(test_mode == 2)
            {
                BT.print("\tMovement: Right turn");
            }
            turnRight();            
        }
        else if (command == 4)
        {
            if(test_mode == 1)
            {
                Serial.print("\tMovement: Left turn");  
            }
            else if(test_mode == 2)
            {
                BT.print("\tMovement: Left turn");  
            }
            turnLeft();
        }
        else if (command == 3)
        {
            if(test_mode == 1)
            {
                Serial.print("\tMovement: Reverse");
            }
            else if(test_mode == 2)
            {
                BT.print("\tMovement: Reverse");
            }
            moveBackward();
        }   
    }  

      // *** BT LOOP *** //
      sendBTData(); // Gets all data for GUI and sends over bluetooth
}

// Function to manually set the anchor coordinates as supplied by pozyx website
void setAnchorsManual()
{
      for(int i = 0; i < num_anchors; i++)
      {
            device_coordinates_t anchor;
            anchor.network_id = anchors[i];
            anchor.flag = 0x1;
            anchor.pos.x = anchors_x[i];
            anchor.pos.y = anchors_y[i];
            anchor.pos.z = heights[i];
            Pozyx.addDevice(anchor, NULL);
      }
      if(num_anchors > 4)
      {
            Pozyx.setSelectionOfAnchors(POZYX_ANCHOR_SEL_AUTO, num_anchors, NULL);
      }
}

// Get position information
void getCoordinates (int avgNum){
    coordinates_t position;
  int status;
  int32_t out_x;
  int32_t out_y;
  for (int i = 0; i<avgNum; i++){
    status = Pozyx.doPositioning(&position, dimension, height, algorithm);
    if(status == POZYX_SUCCESS){
      if(position.x>0 && position.y>0){
        if(position.x<5001 && position.y<5001){
          out_x = out_x + position.x;
          out_y = out_y + position.y;
        }else{
          i--;
        }
      }else{
        i--;
      }
    }else{
      i--;
    }
  }
  x_loc = out_x/avgNum;
  y_loc = out_y/avgNum;
}

// NEW get coordinates
void getCoordinatesV2(int avgNum){
  coordinates_t position;
  int status;
  //int finalOutX;
  //int finalOutY;
  int num2remove = int(avgNum * 0.1);

  int32_t out_x;
  int32_t out_y;
  int avgArrayX[avgNum];
  int avgArrayY[avgNum];

   for (int i = 0; i<avgNum; i++){
    status = Pozyx.doPositioning(&position, dimension, height, algorithm);
    if(status == POZYX_SUCCESS){
      if(position.x>0 && position.y>0){
        if(position.x<5001 && position.y<5001){
          avgArrayX[i] = position.x;
          avgArrayY[i] = position.y;
        }else{
          i--;
        }
      }else{
        i--;
      }
    }else{
      i--;
    }
  }

for (int k = 0; k<num2remove; k++){
  //Serial.print("Test Count: ");
  //Serial.println(k);
  // arrays for finding max and min locations
  // [x max, x min, y max, y min]
  int arMaxMin[] = {0, 5000, 0, 5000};
  int arMaxMinLoc[] = {0, 0, 0, 0};

  // find the min and max locations
  for(int i = 0; i<avgNum; i++){
    if (avgArrayX[i]>arMaxMin[0]){
      arMaxMin[0] = avgArrayX[i];
      arMaxMinLoc[0] = i;
    }
    if (avgArrayX[i]<arMaxMin[1]){
      arMaxMin[1] = avgArrayX[i];
      arMaxMinLoc[1] = i;
    }
    if (avgArrayY[i]>arMaxMin[2]){
      arMaxMin[2] = avgArrayY[i];
      arMaxMinLoc[2] = i;
    }
    if (avgArrayY[i]<arMaxMin[3]){
      arMaxMin[3] = avgArrayY[i];
      arMaxMinLoc[3] = i;
    }
  }

    if(arMaxMinLoc[0]<1){
      avgArrayX[arMaxMinLoc[0]] = avgArrayX[arMaxMinLoc[0]+1];
    }else{
      avgArrayX[arMaxMinLoc[0]] = avgArrayX[arMaxMinLoc[0]-1];
    }

    if(arMaxMinLoc[1]<1){
      avgArrayX[arMaxMinLoc[1]] = avgArrayX[arMaxMinLoc[1]+1];
    }else{
      avgArrayX[arMaxMinLoc[1]] = avgArrayX[arMaxMinLoc[1]-1];
    }

    if(arMaxMinLoc[2]<1){
      avgArrayY[arMaxMinLoc[2]] = avgArrayY[arMaxMinLoc[2]+1];
    }else{
      avgArrayY[arMaxMinLoc[2]] = avgArrayY[arMaxMinLoc[2]-1];
    }

    if(arMaxMinLoc[3]<1){
      avgArrayY[arMaxMinLoc[3]] = avgArrayY[arMaxMinLoc[3]+1];
    }else{
      avgArrayY[arMaxMinLoc[3]] = avgArrayY[arMaxMinLoc[3]-1];
    }
}
    for (int i = 0; i<avgNum; i++){
      out_x = out_x + avgArrayX[i];
      out_y = out_y + avgArrayY[i];
    }
  x_loc = out_x/avgNum;
  y_loc = out_y/avgNum;

}

// initializes the heading by creating an offset so that the heading at init is 0deg  NEW 31/05
void headingInit(){
  euler_angles_t euler_angles;
  int status;
  status = Pozyx.getEulerAngles_deg(&euler_angles, NULL);
  if(status == POZYX_SUCCESS){
    headingOffset = euler_angles.heading;
  }
}

// Get heading info updates global heading variable with what appears to be magnetic heading
void getHeading(){
  euler_angles_t euler_angles;
  int status;
  status = Pozyx.getEulerAngles_deg(&euler_angles, NULL);
  if(status == POZYX_SUCCESS){
    heading = euler_angles.heading - headingOffset;
  }
  // this corrects if the offset has taken the value below 0deg
  if(heading<0){
      heading = heading + 360;
  }
}

// get all data from Mazing and Pozyx and concat into string for BT send (HOFFY)
void sendBTData()
{
  // Get grid data
  for(int i = 1; i < 11; i++) // concat int data into one long string with delimeters
  {
    for(int j = 10; j > 0; j--)
    {
      result = result + nav.grid_[i][j][2] + ";"; // nav.grid_ is glob var
    }
  }
  result.remove(result.length() - 1); // remove last unwanted semicolon
// Get Pozyx data, prev loc data is for calculating speed on MATLAB
  String temp = ":" + String(heading) + ":" + String(x_loc) + ":" + String(y_loc) + ":" + String(x_loc_prev)
    + ":" + String(y_loc_prev) + ":" + "w"; // from pozyx lob var
  x_loc_prev = x_loc; //update prev xy values
  y_loc_prev = y_loc;
  result += temp;
  BT.println(result); // tx string
  delay(100); //delay in us
  BT.flush();
  result = ";911;";
}

//--------------------------------------cgk-------------------------------------------------------
//cgk these moves are based on the fact that north is up, 0 and that the
//cgk bottom left of the grid is position 0,0 i think
//cgk that means going north or east increases the x and y coord respectively
//cgk probably need a method to switch from magnetic to grid co-ordinates, 
//cgk but can test as is with mag north gid squares

void turnLeft(){
  getHeading();
  myMotor->setSpeed(cgk_motor_speed);
  int current_heading = heading;              //cgk record current heading
  int new_heading = 0;                        //cgk new heading at turn completion
  int course_heading = 0;                     //cgk record previous heading
  
  //cgk determine distance and direction to move
  if ((current_heading > 315) || (current_heading <45)){ //north
    course_heading = 360;
    new_heading = 270; 
    }
  
  if ((current_heading > 45) && (current_heading < 135)){//east
    course_heading = 90;
    new_heading = 1;                          //cgk WARNING CHANGE IF STATEMENT BELOW
    }
  
  if ((current_heading > 135) && (current_heading < 225)){//south
    course_heading = 180;
    new_heading = 90;
    }
  
  if ((current_heading > 225) && (current_heading < 315)){//west
    course_heading = 270;
    new_heading = 180;
    }

   if (test_mode == 1)
   {
       Serial.println("Current heading is ");
       Serial.println(heading);
       Serial.println("Course heading was");
       Serial.println(course_heading);
       Serial.println("Turning to new course of ");
       Serial.println(new_heading);
   }
   else if (test_mode == 2)
   {
       BT.println("Current heading is ");
       BT.println(heading);
       BT.println("Course heading was");
       BT.println(course_heading);
       BT.println("Turning to new course of ");
       BT.println(new_heading);
   }
   
   //cgk move the car to the front of the box
   servo1.write(map(cgk_straight, 0, 255, 0, 180));//centre
   delay(cgk_turn_delay);
   myMotor->run(FORWARD);
   delay(cgk_fwd_little);
   myMotor->run(BACKWARD);
   delay(cgk_brake_time);
   myMotor->run(RELEASE);
   delay(cgk_turn_delay);

   //cgk turn left and then keep going until you get there?
   
   cgk_left();
   getHeading();
   current_heading = heading;
   if(test_mode == 1)
   {
       Serial.println("-");
       Serial.println("Current heading is ");
       Serial.println(heading);
   }
   else if(test_mode == 2)
   {
       BT.println("-");
       BT.println("Current heading is ");
       BT.println(heading);
   }

   //cgk wiggle left with cgk_left until we get halfway to new heading
   
   while (current_heading > (new_heading+40))
   {
        cgk_left();
        getHeading();
        current_heading = heading;
        if(test_mode == 1)
        {
             Serial.println("-");
             Serial.println("Current heading is ");
             Serial.println(heading);
        }
        else if(test_mode == 2)
        {
             BT.println("-");
             BT.println("Current heading is ");
             BT.println(heading);
        }
    }

   //cgk now drive forward and hard left to reach centre of target square, ie left one
   
   servo1.write(map(cgk_straight, 0, 255, 0, 180));          //cgk centre
   delay(cgk_turn_delay);
   servo1.write(map(1, 0, 255, 0, 180));            //cgk left
   delay(cgk_turn_delay);
   myMotor->run(FORWARD);
   delay((cgk_fwd_end_turn*2.1));                   //cgk how far fwd                   2.5*cgk_fwd_end_turn
   myMotor->run(BACKWARD);
   delay(cgk_brake_time*1.5);                         //cgk brake time, compensate for 
                                                    //cgk greater speed?                1*cgk_brake_time
   myMotor->run(RELEASE);
   delay(cgk_brake_time);
   servo1.write(map(cgk_straight, 0, 255, 0, 180));          //cgk centre
   delay(cgk_turn_delay);
   
   getHeading();
   current_heading = heading;
   
   if ((new_heading==1)&&(current_heading>340)){    //cgk this ensures that the north compass  
    current_heading=1;                              //cgk wraparound, when turning left from east,
   }                                                //cgk will not cause the car to keep turning.
   
   while (current_heading > (new_heading+8)){       //cgk wiggle left until we get 
                                                    //cgk close to correct new_heading  new_heading+8
     cgk_left();
     getHeading();
     current_heading = heading;

     servo1.write(map(cgk_straight, 0, 255, 0, 180));//centre
     delay(cgk_turn_delay);
   
     if(test_mode == 1)
     {
         Serial.println("-");
         Serial.println("Current heading is ");
         Serial.println(heading);
     }
     else if(test_mode == 2)
     {
         BT.println("-");
         BT.println("Current heading is ");
         BT.println(heading);
     }
   }
  }
//--------------------------------------cgk---------------------------------------------------

//--------------------------------------cgk-------------------------------------------------------
//cgk these moves are based on the fact that north is up, 0 and that the
//cgk bottom left of the grid is position 0,0 i think
//cgk that means going north or east increases the x and y coord respectively
//cgk probably need a method to switch from magnetic to grid co-ordinates, 
//cgk but can test as is with mag north gid squares

void turnRight(){
  getHeading();
  myMotor->setSpeed(cgk_motor_speed);
  int current_heading = heading;              //cgk record current heading
  int new_heading = 0;                        //cgk new heading at turn completion
  int course_heading = 0;                     //cgk record previous heading
  
  //cgk determine distance and direction to move
  if ((current_heading > 315) || (current_heading <45)){ //north
    course_heading = 1;
    new_heading = 90; 
    }
  
  if ((current_heading > 45) && (current_heading < 135)){//east
    course_heading = 90;
    new_heading = 180;                          
    }
  
  if ((current_heading > 135) && (current_heading < 225)){//south
    course_heading = 180;
    new_heading = 270;
    }
  
  if ((current_heading > 225) && (current_heading < 315)){//west
    course_heading = 270;
    new_heading = 359;                            //cgk WARNING CHANGE IF STATEMENT BELOW
    }

   if(test_mode == 1)
   {
       Serial.println("Current heading is ");
       Serial.println(heading);
       Serial.println("Course heading was");
       Serial.println(course_heading);
       Serial.println("Turning to new course of ");
       Serial.println(new_heading);
   }
   else if(test_mode == 2)
   {
       BT.println("Current heading is ");
       BT.println(heading);
       BT.println("Course heading was");
       BT.println(course_heading);
       BT.println("Turning to new course of ");
       BT.println(new_heading);
   }
   
   //cgk move the car to the front of the box
   servo1.write(map(cgk_straight, 0, 255, 0, 180));//centre
   delay(cgk_turn_delay);
   myMotor->run(FORWARD);
   delay(cgk_fwd_little);
   myMotor->run(BACKWARD);
   delay(cgk_brake_time);
   myMotor->run(RELEASE);
   delay(cgk_turn_delay);

   //cgk turn right and then keep going until you get there?                  ***
   
   cgk_right();
   getHeading();
   current_heading = heading;
   if(test_mode == 1)
   {
       Serial.println("-");
       Serial.println("Current heading is ");
       Serial.println(heading);
   }
   else if(test_mode == 2)
   {
       BT.println("-");
       BT.println("Current heading is ");
       BT.println(heading);
   }

   //cgk wiggle right with cgk_right until we get halfway to new heading
   
   while (current_heading < (new_heading-40)){
     cgk_right();
     getHeading();
     current_heading = heading;
     if(test_mode == 1)
     {
         Serial.println("-");
         Serial.println("Current heading is ");
         Serial.println(heading);
     }
     else if(test_mode == 2)
     {
         BT.println("-");
         BT.println("Current heading is ");
         BT.println(heading);
     }
    }

   //cgk now drive forward and hard right to reach centre of target square, ie the right one
   
   servo1.write(map(cgk_straight, 0, 255, 0, 180));          //cgk centre
   delay(cgk_turn_delay);
   servo1.write(map(254, 0, 255, 0, 180));            //cgk right
   delay(cgk_turn_delay);
   myMotor->run(FORWARD);
   delay((cgk_fwd_end_turn*2.1));                   //cgk how far fwd                   2.5*cgk_fwd_end_turn
   myMotor->run(BACKWARD);
   delay(cgk_brake_time*1.5);                         //cgk brake time, compensate for 
                                                    //cgk greater speed?                1*cgk_brake_time
   myMotor->run(RELEASE);
   delay(cgk_brake_time);
   servo1.write(map(122, 0, 255, 0, 180));          //cgk centre
   delay(cgk_turn_delay);
   
   getHeading();
   current_heading = heading;
                                                    //cgk WARNING check above warning
   if ((new_heading==359)&&(current_heading<20)){   //cgk this ensures that the north compass  
    current_heading=1;                              //cgk wraparound, when turning right from west,
   }                                                //cgk will not cause the car to keep turning.
   
   while (current_heading < (new_heading-8)){       //cgk wiggle right until we get 
                                                    //cgk close to correct new_heading  new_heading-8
     cgk_right();
     getHeading();
     current_heading = heading;

     servo1.write(map(cgk_straight, 0, 255, 0, 180));//centre
     delay(cgk_turn_delay);
   
     if(test_mode == 1)
     {
         Serial.println("-");
         Serial.println("Current heading is ");
         Serial.println(heading);
     }
     else if(test_mode == 2)
     {
         BT.println("-");
         BT.println("Current heading is ");
         BT.println(heading);
     }
   }
  }
//--------------------------------------cgk---------------------------------------------------


//--------------------------------------cgk---------------------------------------------------
//cgk turns hard right, then goes backwards, then hard left, then goes forwards.

void cgk_left(){
  myMotor->setSpeed(cgk_motor_speed);
 
  servo1.write(map(254, 0, 255, 0, 180));//right
  delay(cgk_turn_delay);
  myMotor->run(BACKWARD);
  delay(cgk_back_little);
  myMotor->run(FORWARD);
  delay(cgk_brake_time);
  myMotor->run(RELEASE);
  servo1.write(map(1, 0, 255, 0, 180));//left
  delay(cgk_turn_delay);
  myMotor->run(FORWARD);
  delay(cgk_fwd_little);
  myMotor->run(BACKWARD);
  delay(cgk_brake_time);
  myMotor->run(RELEASE);
  delay(cgk_brake_time);
  servo1.write(map(cgk_straight, 0, 255, 0, 180));//centre
  delay(cgk_turn_delay); 
  }
//--------------------------------------cgk----------------------------------------------------

//--------------------------------------cgk---------------------------------------------------
//cgk turns hard left, then goes backwards, then hard right, then goes forwards.

void cgk_right(){
  myMotor->setSpeed(cgk_motor_speed);
 
  servo1.write(map(1, 0, 255, 0, 180));//left
  delay(cgk_turn_delay);
  myMotor->run(BACKWARD);
  delay(cgk_back_little);
  myMotor->run(FORWARD);
  delay(cgk_brake_time);
  myMotor->run(RELEASE);
  servo1.write(map(254, 0, 255, 0, 180));//right
  delay(cgk_turn_delay);
  myMotor->run(FORWARD);
  delay(cgk_fwd_little);
  myMotor->run(BACKWARD);
  delay(cgk_brake_time);
  myMotor->run(RELEASE);
  delay(cgk_brake_time);
  servo1.write(map(cgk_straight, 0, 255, 0, 180));//centre
  delay(cgk_turn_delay); 
  }
//--------------------------------------cgk----------------------------------------------------


//---------------------------------------cgk---------------------------------------------------
//cgk move the car forward 500mm.
//cgk the plan is to take the current position to determine how far forward to move, and at 
//cgk what perpendicular offset, so that the final heading and position is centred in the 
//cgk forward gird square. It aims to finish on the correct heading too.

void moveForward(){
  myMotor->setSpeed(cgk_motor_speed);
  //cgk this old code is for pozyx integration for x and y positioning
  //cgk i will just use heading info, this assumes the car remains centred
  //cgk in the square and that it always moves 500mm to the exact centre and correct heading of the next square
  getHeading();
  //getCoordinates();
  int current_heading = heading;
  int course_heading = 0;
  int course_offset = 0;
//cgk determine heading
    if(test_mode == 1)
    {
         Serial.println("-");
         Serial.println("Current heading is ");
         Serial.println(heading);
         Serial.println("Going Forwards");
    }
    else if(test_mode == 2)
    {
         BT.println("-");
         BT.println("Current heading is ");
         BT.println(heading);
         BT.println("Going Forwards");
    }
     
  if ((current_heading > 315) || (current_heading <45)){ //north
    course_heading = 1;
  }
  if ((current_heading > 45) && (current_heading <135)){ //east
    course_heading = 90;
  }
  if ((current_heading > 135) && (current_heading <225)){ //south
    course_heading = 180;
  }
  if ((current_heading > 225) && (current_heading <315)){ //west
    course_heading = 270;
  }
  
  servo1.write(map(cgk_straight, 0, 255, 0, 180));//centre
  delay(cgk_turn_delay);

  //cgk check if we are facing close to the course_heading and wiggle left or right once only if needed
  if ((current_heading < course_heading-8)&&(course_heading!=1)){       //cgk too far left
    cgk_right();
    if(test_mode == 1)
    {
        Serial.println("adjusting to the right");
    }
    else if(test_mode == 2)
    {
        BT.println("adjusting to the right");
    }
  }

  if ((current_heading > course_heading+8)&&(course_heading!=1)){       //cgk too far right
    cgk_left();
    if(test_mode == 1)
    {
        Serial.println("adjusting to the left");
    }
    else if(test_mode == 2)
    {
        BT.println("adjusting to the left");
    }
  }

  if (course_heading==1){                                               //cgk takes care of north moves
    if ((current_heading<353)&&(current_heading>315))
    {
        cgk_right();
        if(test_mode == 1)
        {
            Serial.println("adjusting to the right");
        }
        else if(test_mode == 2)
        {
            BT.println("adjusting to the right");
        }
    }
    if (current_heading>7&&current_heading<45)
    {
        cgk_left();
      if(test_mode == 1)
      {
            Serial.println("adjusting to the left");
      }
      else if(test_mode == 2)
      {
            BT.println("adjusting to the left");
      }
    }
    
  }
  if(test_mode == 1)
  {
     Serial.println("-");
     Serial.println("Current heading is ");
     Serial.println(heading);
  }
  else if(test_mode == 2)
  {
     BT.println("-");
     BT.println("Current heading is ");
     BT.println(heading);
  }

  //cgk move forward the 500mm
if(test_mode == 1)
{
     Serial.println("MOVE NOW");
}
else if(test_mode == 2)
{
     BT.println("MOVE NOW");
}
  servo1.write(map(cgk_straight, 0, 255, 0, 180));//centre
  delay(cgk_turn_delay);
  myMotor->run(FORWARD);
  delay(cgk_fwd_one*2);
  myMotor->run(BACKWARD);
  delay(cgk_brake_time*2);
  myMotor->run(RELEASE);
  delay(cgk_brake_time);
  servo1.write(map(cgk_straight, 0, 255, 0, 180));//centre
  delay(cgk_turn_delay);

  getHeading();
  //getCoordinates();
 current_heading = heading;
    if(test_mode == 1)
    {
         Serial.println("-");
         Serial.println("Current heading is ");
         Serial.println(heading);
    }
    else if(test_mode == 2)
    {
         BT.println("-");
         BT.println("Current heading is ");
         BT.println(heading);
    }

  //cgk check if we are facing close to the course_heading and wiggle left or right once only if needed
  if ((current_heading < course_heading-8)&&(course_heading!=1)){       //cgk too far left
    cgk_right();
  }

  if ((current_heading > course_heading+8)&&(course_heading!=1)){       //cgk too far right
    cgk_left();
  }

   if (course_heading==1){                                               //cgk takes care of north moves
    if ((current_heading<353)&&(current_heading>315)){
      cgk_right();
      if(test_mode == 1)
      {
        Serial.println("adjusting to the right");
      }
      else if(test_mode == 2)
      {
        BT.println("adjusting to the right");
      }
    }
    if ((current_heading>7)&&(current_heading<45)){
      cgk_left();
        if(test_mode == 1)
        {
            Serial.println("adjusting to the left");
        }
        else if(test_mode == 2)
        {
            BT.println("adjusting to the left");
        }
    }
   }

   servo1.write(map(cgk_straight, 0, 255, 0, 180));//centre
   delay(cgk_turn_delay);
   
    if(test_mode == 1)
    {
         Serial.println("-");
         Serial.println("Current heading is ");
         Serial.println(heading);
         Serial.println("Finished moving forwad");
    }
    else if(test_mode == 2)
    {
         BT.println("-");
         BT.println("Current heading is ");
         BT.println(heading);
         BT.println("Finished moving forwad");
    }
}

//---------------------------------------cgk---------------------------------------------------

//---------------------------------------cgk---------------------------------------------------
//cgk move the car backward 500mm.
//cgk the plan is to take the current position to determine how far backward to move, and at 
//cgk what perpendicular offset, so that the final heading and position is centred in the 
//cgk backward gird square. It aims to finish on the correct heading too.

void moveBackward(){
  myMotor->setSpeed(cgk_motor_speed);
  //cgk this old code is for pozyx integration for x and y positioning
  //cgk i will just use heading info, this assumes the car remains centred
  //cgk in the square and that it always moves 500mm to the exact centre and correct heading of the next square
  getHeading();
  //getCoordinates();
  int current_heading = heading;
  int course_heading = 0;
  int course_offset = 0;
//cgk determine heading
    if(test_mode == 1)
    {
         Serial.println("-");
         Serial.println("Current heading is ");
         Serial.println(heading);
    }
    else if(test_mode == 2)
    {
         BT.println("-");
         BT.println("Current heading is ");
         BT.println(heading);
    }
  if (current_heading > 315 || current_heading <45){ //north
    course_heading = 1;
  }
  if ((current_heading > 45) && (current_heading <135)){ //east
    course_heading = 90;
  }
  if ((current_heading > 135) && (current_heading <225)){ //south
    course_heading = 180;
  }
  if ((current_heading > 225) && (current_heading <315)){ //west
    course_heading = 270;
  }
  
  servo1.write(map(cgk_straight, 0, 255, 0, 180));//centre
  delay(cgk_turn_delay);

    if(test_mode == 1)
    {
         Serial.println("-");
         Serial.println("Current heading is ");
         Serial.println(heading);
    }
    else if(test_mode == 2)
    {
         BT.println("-");
         BT.println("Current heading is ");
         BT.println(heading);
    }
  //cgk check if we are facing close to the course_heading and wiggle left or right once only if needed
  if ((current_heading < course_heading-8)&&(course_heading!=1)){       //cgk too far left
    cgk_right();
    if(test_mode == 1)
    {
        Serial.println("adjusting to the right");
    }
    else if(test_mode == 2)
    {
        BT.println("adjusting to the right");
    }
  }

  if ((current_heading > course_heading+8)&&(course_heading!=1)){       //cgk too far right
    cgk_left();
    if(test_mode == 1)
    {
         Serial.println("adjusting to the left");
    }
    else if(test_mode == 2)
    {
         BT.println("adjusting to the left");
    }
  }

  if (course_heading==1){                                               //cgk takes care of north moves
    if (current_heading<353){
      cgk_right();
        if(test_mode == 1)
        {
             Serial.println("adjusting to the right");
        }
        else if(test_mode == 2)
        {
             BT.println("adjusting to the right");
        }
    }
    if (current_heading>7){
      cgk_left();
      if(test_mode == 1)
      {
            Serial.println("adjusting to the left");
      }
      else if(test_mode == 2)
      {
            BT.println("adjusting to the left");
      }
    }
    
  }
  if(test_mode == 1)
  {
     Serial.println("-");
     Serial.println("Current heading is ");
     Serial.println(heading);
     Serial.println("MOVE NOW");
  }
  else if(test_mode == 2)
  {
     BT.println("-");
     BT.println("Current heading is ");
     BT.println(heading);
     BT.println("MOVE NOW");
  }

    //cgk move forward the 500mm
    servo1.write(map(cgk_straight, 0, 255, 0, 180));//centre
    delay(cgk_turn_delay);
    myMotor->run(BACKWARD);
    delay(cgk_back_one*2);
    myMotor->run(FORWARD);
    delay(cgk_brake_time*2);
    myMotor->run(RELEASE);
    delay(cgk_brake_time);
    servo1.write(map(cgk_straight, 0, 255, 0, 180));//centre
    delay(cgk_turn_delay);
    if(test_mode == 1)
    {
         Serial.println("-");
         Serial.println("Current heading is ");
         Serial.println(heading);
    }
    else if(test_mode == 2)
    {
         BT.println("-");
         BT.println("Current heading is ");
         BT.println(heading);
    }
  getHeading();
  //getCoordinates();
  current_heading = heading;

  //cgk check if we are facing close to the course_heading and wiggle left or right once only if needed
  if ((current_heading < course_heading-8)&&(course_heading!=1)){       //cgk too far left
    cgk_right();
  }

  if ((current_heading > course_heading+8)&&(course_heading!=1)){       //cgk too far right
    cgk_left();
  }

  if (course_heading==1){                                               //cgk takes care of north moves
    if ((current_heading<353)&&(current_heading>315)){
      cgk_right();
     if(test_mode == 1)
     {
        Serial.println("adjusting to the right");
     }
     else if(test_mode == 2)
     {
        BT.println("adjusting to the right");
     }
    }
    if ((current_heading>7)&&(current_heading<45)){
      cgk_left();
     if(test_mode == 1)
     {
        Serial.println("adjusting to the left");
     }
     else if(test_mode == 2)
     {
        BT.println("adjusting to the left");
     }
    }
    
  }
  servo1.write(map(cgk_straight, 0, 255, 0, 180));//centre
   delay(cgk_turn_delay);
   
     if(test_mode == 1)
     {
         Serial.println("-");
         Serial.println("Current heading is ");
         Serial.println(heading);
         Serial.println("Finished moving backwards");
         }
     else if(test_mode == 2)
     {
         BT.println("-");
         BT.println("Current heading is ");
         BT.println(heading);
         BT.println("Finished moving backwards");
     }
}
//---------------------------------------cgk---------------------------------------------------

//=================================== BT PRINTS ===============================================
void btPrintMap()
{
    if(test_mode == 2)
    {
        BT.println("VALUES:");
        for(int i = 0; i < HEIGHT + 2; i++)
        {
            for(int j = 0; j < WIDTH + 2; j++)
            {
                BT.print(nav.grid_[i][j][ELEMENT_VALUE]);
                BT.print("\t");
            }
            BT.println();
        }   
    }
}

void btPrintSensorDistances(String sensorName, ObstacleSensor* sensor)
{
        if(test_mode == 2)
    {
        BT.print(sensorName);
        BT.print(": ");
        
        if (sensor->distance_ == 1023.0f)
        {
            BT.println("Distance out of range");
        }
        else 
        {
            BT.print("xPos_: ");
            BT.print(sensor->xPos_);
            BT.print(" cm\t");
    
            BT.print("yPos_: ");
            BT.print(sensor->yPos_);
            BT.print(" cm\t ");     
            
            BT.print("Distance: ");
            BT.print(sensor->distance_);
            BT.print(" cm\t");
    
            BT.print("xDistComp_: ");
            BT.print(sensor->xDistComp_);
            BT.print(" cm\t");
    
            BT.print("yDistComp_: ");
            BT.print(sensor->yDistComp_);
            BT.print(" cm\t ");
            
            BT.print("objXDist_: ");
            BT.print(sensor->objXDist_);
            BT.print(" cm\t");
    
            BT.print("objYDist_: ");
            BT.print(sensor->objYDist_);
            BT.print(" cm\t ");
    
            BT.print("Coord [x, y] = [");
            BT.print(sensor->objX_);
            BT.print(", ");
            BT.print(sensor->objY_);
            BT.print("]\t");
                   
            BT.print("Grid: [x, y] = [");
            BT.print(sensor->gridX_);
            BT.print(", ");
            BT.print(sensor->gridY_);
            BT.println("]");
        }
    }
}

