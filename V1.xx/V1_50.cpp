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
DigitalEncoder left_encoder(FEHIO::P0_2);
FEHMotor right_motor(FEHMotor::Motor0);
FEHMotor left_motor(FEHMotor::Motor1);
AnalogInputPin cds(FEHIO::P0_4); //CdS cell

#define counts_per_rotation 33.7408
//define different levels of motor power for easily modifying later
#define full_power 100
#define normal_power 75
#define half_power 50
float encode;
float x, y, head;
void start_at_light(void)
{
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
void check_x_plus(float x_coordinate) //using RPS while robot is in the +x direction
{
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
void check_y_minus(float y_coordinate) //using RPS while robot is in the -y direction
{
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
void check_y_plus(float y_coordinate) //using RPS while robot is in the +y direction
{
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

void check_heading(float heading) //using RPS
{
float actual_heading;//find the actual heading
actual_heading=RPS.Heading();
while ((actual_heading-heading)>2 || (actual_heading-heading)<-2 ||(actual_heading-heading)>358 || (actual_heading-heading)<-358)
{
actual_heading=RPS.Heading();
Sleep(50);
if (actual_heading>heading)//compare desired heading to current heading
{
left_motor.SetPercent(15);
right_motor.SetPercent(15);
Sleep(25);
}
else if (actual_heading<heading)
{
left_motor.SetPercent(-15);
right_motor.SetPercent(-15);
Sleep(25);
}
if (heading>358 || heading <2)
{
if ((actual_heading-heading)>300)
{
while ((actual_heading-heading)>358)
{
right_motor.SetPercent(15);
left_motor.SetPercent(15);
Sleep(25);
}
}
else if ((actual_heading-heading)<-300)
{
while ((actual_heading-heading)>-358)
{
right_motor.SetPercent(-15);
left_motor.SetPercent(-15);
Sleep(25);
}
}
}
left_motor.Stop();
right_motor.Stop();
Sleep(75);
}//end while loop
}//end check heading function
//MAIN FUNCTION
int main(void)
{
    RPS.InitializeMenu();

/*LCD.WriteLine("Waiting for the light");
//wait for start light
start_at_light();
LCD.Clear();*/

//robot starts at pt A: (18.4, 31.3) 180 deg
write_coordinates();
move_forward(25, 14);//drive 14 inches down
Sleep(100);
write_coordinates();
check_y_minus(18.4);//ensure that robot went far enough in y direction
Sleep(100);
check_heading(180.0);


//robot should be at point B: (18.4, 18.4) 180 deg
write_coordinates();//check that it is
Sleep(100);
turn_left(25, 5);
write_coordinates();
Sleep(100);
check_heading(270.0);//make sure robot turned a full 90 degrees
write_coordinates();
Sleep(100);
check_x_plus(18.4);//correct for forward motion in the turn
Sleep(100);
move_forward(25, 10);//drive twelve inches in the +x direction
write_coordinates();
Sleep(100);
check_x_plus(30.4);
write_coordinates();
Sleep(100);

//should be at point C
write_coordinates();//ensure robot is at point C
Sleep(100);
turn_left(25,5);
Sleep(100);
write_coordinates();
check_heading(359.0);//turn robot to face ramp
Sleep(100);
check_y_plus(18.5);
write_coordinates();
Sleep(100);
move_forward(25,5);//move to point D
Sleep(100);
check_y_plus(24.9);//make sure robot is at correct y coordinate

//robot should be at point D
write_coordinates();//check coordinates
Sleep(100);
move_forward(25, 20);//full power to make it up the ramp
write_coordinates();
Sleep(100);
check_y_plus(40.199);
write_coordinates();
Sleep(1000);

//Try to touch the crank
check_heading(359.0);
write_coordinates();
Sleep(1000);
move_forward(25, 15.25);
write_coordinates();
Sleep(100);
check_y_plus(56.199);
}//end of int main
