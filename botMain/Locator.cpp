#include <Pozyx.h>
#include <Pozyx_definitions.h>
#include <Wire.h>
#include "Locator.h"

////////////////////////////////////////////////
////////////////// PARAMETERS //////////////////
////////////////////////////////////////////////

const int num_to_avg = 5;                                         //number of position readings to average out for the output of position data
const uint8_t num_anchors = 4;                                    // the number of anchors
uint16_t anchors[num_anchors] = {0x6827, 0x6821, 0x687c, 0x6851}; // the network id of the anchors
int32_t anchors_x[num_anchors] = {0, 4900, 0, 4900};              // anchor x-coorindates in mm
int32_t anchors_y[num_anchors] = {0, 0, 3900, 3900};              // anchor y-coordinates in mm
int32_t heights[num_anchors] = {750, 1700, 1700, 750};            // anchor z-coordinates in mm

uint8_t algorithm = POZYX_POS_ALG_UWB_ONLY;                       // positioning algorithm to use. try POZYX_POS_ALG_TRACKING for fast moving objects.
uint8_t dimension = POZYX_2D;                                     // positioning dimension ie 2D, 2_5D and 3D
int32_t height = 0;                                               // The z position of pozyx in mm

////////////////////////////////////////////////

Locator::Locator()
{
    pozyxSetup();
    getCoordinates();
    getHeading();
    Serial.println("Locator constructor");
}

void Locator::setAnchorsManual()
{
    for(int i = 0; i < num_anchors; i++){
        device_coordinates_t anchor;
        anchor.network_id = anchors[i];
        anchor.flag = 0x1;
        anchor.pos.x = anchors_x[i];
        anchor.pos.y = anchors_y[i];
        anchor.pos.z = heights[i];
        Pozyx.addDevice(anchor, NULL);
  }
  if (num_anchors > 4)
  {
    Pozyx.setSelectionOfAnchors(POZYX_ANCHOR_SEL_AUTO, num_anchors, NULL);
  }
}

void Locator::pozyxSetup() 
{
    Serial.begin(115200);
    if(Pozyx.begin() == POZYX_FAILURE)
    {
        Serial.println(F("ERROR: Unable to connect to POZYX shield"));
        Serial.println(F("Reset required"));
        delay(100);
        abort();
    }
    
    // clear all previous devices in the device list
    Pozyx.clearDevices(NULL);
    
    //calibration
    setAnchorsManual();
    
    // sets the positioning algorithm
    Pozyx.setPositionAlgorithm(algorithm, dimension, NULL);
    
    delay(1000); //delay not required
}

void Locator::getCoordinates ()
{
    coordinates_t position;
    int status;
    int out_x;
    int out_y;
    for (int i = 0; i<num_to_avg; i++)
    {
        status = Pozyx.doPositioning(&position, dimension, height, algorithm);
        if(status == POZYX_SUCCESS)
        {
            if(position.x>0 && position.y>0)
            {
                if(position.x<5001 && position.y<5001)
                {
                    out_x = out_x + position.x;
                    out_y = out_y + position.y;
                }
                else
                {
                  i--;
                }
            }
            else
            {
            i--;
            }
        }
        else
        {
          i--;
        }
    }
    x_loc = out_x/num_to_avg;
    y_loc = out_y/num_to_avg;
}

void Locator::getHeading()
{
    euler_angles_t euler_angles;
    int status;
    status = Pozyx.getEulerAngles_deg(&euler_angles, NULL);
    if(status == POZYX_SUCCESS)
    {
        //
        heading = euler_angles.heading;
    }
}

