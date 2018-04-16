#ifndef LOCATOR_H
#define LOCATOR_H

class Locator
{
    public:
        int x_loc = -99;                                                 //The measured x location in mm from coordinate (0,0)
        int y_loc = -99;                                                 //The measured y location in mm from coordinate (0,0)
        int heading = -99;                                               //The magnetic heading in degrees
    
    private: 
        //! Function to manually set the anchor coordinates
        //! supplied by pozyx website
        void setAnchorsManual();

        //! Initial setup parameters of pozyx system
        void pozyxSetup();

    public:
        //! Default constructor
        Locator();

        //! Get position information
        void getCoordinates();

        //! Get heading info updates global heading variable with what appears to be magnetic heading
        void getHeading();
};

#endif
