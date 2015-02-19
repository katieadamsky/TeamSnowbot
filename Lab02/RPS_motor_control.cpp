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



void check_x_plus(float x_coordinate) //using RPS while robot is in the +x direction
{
    //check whether the robot is within an acceptable range
    while(RPS.X() < x_coordinate - 1 || RPS.X() > x_coordinate + 1)
    {
        if(RPS.X() > x_coordinate)
        {
            //pulse the motors for a short duration in the correct direction
            left_motor.SetPercent(75);
            right_motor.SetPercent(-75);
            Sleep(50);
            left_motor.SetPercent(0);
            right_motor.SetPercent(0);
            
        }
        else if(RPS.X() < x_coordinate)
        {
            //pulse the motors for a short duration in the correct  direction
            left_motor.SetPercent(-75);
            right_motor.SetPercent(75);
            Sleep(50);
            left_motor.SetPercent(0);
            right_motor.SetPercent(0);
        }
    }
}

void check_y_minus(float y_coordinate) //using RPS while robot is in the -y direction
{
    //check whether the robot is within an acceptable range
    while(RPS.Y() < y_coordinate - 1 || RPS.Y() > y_coordinate + 1)
    {
        if(RPS.Y() > y_coordinate)
        {
            //pulse the motors for a short duration in the correct direction
            left_motor.SetPercent(75);
            right_motor.SetPercent(75);
            Sleep(50);
            left_motor.SetPercent(0);
            right_motor.SetPercent(0);
        }
        else if(RPS.Y() < y_coordinate)
        {
            //pulse the motors for a short duration in the correct direction
            left_motor.SetPercent(-75);
            right_motor.SetPercent(-75);
            Sleep(50);
            left_motor.SetPercent(0);
            right_motor.SetPercent(0);
        }
    }
}

void check_y_plus(float y_coordinate) //using RPS while robot is in the +y direction
{
    //check whether the robot is within an acceptable range
    while(RPS.Y() < y_coordinate - 1 || RPS.Y() > y_coordinate + 1)
    {
        if(RPS.Y() > y_coordinate)
        {
            //pulse the motors for a short duration in the correct direction
            left_motor.SetPercent(-75);
            right_motor.SetPercent(-75);
            Sleep(50);
            left_motor.SetPercent(0);
            right_motor.SetPercent(0);
        }
        else if(RPS.Y() < y_coordinate)
        {
            //pulse the motors for a short duration in the correct direction
            left_motor.SetPercent(75);
            right_motor.SetPercent(75);
            Sleep(50);
            left_motor.SetPercent(0);
            right_motor.SetPercent(0);
        }
    }
}

void check_heading(float heading) //using RPS
{
    //you will need to fill out this one yourself and take into account
    //the edge conditions (when you want the robot to go to 0 degrees
    //or close to 0 degrees)
    
}

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

    //Turn off motors
    right_motor.Stop();
    left_motor.Stop();
}

int main(void)
{
//drive 14 inches down
move_forward(75, 297.094);
//change to desired coordinate (18.99,31.000)
check_x_plus(18.099);
check_y_minus(31.000);
}

