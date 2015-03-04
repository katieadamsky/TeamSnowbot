/*Performance Test 3: Pressing the Buttons
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
FEHServo servo(FEHServo::Servo0);

#define counts_per_rotation 33.7408
//define different levels of motor power for easily modifying later
#define full_power 100
#define normal_power 25
#define half_power 50

//COME BACK AND ADJUST THESE VALUES TO ACTUAL CALIBRATION
#define servomin 545
#define servomax 2367
#define redangle 45
#define whiteangle 90
#define blueangle 180

float encode;//for rotary encoding functions
float x, y, head;//for RPS functions
int r, w, b, num_buttons=0, buttoncase;//for button pressing function

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

void check_x_minus(float x_coordinate){
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

void move_up_ramp(void)//starts at light and drives to crank
{

LCD.WriteLine("Waiting for the light");
//wait for start light
start_at_light();
LCD.Clear();

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
}//end of move_up_ramp

//Pressing buttons functions
void position_to_buttons(void) //function to find position needed for buttons
{
    check_y_plus(41.9);
    write_coordinates();
    Sleep(100);
    turn_right(25,2.5);
    Sleep(50);
    check_heading(45);
     Sleep(50);
    check_y_plus(48.699);
     Sleep(50);
    check_x_minus(30.0);
     Sleep(50);
     write_coordinates();
     Sleep(1000);
    //should now move to the front of the buttons
     move_forward(25,15.11);
      Sleep(50);
      check_heading(43.8);
      write_coordinates();
      Sleep(1000);
}

void press_button1(void)
{
    if (r==1){
        servo.SetDegree(redangle);
        move_forward(10,1);
    }
    else if(w==1){
        servo.SetDegree(whiteangle);
        move_forward(10,1);
    }
    else if (b==1){
        servo.SetDegree(blueangle);
        move_forward(10,1);
    }
    else{
        LCD.WriteLine("ERROR1");
    }
    Sleep(100);
}

void press_button2(void)
{
    if (r==2){
        servo.SetDegree(redangle);
        move_forward(10,1);
    }
    else if(w==2){
        servo.SetDegree(whiteangle);
        move_forward(10,1);
    }
    else if (b==2){
        servo.SetDegree(blueangle);
        move_forward(10,1);
    }
    else{
        LCD.WriteLine("ERROR2");
    }
    Sleep(100);
}

void press_button3(void)
{
    if (r==3){
        servo.SetDegree(redangle);
        move_forward(10,1);
    }
    else if(w==3){
        servo.SetDegree(whiteangle);
        move_forward(10,1);
    }
    else if (b==3){
        servo.SetDegree(blueangle);
        move_forward(10,1);
    }
    else{
        LCD.WriteLine("ERROR3");
    }
    Sleep(100);
}

void press_buttons(void) //presses buttons, hopefully in correct order
{
    servo.SetMin(servomin);
    servo.SetMax(servomax);
    while(num_buttons<3)
    {
        position_to_buttons();
        r=RPS.RedButtonOrder();
        w=RPS.WhiteButtonOrder();
        b=RPS.BlueButtonOrder();
        press_button1();
        press_button2();
        press_button3();
        num_buttons=RPS.ButtonsPressed();
        move_forward(10,-1);
    }
}


int main(void)
{
    servo.SetMin(545);
    servo.SetMax(2367);
    servo.SetDegree(0);
    Sleep(2500);
    
    servo.SetDegree(45);
    Sleep(2500);
    
    servo.SetDegree(90);
    Sleep(2500);

    servo.SetDegree(135);
    Sleep(2500);
  
    servo.SetDegree(180);
    Sleep(2500);
}
