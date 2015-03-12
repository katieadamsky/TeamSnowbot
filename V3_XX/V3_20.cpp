/*Performance Test 4: Transporting the Salt Bag
Written by Katie Adamsky 3/4/15
RJF 8:00 Team B6*/

#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include <FEHMotor.h>
#include <FEHRPS.h>
#include <FEHServo.h>

//Declarations for encoders & motors
ButtonBoard buttons(FEHIO::Bank3);
DigitalEncoder right_encoder(FEHIO::P0_0);
DigitalEncoder left_encoder(FEHIO::P0_2);
FEHMotor right_motor(FEHMotor::Motor0);
FEHMotor left_motor(FEHMotor::Motor1);
AnalogInputPin cds(FEHIO::P0_4); //CdS cell
FEHServo servo(FEHServo::Servo0); //servo for button, crank and oil switch mechanism
FEHServo saltservo(FEHServo::Servo2); //servo for scoop mechanism
DigitalInputPin bump(FEHIO::P0_4); //bump switch to determine if salt bag is in scoop


//values for motion functions
#define counts_per_rotation 33.7408
#define full_power 100
#define normal_power 25
#define half_power 50

//values for button-pressing functions
#define servomin 522
#define servomax 2352
#define redangle 0
#define whiteangle 60
#define blueangle 38

//values for salt bag functions
//STILL NEED TO BE DETERMINED EXPERIMENTALLY
#define scoopangle 0
#define startangle 180
#define saltholdingangle 120
#define saltservomin 500
#define saltservomax 2200

float encode;//for rotary encoding functions
float x, y, head;//for RPS functions
int r, w, b, num_buttons=0;//for button pressing function
int micro;//for scoop

void start_at_light(void)
{
    //waits for input from the CdS cell before progressing
    //*threshold value for cds may vary depending on ambient light and other factors
    float v;
    v=cds.Value();
    while (v>0.2)
    {
        v=cds.Value();
        LCD.WriteLine(v);
        Sleep(10);
    }
    Sleep(10);
}

//MOVING FUNCTIONS
void move_forward(int percent, float distance) //using encoders
{
    //moves forward at specified power for specified number of encoder counts
    encode = distance * counts_per_rotation;
    //Reset encoder counts
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();
    //Set both motors to desired percent
    right_motor.SetPercent(-percent);
    left_motor.SetPercent(percent);
    //While the average of the left and right encoder are less than counts,
    //keep running motors
    while((left_encoder.Counts() + right_encoder.Counts()) / 2. < encode);
    {
    }
    //Turn off motors
    right_motor.Stop();
    left_motor.Stop();
}//end move forward function

void turn_left(int percent, float distance) //using encoders
{
    //turns left for specified distance and percent power
    encode = distance * counts_per_rotation;
    //Reset encoder counts
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();
    //Set both motors to desired percent
    right_motor.SetPercent(-percent);
    left_motor.SetPercent(-percent);
    //While the average of the left and right encoder are less than counts,
    //keep running motors
    while ((left_encoder.Counts()+right_encoder.Counts())/2<encode)
    {
    }
    //Turn off motors
    right_motor.Stop();
    left_motor.Stop();
    }//end turn left function

void turn_right(int percent, int distance) //using encoders
{
    //powers the motors the specified percent and distance in a right turn
    encode = distance * counts_per_rotation;
    //Reset encoder counts
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();
    //Set both motors to desired percent
    right_motor.SetPercent(percent);
    left_motor.SetPercent(percent);
    //While the average of the left and right encoder are less than counts,
    //keep running motors
    while ((left_encoder.Counts()+right_encoder.Counts())/2<encode)
    {
    }
    //Turn off motors
    right_motor.Stop();
    left_motor.Stop();
}//end turn right function

