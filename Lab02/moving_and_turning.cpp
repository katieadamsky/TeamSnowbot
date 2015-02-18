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

void turn_right(int percent, int counts) //using encoders
{
//Reset encoder counts
right_encoder.ResetCounts();
left_encoder.ResetCounts();
//Set both motors to desired percent
right_motor.SetPercent(-percent);
left_motor.SetPercent(percent);
//While the average of the left and right encoder are less than counts,
//keep running motors

        while ((left_encoder.Counts()+right_encoder.Counts())/2<counts)
        {

        }

//Turn off motors
right_motor.Stop();
left_motor.Stop();
}

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


int main(){

    move_forward(75,297);
    Sleep(500);
    turn_left(75,95);
    Sleep(500);
    move_forward(75,210.221);
    Sleep(500);
    turn_left(75,95);
    Sleep(500);
    move_forward(75,84.884);


}
