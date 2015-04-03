/*Individual Competition
Written by Katie Adamsky
RJF 8:00 Team B6*/

#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include <FEHMotor.h>
#include <FEHRPS.h>
#include <FEHServo.h>
#include <FEHBuzzer.h>
#include <FEHBattery.h>

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

//values for calibrating RPS
//COME BACK AND DETERMINE THESE EXPERIMENTALLY
#define startx 17.6
#define starty 29.4

//values for button-pressing functions
#define servomin 523
#define servomax 2380
#define redangle 0
#define whiteangle 60
#define blueangle 38

//values for salt bag functions
#define scoopangle 155
#define startangle 60
#define saltholdingangle 100
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
    while (v>0.9)
    {
        v=cds.Value();
        LCD.WriteLine(v);
        Sleep(10);
        LCD.Clear();
    }
    Sleep(10);
}




/************MOVING FUNCTIONS***********/

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

void turn_left_backwards(int percent, float distance)
{
    encode = distance * counts_per_rotation;
    //Reset encoder counts
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();
    //set both motors to desired percent
    right_motor.SetPercent(percent);
    left_motor.SetPercent(percent);
    while ((left_encoder.Counts()+right_encoder.Counts())/2<encode)
    {
    }
    //Turn off motors
    right_motor.Stop();
    left_motor.Stop();
}

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

void turn_right_backwards(int percent, float distance)
{
    //powers the motors the specified percent and distance in a right turn
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
}//end turn right function