//RPS FUNCTIONS
void check_coordinates(void)//continuously writes the RPS coordinates to the screen
{
    LCD.Clear( FEHLCD::Scarlet );
    LCD.SetBackgroundColor(FEHLCD::Scarlet);
    LCD.SetFontColor( FEHLCD::Gray );
    //Call this function to initialize the RPS to a course
    RPS.InitializeMenu();
    //Press middle button to begin
    LCD.WriteLine("Press the middle button to begin");
    //Wait for middle button to be pressed
    while(!buttons.MiddlePressed());
    //Wait for middle button to be unpressed
    while(buttons.MiddlePressed());
    LCD.Clear();
    //Write initial screen info
    LCD.WriteRC("X Position:",0,0);
    LCD.WriteRC("Y Position:",1,0);
    LCD.WriteRC(" Heading:",2,0);
    while( true )
    {
        LCD.WriteRC(RPS.X(),0,12); //update the x coordinate
        LCD.WriteRC(RPS.Y(),1,12); //update the y coordinate
        LCD.WriteRC(RPS.Heading(),2,12); //update the heading
        Sleep(10); //wait for a 10ms to avoid updating the screen too quickly
    }
}

void write_coordinates(void)
{
    //finds RPS coordinates and writes them to the screen
    float x, y, head;
    x=RPS.X();
    y=RPS.Y();
    head=RPS.Heading();
    LCD.WriteLine(x);
    LCD.WriteLine(y);
    LCD.WriteLine(head);
}

void check_x_plus(float x_coordinate)
{
    //aligns robot with desired RPS coordinate in the +x direction
    //check whether the robot is within an acceptable range
    while(RPS.X() < x_coordinate - 1 || RPS.X() > x_coordinate + 1)
    {
        Sleep(75);
        if(RPS.X() > x_coordinate)
        {
            //pulse the motors for a short duration in the correct direction
            left_motor.SetPercent(-15);
            right_motor.SetPercent(15);
            Sleep(75);
            left_motor.Stop();
            right_motor.Stop();
        }
        else if(RPS.X() < x_coordinate)
        {
            //pulse the motors for a short duration in the correct direction
            left_motor.SetPercent(15);
            right_motor.SetPercent(-15);
            Sleep(75);
            left_motor.Stop();
            right_motor.Stop();
        }
    }
}//end check x plus function

void check_x_minus(float x_coordinate)
{
    //aligns robot with desired RPS coordinate in -x direction
    while(RPS.X() < x_coordinate - 1 || RPS.X() > x_coordinate + 1)
    {
        Sleep(75);
        if(RPS.X() > x_coordinate)
        {
            //pulse the motors for a short duration in the correct direction
            left_motor.SetPercent(15);
            right_motor.SetPercent(-15);
            Sleep(75);
            left_motor.Stop();
            right_motor.Stop();
        }
        else if(RPS.X() < x_coordinate)
        {
            //pulse the motors for a short duration in the correct direction
            left_motor.SetPercent(-15);
            right_motor.SetPercent(15);
            Sleep(75);
            left_motor.Stop();
            right_motor.Stop();
        }
    }
}

void check_y_minus(float y_coordinate)
{
    //aligns robot with desired RPS coordinate in the -y direction
    y=RPS.Y();
    //check whether the robot is within an acceptable range
    while(y < y_coordinate - 1 || y > y_coordinate + 1)
    {
        Sleep(75);
        y=RPS.Y();
        if(y > y_coordinate)
        {
            //pulse the motors for a short duration in the correct direction
            left_motor.SetPercent(15);
            right_motor.SetPercent(-15);
            Sleep(75);
            left_motor.Stop();
            right_motor.Stop();
        }
        else if(y < y_coordinate)
        {
            //pulse the motors for a short duration in the correct direction
            left_motor.SetPercent(-15);
            right_motor.SetPercent(15);
            Sleep(75);
            left_motor.Stop();
            right_motor.Stop();
        }
    }
}//end check y minus

void check_y_plus(float y_coordinate)
{
    //aligns robot with desired RPS coordinate in the +y direction
    //check whether the robot is within an acceptable range
    while(RPS.Y() < y_coordinate - 1 || RPS.Y() > y_coordinate + 1)
    {
        Sleep(50);
        if(RPS.Y() > y_coordinate)
        {
            //pulse the motors for a short duration in the correct direction
            left_motor.SetPercent(-15);
            right_motor.SetPercent(15);
            Sleep(75);
            left_motor.Stop();
            right_motor.Stop();
        }
        else if(RPS.Y() < y_coordinate)
        {
            //pulse the motors for a short duration in the correct direction
            left_motor.SetPercent(15);
            right_motor.SetPercent(-15);
            Sleep(75);
            left_motor.Stop();
            right_motor.Stop();
        }
    }
}//end check y plus function

