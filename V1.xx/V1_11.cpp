/*Performance Test 2: Driving to the Upper Level
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
DigitalInputPin cds(FEHIO::P0_0); //CdS cell

#define counts_per_rotation 33.7408
//define different levels of motor power for easily modifying later
#define full_power 100
#define normal_power 75
#define half_power 50

float encode;

void move_forward(int percent, int distance) //using encoders
{
    encode = distance * counts_per_rotation;
    //Reset encoder counts
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();

    //Set both motors to desired percent
    right_motor.SetPercent(percent);
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

void turn_left(int percent, int distance) //using encoders
{
    encode = distance * counts_per_rotation;
//Reset encoder counts
right_encoder.ResetCounts();
left_encoder.ResetCounts();
//Set both motors to desired percent
right_motor.SetPercent(percent);
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

void start_at_light(void)
{
    float v;
    v=cds.Value();
    while (v>0.4)
    {
        v=cds.Value();
        Sleep(10);
    }
    Sleep(10);
}

int main(void)
{

LCD.WriteLine("Wait for the light");
//wait for start light
start_at_light();
LCD.Clear();
//robot starts at pt A
move_forward(normal_power, 14);//drive 14 inches down
Sleep(1000);

//robot should be at point B
turn_left(normal_power, 4);//FIND OUT EXPERIMENTALLY HOW MANY COUNTS TURNS IT RIGHT AND LEFT 4 is not correct
Sleep(1000);
move_forward(normal_power, 10);//drive ten inches in the +x direction
Sleep(1000);

//should be at point C
turn_left(normal_power,4);
Sleep(1000);
move_forward(normal_power,4);//move to point D
Sleep(1000);

//robot should be at point D

move_forward(full_power, 8);//full power to make it up the ramp
Sleep(100);

//Try to touch the crank
move_forward(normal_power, 10);
Sleep(100);


}//end of int main
