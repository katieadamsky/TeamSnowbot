/*Rotary Encoder and RPS Control of Motors
Written by Katie Adamsky 2/18/15
RJF 8:00 Team B6*/

#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include <FEHMotor.h>
#include <FEHRPS.h>

//Declarations for encoders & motors
ButtonBoard buttons(FEHIO::Bank3);
DigitalEncoder right_encoder(FEHIO::P0_0);
DigitalEncoder left_encoder(FEHIO::P0_1);
FEHMotor right_motor(FEHMotor::Motor0);
FEHMotor left_motor(FEHMotor::Motor1);

float x, y, head;

void check_x_plus(float x_coordinate) //using RPS while robot is in the +x direction
{
    //check whether the robot is within an acceptable range
    while(RPS.X() < x_coordinate - 1 || RPS.X() > x_coordinate + 1)
    {
        Sleep(500);
        if(RPS.X() > x_coordinate)
        {
            //pulse the motors for a short duration in the correct direction
            left_motor.SetPercent(-75);
            right_motor.SetPercent(-75);
            Sleep(100);
            left_motor.Stop();
            right_motor.Stop();

        }
        else if(RPS.X() < x_coordinate)
        {
            //pulse the motors for a short duration in the correct  direction
            left_motor.SetPercent(75);
            right_motor.SetPercent(75);
            Sleep(100);
            left_motor.Stop();
            right_motor.Stop();
        }
    }
}//end check x plus function

void check_y_minus(float y_coordinate) //using RPS while robot is in the -y direction
{
    y=RPS.Y();
    //check whether the robot is within an acceptable range
    while(y < y_coordinate - 1 || y > y_coordinate + 1)
    {
        Sleep(500);
        y=RPS.Y();
        if(y > y_coordinate)
        {
            //pulse the motors for a short duration in the correct direction
            left_motor.SetPercent(75);
            right_motor.SetPercent(75);
            Sleep(100);
            left_motor.Stop();
            right_motor.Stop();
        }
        else if(y < y_coordinate)
        {
            //pulse the motors for a short duration in the correct direction
            left_motor.SetPercent(-75);
            right_motor.SetPercent(-75);
            Sleep(100);
            left_motor.Stop();
            right_motor.Stop();
        }
    }
}//end check y minus

void check_y_plus(float y_coordinate) //using RPS while robot is in the +y direction
{
    //check whether the robot is within an acceptable range
    while(RPS.Y() < y_coordinate - 1 || RPS.Y() > y_coordinate + 1)
    {
        Sleep(500);
        if(RPS.Y() > y_coordinate)
        {
            //pulse the motors for a short duration in the correct direction
            left_motor.SetPercent(-75);
            right_motor.SetPercent(-75);
            Sleep(100);
            left_motor.Stop();
            right_motor.Stop();
        }
        else if(RPS.Y() < y_coordinate)
        {
            //pulse the motors for a short duration in the correct direction
            left_motor.SetPercent(75);
            right_motor.SetPercent(75);
            Sleep(100);
            left_motor.Stop();
            right_motor.Stop();
        }
    }
}//end check y plus function

void check_heading(float heading) //using RPS
{
    float actual_heading;//find the actual heading
    actual_heading=RPS.Heading();
    while ((actual_heading-heading)>2 || (actual_heading-heading)<-2 ||(actual_heading-heading)>358 || (actual_heading-heading)<-358)
    {
    actual_heading=RPS.Heading();
    Sleep(100);
    if (actual_heading>heading)//compare desired heading to current heading
    {
        left_motor.SetPercent(75);
        right_motor.SetPercent(-75);
        Sleep(100);
        left_motor.Stop();
        right_motor.Stop();
    }
    else if (actual_heading<heading)
    {
        left_motor.SetPercent(-75);
        right_motor.SetPercent(75);
        Sleep(100);
        left_motor.Stop();
        right_motor.Stop();
    }
    if (heading>358 || heading <2)
    {
        if ((actual_heading-heading)>300)
        {
            while ((actual_heading-heading)>358)
            {
            right_motor.SetPercent(75);
            left_motor.SetPercent(-75);
            Sleep(100);
            right_motor.Stop();
            left_motor.Stop();
            }
        }
        else if ((actual_heading-heading)<-300)
        {
            while ((actual_heading-heading)>-358)
            {
            right_motor.SetPercent(-75);
            left_motor.SetPercent(75);
            Sleep(100);
            right_motor.Stop();
            left_motor.Stop();
            }
        }

    }
    Sleep(50);
    }//end while loop
}//end check heading function

void move_forward(int percent, int counts) //using encoders
{
    //Reset encoder counts
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();

    //Set both motors to desired percent
    right_motor.SetPercent(percent);
    left_motor.SetPercent(percent);

    //While the average of the left and right encoder are less than counts,
    //keep running motors
    while((left_encoder.Counts() + right_encoder.Counts()) / 2. < counts);
    {
    }
    //Turn off motors
    right_motor.Stop();
    left_motor.Stop();
}//end move forward function

void turn_left(int percent, int counts) //using encoders
{
//Reset encoder counts
right_encoder.ResetCounts();
left_encoder.ResetCounts();
//Set both motors to desired percent
right_motor.SetPercent(percent);
left_motor.SetPercent(-percent);
//While the average of the left and right encoder are less than counts,
//keep running motors
        while ((left_encoder.Counts()+right_encoder.Counts())/2<counts)
        {

        }
//Turn off motors
right_motor.Stop();
left_motor.Stop();
}//end turn left function

void write_coordinates(void)
{
    float x, y, head;
    x=RPS.X();
    y=RPS.Y();
    head=RPS.Heading();
    LCD.WriteLine(x);
    LCD.WriteLine(y);
    LCD.WriteLine(head);
}

int main(void)
{
    RPS.InitializeMenu();
    LCD.WriteLine("Press the middle button to start");
while (!buttons.MiddlePressed())
{
    //wait for middle button to be pressed
}
Sleep(500);

//robot starts at pt A
write_coordinates();
move_forward(75, 297);//drive 14 inches down
Sleep(1500);
write_coordinates();
check_y_minus(16);//ensure that robot went far enough in y direction
Sleep(1500);
check_heading(180.0);

//robot should be at point B
write_coordinates();//check that it is
Sleep(1500);
turn_left(75, 95);
write_coordinates();
Sleep(1500);
check_heading(270.0);//make sure robot turned a full 90 degrees
write_coordinates();
Sleep(1500);
move_forward(75, 211);
write_coordinates();
Sleep(1500);
check_x_plus(28.800);
Sleep(1500);

//should be at point C
write_coordinates();//ensure robot is at point C
Sleep(1500);
turn_left(75,95);
Sleep(1500);
write_coordinates();
check_heading(359.9);//turn robot to face ramp
Sleep(1500);
write_coordinates();
move_forward(75,85);//move to point D
Sleep(500);
check_y_plus(23);//make sure robot is at correct y coordinate

//robot should be at point D
write_coordinates();//check coordinates for the final time
Sleep(10000);
}//end of int main