void check_heading(float heading)
{
    //compares RPS heading to desired heading, then turns robot until the two are within 2 degrees of each other
    float actual_heading;//find the actual heading
    actual_heading=RPS.Heading();
    //determine if heading is within acceptable range
    while ((actual_heading-heading)>2 || (actual_heading-heading)<-2 ||(actual_heading-heading)>358 || (actual_heading-heading)<-358)
    {
        actual_heading=RPS.Heading();
        Sleep(50);
        //compare desired heading to current heading
        if (actual_heading>heading)
        {
            //pulse motors for short duration in correct direction
            left_motor.SetPercent(15);
            right_motor.SetPercent(15);
            Sleep(25);
            left_motor.Stop();
            right_motor.Stop();
        }
        else if (actual_heading<heading)
        {
            left_motor.SetPercent(-15);
            right_motor.SetPercent(-15);
            Sleep(25);
            left_motor.Stop();
            right_motor.Stop();
        }
        if (heading>358 || heading <2)//special case for heading of 0 degrees so robot won't spin in a full circle
        {
            if ((actual_heading-heading)>0)
            {
                while ((actual_heading-heading)>358)
                {
                    actual_heading==RPS.Heading();
                    right_motor.SetPercent(15);
                    left_motor.SetPercent(15);
                    Sleep(15);
                    right_motor.Stop();
                    left_motor.Stop();
                    Sleep(15);
                }
            }
            else if ((actual_heading-heading)<0)
            {
                while ((actual_heading-heading)>-358)
                {
                    actual_heading==RPS.Heading();
                    right_motor.SetPercent(-15);
                    left_motor.SetPercent(-15);
                    Sleep(15);
                    right_motor.Stop();
                    left_motor.Stop();
                    Sleep(15);
                }
            }
        }
        Sleep(15);
    }//end while loop
}//end check heading function

void move_up_ramp(void)//drives to crank
{
    LCD.Clear();

//robot starts at (25.4, 10.599, heading 37.5)
    write_coordinates();
    //first it needs to turn itself back around
    turn_right(25,5);
    move_forward(25, 2);
    check_heading(0);//orient it in ramp direction

//move to bottom of ramp
    write_coordinates();
    Sleep(50);
    move_forward(25,5);//move to point D
    Sleep(50);
    check_y_plus(24.9);//make sure robot is at correct y coordinate

//robot should be at point D
    move_forward(25, 16);//full power to make it up the ramp
    write_coordinates();
    Sleep(50);

}//end of move_up_ramp

//PRESSING BUTTONS FUNCTIONS
void position_to_buttons(void) //function to find position needed for buttons
{
    check_y_plus(41.9);
    move_forward(25,4);//move forward from top of ramp
    write_coordinates();
    Sleep(50);
    turn_left(25,2.5);//turn on a 45 degree angle toward buttons
    Sleep(50);
    check_heading(34.199);//correct to avoid snow
    Sleep(50);
    move_forward(15,7);
    Sleep(50);
    check_heading(54.199);//correct to avoid crank
    Sleep(50);
    //buttons require precise positioning, so use RPS data
    check_y_plus(62.9);
    Sleep(50);
    check_x_minus(14.9);
    Sleep(50);
    write_coordinates();
    Sleep(50);
    //should now move to the front of the buttons
    move_forward(10,-3);//back up a little so you have room to move forward and press them
}

void press_button1(void)
{
    //determines which of the three buttons has the value of 1, sets the servo to the corresponding angle
    //and moves forward to press it, then backward to get ready to press next button
    if (r==1){
        servo.SetDegree(redangle);
        move_forward(25,1);
        move_forward(25,-1);
    }
    else if(w==1){
        servo.SetDegree(whiteangle);
        move_forward(25,1);
        move_forward(25,-1);
    }
    else if (b==1){
        servo.SetDegree(blueangle);
        move_forward(25,1);
        move_forward(25,-1);
    }
    else{
        LCD.WriteLine("ERROR1");
    }
    Sleep(100);
}//end press button1