/************RPS FUNCTIONS*********/

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
    while(RPS.X() < x_coordinate+x - 1 || RPS.X() > x_coordinate+x + 1)
    {
        Sleep(75);
        if(RPS.X() > x_coordinate+x)
        {
            //pulse the motors for a short duration in the correct direction
            left_motor.SetPercent(-15);
            right_motor.SetPercent(15);
            Sleep(75);
            left_motor.Stop();
            right_motor.Stop();
        }
        else if(RPS.X() < x_coordinate+x)
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
    while(RPS.X() < x_coordinate+x - 1 || RPS.X() > x_coordinate+x + 1)
    {
        Sleep(75);
        if(RPS.X() > x_coordinate+x)
        {
            //pulse the motors for a short duration in the correct direction
            left_motor.SetPercent(15);
            right_motor.SetPercent(-15);
            Sleep(75);
            left_motor.Stop();
            right_motor.Stop();
        }
        else if(RPS.X() < x_coordinate+x)
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
    RPS.Y();
    //check whether the robot is within an acceptable range
    while(RPS.Y() < y_coordinate+y - 1 || RPS.Y() > y_coordinate+y + 1)
    {
        Sleep(75);
        if(RPS.Y() > y_coordinate+y)
        {
            //pulse the motors for a short duration in the correct direction
            left_motor.SetPercent(15);
            right_motor.SetPercent(-15);
            Sleep(75);
            left_motor.Stop();
            right_motor.Stop();
        }
        else if(RPS.Y() < y_coordinate+y)
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
    while(RPS.Y() < y_coordinate+y - 1 || RPS.Y() > y_coordinate+y + 1)
    {
        Sleep(50);
        if(RPS.Y() > y_coordinate+y)
        {
            //pulse the motors for a short duration in the correct direction
            left_motor.SetPercent(-15);
            right_motor.SetPercent(15);
            Sleep(75);
            left_motor.Stop();
            right_motor.Stop();
        }
        else if(RPS.Y() < y_coordinate+y)
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
    //compare desired heading to current heading
    while (RPS.Heading()==-1)
    {
        //wait for RPS system to return
        LCD.WriteLine("RPS lost! :(");
        LCD.WriteLine(RPS.Heading());
    }
        if (heading != 0)
        {
           while ((actual_heading-heading)>2 || (actual_heading-heading)<-2)
           {
               actual_heading=RPS.Heading();
               LCD.WriteLine(actual_heading);
               Sleep(50);
            if (actual_heading>heading)
            {
                //pulse motors for short duration in correct direction
                left_motor.SetPercent(15);
                right_motor.SetPercent(15);
                Sleep(75);
                left_motor.Stop();
                right_motor.Stop();
            }
            else if (actual_heading<heading)
            {
                left_motor.SetPercent(-15);
                right_motor.SetPercent(-15);
                Sleep(75);
                left_motor.Stop();
                right_motor.Stop();
            }
           }
        }
        else if (heading==0)//special case for heading of 0 degrees so robot won't spin in a full circle
        {
        while ((actual_heading-heading)>2 && (actual_heading-heading)<358)
        {
            actual_heading=RPS.Heading();
            LCD.WriteLine(actual_heading);
            Sleep(50);
            if ((actual_heading-heading)<358 && actual_heading>180)
            {
                actual_heading==RPS.Heading();
                right_motor.SetPercent(-15);
                left_motor.SetPercent(-15);
                Sleep(75);
                right_motor.Stop();
                left_motor.Stop();
                Sleep(15);
            }
            else if ((actual_heading-heading)<-358)
            {
                actual_heading==RPS.Heading();
                right_motor.SetPercent(-15);
                left_motor.SetPercent(-15);
                Sleep(75);
                right_motor.Stop();
                left_motor.Stop();
                Sleep(15);
            }
            else if ((actual_heading-heading)>2 && actual_heading<180)
            {
                actual_heading==RPS.Heading();
                right_motor.SetPercent(15);
                left_motor.SetPercent(15);
                Sleep(75);
                right_motor.Stop();
                left_motor.Stop();
                Sleep(15);
            }
        }
        }
        Sleep(15);

}//end check heading function

float calibrate_x(void)
{
    //compares RPS readings at the start point to the ideal RPS readings, and finds the difference
    float x=0;
    x=RPS.X()-startx;
    LCD.WriteLine("In the x direction, the RPS is off by ");
    LCD.Write(x);
    return x;
}

float calibrate_y(void)
{
    float y=0;
    y=RPS.Y()-starty;
    LCD.WriteLine("In the y direction, the RPS is off by ");
    LCD.Write(y);
    return y;
}



/********PRESSING BUTTONS FUNCTIONS********/

void position_to_buttons(void) //function to find position needed for buttons
{
    check_y_minus(41.9);
    move_forward(-25,4);//move forward from top of ramp
    write_coordinates();
    Sleep(50);
    turn_right(25,7.5);//turn on a 45 degree angle toward buttons
    Sleep(50);
    check_heading(34.199);//correct to avoid snow
    Sleep(50);
    move_forward(15,7);
    Sleep(50);
    check_heading(50.199);//correct to avoid crank
    Sleep(50);
    //buttons require precise positioning, so use RPS data
    check_y_plus(62.9);
    Sleep(50);
    check_x_minus(14.9);
    Sleep(50);
    write_coordinates();
    Sleep(50);
    //should now move to the front of the buttons
    move_forward(-10,3);//back up a little so you have room to move forward and press them
}

void position_to_buttons_final(void)
{
    //begins with the robot at the crank rather than the top of the ramp
    move_forward(-25,4); //move away from crank
    check_y_plus(49.199);
    turn_left(25, 2.5); //turn on a 45 degree angle toward buttons
    check_heading(40.199);//correct to avoid snow
    move_forward(25,9);
    check_heading(56.199);//correct to avoid fence
    move_forward(25,9);
    //from here, use RPS to precisely position to the buttons
    check_heading(46.3);
    move_forward(-10,1);//back up a little for optimum button pressing distance
}

void ram(void)
{
    float starttime=0, timer=0;
    starttime=TimeNow();
    //in case no mechanism is developed to press the buttons in the correct order
    while (RPS.ButtonsPressed()==0)
    {
        right_motor.SetPercent(-60);
        left_motor.SetPercent(60);
        timer=TimeNow();
        if ((timer-starttime)>10)
        {
            break;
        }
    }
    right_motor.Stop();
    left_motor.Stop();
    move_forward(-40, 0.5);
    Sleep(100);

}




/*******FUNCTIONS FOR SALT BAG CONTROL********/

void move_to_saltbag(void)
{
    //positions robot in ideal scooping position
    move_forward(-25, 22);
    check_y_plus(7.0);
    turn_right(-25, 5);
    check_x_minus(17.699);
    check_heading(90.0);
    move_forward(-25,2.5);
}

void scoop(void)
{
    //lowers servo to scooping position and drives forward until salt bag is within confines of scooper
    saltservo.SetDegree(scoopangle);
    Sleep(1000);
    move_forward(-45,15);
    saltservo.SetDegree(saltholdingangle);//hold salt bag at high enough angle that it won't fall out
    Sleep(1000);
    move_forward(40, 5);
}

void move_to_garage(void)
{
    //once robot is at the buttons, it will find the garage
    move_forward(-25,3);
    turn_right(25,5);
    //check_heading(
    move_forward(-30, 3);
    check_y_plus(52.9);//robot is moving backwards, so flip signs of each RPS function
    check_x_plus(11);
    move_forward(-40, 0.5);
    turn_left(-30, 5);
    check_heading(224.1);
}

void deposit_salt(void)
{
    saltservo.SetDegree(scoopangle);
    move_forward(-25, 5);
    saltservo.SetDegree(startangle);
        Sleep(500);
    right_motor.SetPercent(-50);
    left_motor.SetPercent(50);
    saltservo.SetDegree(scoopangle);
    Sleep(550);
    right_motor.Stop();
    left_motor.Stop();
    saltservo.SetDegree(startangle);
}

void poke(void)
{
    //after the salt bag has been deposited, nudges the bag into the garage a little further
    saltservo.SetDegree(scoopangle-10);
    check_heading(220.1);
    move_forward(-50, 9);
    move_forward(50, 3);
    saltservo.SetDegree(startangle);
}

void move_up_ramp(void)
{
    LCD.Clear();
    write_coordinates();

    move_forward(10,1);
    turn_left(30, 4);
    move_forward(-25,5);
    check_x_minus(30.4);
    check_heading(180);

//move to bottom of ramp
    write_coordinates();
    Sleep(50);
    move_forward(-25,5);//move to point D
    Sleep(50);
    check_y_minus(24.9);//make sure robot is at correct y coordinate

//robot should be at point D
    check_heading(0.00);
    move_forward(-20, 20);//full power to make it up the ramp
    write_coordinates();
    Sleep(50);

}//end of move_up_ramp

void move_up_ramp_forwards(void)
{
    LCD.Clear();
    write_coordinates();

    move_forward(10,1);
    turn_right(30, 8);
    move_forward(30,6);
    check_x_plus(29.5);
    check_heading(0.00);

//move to bottom of ramp
    write_coordinates();
    Sleep(50);
    move_forward(30, 5);//move to point D
    Sleep(50);
    check_y_plus(24.9);//make sure robot is at correct y coordinate

//robot should be at point D
    check_heading(0.00);
    move_forward(40, 20);//full power to make it up the ramp
    write_coordinates();
}



/******CRANK FUNCTIONS*********/

void position_to_crank(void)
{
    //move away from ramp
    turn_left(30, 0.5);
    move_forward(30, 7);
    move_forward(-30, 1);
    turn_left(30, 2);
    check_heading(25);
    check_x_minus(29.0);
    check_heading(0.00);
    check_y_plus(54.6);
}//end position_to_crank


int checkcds(void)
{
    LCD.Clear();
    while (!buttons.MiddlePressed())
    {
        float v;
        v=cds.Value();
        Sleep(100);
        LCD.WriteLine(v);
        Sleep(1000);
    }
}

void crank(void)
{
    float v;
    int direction=0;
     v=cds.Value();
     int find=0;
     Sleep(100);
         while (direction==0)
         {
             v=cds.Value();
             if (v>0.9)
             {
                 direction=1;
                 LCD.WriteLine(v);
                 LCD.WriteLine(direction);
             }
             else if (v<=0.9)
             {
                 direction=2;
                 LCD.WriteLine(v);
                 LCD.WriteLine(direction);
             }
             Sleep(100);
         }
    int turned=0;
    int degree=0;
    int t1=0, t2=0;
    int degree2=180;
    move_forward(-10, 1);
    while (turned<5)
    {
    if (direction==1)
    {
        //the light color is blue
        //turn motor counterclockwise
        LCD.WriteLine("Turn counterclockwise");
        servo.SetDegree(degree2);

        Sleep(1000);
        right_motor.SetPercent(-15);
        left_motor.SetPercent(15);

        servo.SetDegree(degree);
                Sleep(1000);

        move_forward(-10,1);
        Sleep(1000);
        turned=turned+1;
    }
    else if (direction==2)
    {
        //light color is red
        //turn motor clockwise
        LCD.WriteLine("Turn clockwise");
        servo.SetDegree(degree);
        Sleep(1000);
        right_motor.SetPercent(-15);
        left_motor.SetPercent(15);
        servo.SetDegree(degree2);
        Sleep(1000);
        move_forward(-10,1);
        Sleep(1000);
        turned=turned+1;

    }

    }
}//end crank function



/************OIL SWITCH FUNCTIONS****************/

void reach_ramp_top(void)
{
    //starting from garage, go back to where the buttons were to avoid snow pile and trees

    turn_left(30, 5);
    check_heading(315);
    move_forward(30, 11);
    turn_right(30, 4);
    check_heading(245.199); //previously 240.199
    move_forward(30, 17);
    check_x_plus(30.4);
    check_heading(187);
}

void move_down_ramp(void)
{
    move_forward(40, 4);
    write_coordinates();
    //robot should be at top of ramp
    if (RPS.Y()>=40)
    {
        check_y_minus(40.199);
        write_coordinates();
        check_heading(185);
        move_forward(15,10);//drives slowly down ramp
    }
    check_y_minus(20.5);
    //position to oil switch
    turn_right(40, 4);
    check_heading(90);
    move_forward(40, 17);
    check_x_minus(13);
    turn_right(30, 5);
    check_heading(0.00);
    move_forward(-40, 8.5);
    turn_left(25, 1);
    check_heading(0);
    check_y_plus(19.0);

}

void toggle_switch(void)
{
    //if oildirec=0, toggle switch to the right

        saltservo.SetDegree(115);
        move_forward(-30, 5);
        move_forward(30,4);
        Sleep(50);
        move_forward(-30, 5);
        move_forward(30,3);
        Sleep(50);
        move_forward(-30, 5);
        move_forward(30,3);
        Sleep(50);
    //if oildirec=1, toggle switch to left
        //that doesn't matter because it's not for competition points
}


/******MAIN*****/

int main(void)
{
    //final order for competition
    servo.SetDegree(0);
    saltservo.SetDegree(startangle);
    RPS.InitializeMenu();
    calibrate_x();
    calibrate_y();
    start_at_light();
    move_to_saltbag();
    scoop();
    move_up_ramp_forwards();
    position_to_crank();
    crank();
    position_to_buttons_final();
    ram();
    move_to_garage();
    deposit_salt();
    poke();
    reach_ramp_top();
    move_down_ramp();
    toggle_switch();
}