void press_button2(void)
{
    //same as press_button1, but for second set
    if (r==2){
        servo.SetDegree(redangle);
        move_forward(25,1);
        move_forward(25,-1);
    }
    else if(w==2){
        servo.SetDegree(whiteangle);
        move_forward(25,1);
        move_forward(25,-1);
    }
    else if (b==2){
        servo.SetDegree(blueangle);
        move_forward(25,1);
        move_forward(25,-1);
    }
    else{
        LCD.WriteLine("ERROR2");
    }
    Sleep(100);
}//end press button2

void press_button3(void)
{
    //same as previous functions
    if (r==3){
        servo.SetDegree(redangle);
        move_forward(25,1);
        move_forward(25,-1);
    }
    else if(w==3){
        servo.SetDegree(whiteangle);
        move_forward(25,1);
        move_forward(25,-1);
    }
    else if (b==3){
        servo.SetDegree(blueangle);
        move_forward(25,1);
        move_forward(25,-1);
    }
    else{
        LCD.WriteLine("ERROR3");
    }
    Sleep(100);
}//end press button3

void press_buttons(void) //presses buttons in order dictated by RPS
{
    servo.SetMin(servomin);
    servo.SetMax(servomax);
    //read in data on the button order
    r=RPS.RedButtonOrder();
    w=RPS.WhiteButtonOrder();
    b=RPS.BlueButtonOrder();
    LCD.WriteLine(r);//display order for debugging purposes
    LCD.WriteLine(w);
    LCD.WriteLine(b);
    Sleep(5000);
    LCD.Clear();
    while (num_buttons<3)
    {
        if (num_buttons==0)//if none of the buttons have been pressed yet
        {
            press_button1();//function to determine and press first button
            num_buttons=RPS.ButtonsPressed();//takes new count to see if button has been pressed
            LCD.WriteLine(num_buttons);
            Sleep(1000);//sleeps extra long for debugging purposes(should be changed later)
        }
        else if (num_buttons==1)
        {
            press_button2();
            num_buttons=RPS.ButtonsPressed();
            LCD.WriteLine(num_buttons);
            Sleep(1000);
        }
        else if (num_buttons==2)
        {
            press_button3();
            num_buttons=RPS.ButtonsPressed();
            LCD.WriteLine(num_buttons);
            Sleep(1000);
        }
        else if (num_buttons==3)
        {
            LCD.WriteLine("YOU DID IT!!!");
        }
        LCD.Clear();
    }//end while loop
}//end press buttons function

//FUNCTIONS FOR SALT BAG CONTROL

void move_to_saltbag(void)
{
    //positions robot in ideal scooping position
    move_forward(25, -10);
    check_heading(44.8);
    move_forward(25,-6);
    check_x_minus(25.4);//since robot is moving backwards, use opposite RPS functions
    check_y_plus(15.8);
}

void scoop(void)
{
    //lowers servo to scooping position and drives forward until salt bag is within confines of scooper
    saltservo.SetDegree(scoopangle);
    //drive until microswitch is pressed
    move_forward(25,3);
    saltservo.SetDegree(saltholdingangle);//hold salt bag at high enough angle that it won't fall out
}//end scoop function

void move_to_garage(void)
{
    //once robot is at the buttons, it will find the garage
    move_forward(25,-3);
    turn_right(25,5);
    check_y_plus(51.9);//robot is moving backwards, so flip signs of each RPS function
    check_x_plus(11.7);
    check_heading(224.1);
}

void deposit_salt()
{
    //robot will lower the scoop and deposit the salt bag in the garage
    saltservo.SetDegree(scoopangle);
    move_forward(10,9);
    //robot will move backward really fast to let the salt bag slide out
    move_forward(50,-9);
    //check if salt bag is still in scoop
    move_forward(10,9);
    move_forward(50,-9);
}//end deposit_salt function

//MAIN
int main(void)
{
    RPS.InitializeMenu();
    move_to_saltbag();
    scoop();
    move_up_ramp();
    position_to_buttons();
    move_to_garage();
    deposit_salt();
}

