/*Performance Test 5: Turning the Crank
Written by Katie Adamsky
RJF 8:00 Team B6*/

#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include <FEHMotor.h>
#include <FEHRPS.h>
#include <FEHServo.h>
#include <FEHBuzzer.h>

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
    //compare desired heading to current heading
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
                left_motor.SetPercent(30);
                right_motor.SetPercent(30);
                Sleep(20);
                left_motor.Stop();
                right_motor.Stop();
            }
            else if (actual_heading<heading)
            {
                left_motor.SetPercent(-30);
                right_motor.SetPercent(-30);
                Sleep(20);
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
                right_motor.SetPercent(-30);
                left_motor.SetPercent(-30);
                Sleep(25);
                right_motor.Stop();
                left_motor.Stop();
                Sleep(25);
            }
            else if ((actual_heading-heading)<-358)
            {
                actual_heading==RPS.Heading();
                right_motor.SetPercent(-30);
                left_motor.SetPercent(-30);
                Sleep(25);
                right_motor.Stop();
                left_motor.Stop();
                Sleep(25);
            }
            else if ((actual_heading-heading)>2 && actual_heading<180)
            {
                actual_heading==RPS.Heading();
                right_motor.SetPercent(30);
                left_motor.SetPercent(30);
                Sleep(25);
                right_motor.Stop();
                left_motor.Stop();
                Sleep(25);
            }
        }
        }
        Sleep(15);

}//end check heading function




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
    move_forward(-25,6); //move away from crank
    check_y_plus(44.9); //needs to be determined experimentally, just an approximation
    turn_left(-25, 7.5); //turn on a 45 degree angle toward buttons
    check_heading(34.199);//correct to avoid snow
    move_forward(25,7);
    check_heading(50.199);//correct to avoid crank
    move_forward(25,2);
    //from here, use RPS to precisely position to the buttons
    check_y_plus(62.9);
    check_x_minus(14.9);
    move_forward(-10,3);//back up a little for optimum button pressing distance

}

void press_button1(void)
{
    //determines which of the three buttons has the value of 1, sets the servo to the corresponding angle
    //and moves forward to press it, then backward to get ready to press next button
    if (r==1){
        servo.SetDegree(redangle);
        move_forward(25,1);
        move_forward(-25,1);
    }
    else if(w==1){
        servo.SetDegree(whiteangle);
        move_forward(25,1);
        move_forward(-25,1);
    }
    else if (b==1){
        servo.SetDegree(blueangle);
        move_forward(25,1);
        move_forward(-25,1);
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
        move_forward(-25,1);
    }
    else if(w==2){
        servo.SetDegree(whiteangle);
        move_forward(25,1);
        move_forward(-25,1);
    }
    else if (b==2){
        servo.SetDegree(blueangle);
        move_forward(25,1);
        move_forward(-25,1);
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
        move_forward(-25,1);
    }
    else if(w==3){
        servo.SetDegree(whiteangle);
        move_forward(25,1);
        move_forward(-25,1);
    }
    else if (b==3){
        servo.SetDegree(blueangle);
        move_forward(25,1);
        move_forward(-25,1);
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




/*******FUNCTIONS FOR SALT BAG CONTROL********/

void find_servo_angle(void)
{
    //sets servo to many different angles, and writes each degree value to the screen
    servo.SetDegree(0);
    LCD.WriteLine("0");
    Sleep(1000);
    servo.SetDegree(15);
    LCD.WriteLine("15");
    Sleep(1000);
    servo.SetDegree(30);
    LCD.WriteLine("30");
    Sleep(1000);
    servo.SetDegree(45);
    LCD.WriteLine("45");
    Sleep(1000);
    servo.SetDegree(60);
    LCD.WriteLine("60");
    Sleep(1000);
    servo.SetDegree(75);
    LCD.WriteLine("75");
    Sleep(1000);
    servo.SetDegree(90);
    LCD.WriteLine("90");
    Sleep(1000);
    servo.SetDegree(105);
    LCD.WriteLine("105");
    Sleep(1000);
    servo.SetDegree(120);
    LCD.WriteLine("120");
    Sleep(1000);
    servo.SetDegree(135);
    LCD.WriteLine("135");
    Sleep(1000);
    servo.SetDegree(150);
    LCD.WriteLine("150");
    Sleep(1000);
    servo.SetDegree(165);
    LCD.WriteLine("165");
    Sleep(1000);
    servo.SetDegree(180);
    LCD.WriteLine("180");
    Sleep(1000);
}//end find servo angle

void move_to_saltbag(void)
{
    //positions robot in ideal scooping position
    move_forward(-25, 22);
    check_y_plus(6.8);
    turn_right(-25, 5);
    check_x_minus(17.699);
    check_heading(90.0);
    move_forward(-25,2);
}

void scoop(void)
{
    //lowers servo to scooping position and drives forward until salt bag is within confines of scooper
    saltservo.SetDegree(scoopangle);
    move_forward(-25,8);
    Sleep(1000);
    saltservo.SetDegree(saltholdingangle);//hold salt bag at high enough angle that it won't fall out
    Sleep(5000);
}

void move_to_garage(void)
{
    //once robot is at the buttons, it will find the garage
    move_forward(-25,3);
    turn_right(25,5);
    check_y_plus(51.9);//robot is moving backwards, so flip signs of each RPS function
    check_x_plus(11.7);
    check_heading(224.1);
}

void deposit_salt()
{
    //robot will lower the scoop and deposit the salt bag in the garage
    Sleep(50);
    move_forward(-25, 5);
    //robot will move backward really fast to let the salt bag slide out
    saltservo.SetDegree(scoopangle);
    move_forward(70,5);

}

void move_up_ramp(void)
{
    LCD.Clear();
    write_coordinates();

    move_forward(10,1);
    turn_left(-25,8);
    move_forward(-25,2);
    check_heading(180);

//move to bottom of ramp
    write_coordinates();
    Sleep(50);
    move_forward(-25,5);//move to point D
    Sleep(50);
    check_y_minus(24.9);//make sure robot is at correct y coordinate

//robot should be at point D
    move_forward(-80, 20);//full power to make it up the ramp
    write_coordinates();
    Sleep(50);

}//end of move_up_ramp



/******CRANK FUNCTIONS*********/

void position_to_crank(void)
{
    //move away from ramp
    turn_left(10, 0.3);
    move_forward(30, 7);
    check_heading(25);
    check_x_minus(31.411);
    check_heading(0.00);
    check_y_plus(55.0);
}//end position_to_crank

void position_to_crank_final(void)
{
    //just like original, except the robot is facing backwards, and has to turn around
    move_forward(-30,5);
    //attempt a three point turn
    turn_right(30,5);
    turn_left(-30, 5);
    check_heading(15);
    check_x_minus(30.911);
    check_heading(0.00);
    check_y_plus(54.6);
}

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
    int direction;
     v=cds.Value();
     int find=0;
         while (v>1.4 && find<10)
         {
             if (v>0.65 && v<=1.4)
             {
                 direction=1;
             }
             else if (v<0.65)
             {
                 direction=2;
             }
             else if (v>1.4)
             {
                 LCD.WriteLine("light not found!");
                 move_forward(10, 0.1);
                 turn_right(10, 0.1);
                 find=find+1;
                 if (find==9)
                 {
                     direction==2;
                     check_y_plus(54.6);
                 }
             }
         }
    int turned=0;
    int degree=0;
    int t1=0, t2=0;
    int degree2=180;
    while (turned<=5)
    {


    if (direction=1)
    {
        //the light color is blue
        //turn motor counterclockwise
        servo.SetDegree(degree);

        Sleep(1000);
        right_motor.SetPercent(-10);
        left_motor.SetPercent(10);
        /*encode = 1 * counts_per_rotation;
        //Reset encoder counts
        right_encoder.ResetCounts();
        left_encoder.ResetCounts();
        //Set both motors to desired percent
        right_motor.SetPercent(-percent);
        left_motor.SetPercent(percent);
        t1=TimeNow();
        //While the average of the left and right encoder are less than counts,
        //keep running motors
        while((left_encoder.Counts() + right_encoder.Counts()) / 2. < encode);
        {
            t2=TimeNow();
            if ((t2-t1)>2)
            {
                break;
            }
        }
        //Turn off motors
        right_motor.Stop();
        left_motor.Stop();*/

        servo.SetDegree(degree2);
                Sleep(1000);

        move_forward(-10,1);
        Sleep(1000);
        turned=turned+1;

    }
    else if (direction=2)
    {
        //light color is red
        //turn motor clockwise
        servo.SetDegree(degree2);
        Sleep(1000);
        move_forward(10, 1);
        Sleep(1000);
        servo.SetDegree(degree);
        Sleep(1000);
        move_forward(-10,1);
        Sleep(1000);
        turned=turned+1;

    }

    }
}//end crank function

void move_up_ramp_original(void)//starts at light and drives to crank
{
    LCD.Clear();

    //robot starts at pt A: (18.4, 31.3) 180 deg
    write_coordinates();
    move_forward(-25, 14);//drive 14 inches down
    Sleep(10);
    write_coordinates();
    check_y_plus(18.4);//ensure that robot went far enough in y direction
    Sleep(10);


    //robot should be at point B: (18.4, 18.4) 180 deg
    write_coordinates();//check that it is
    Sleep(10);
    turn_right(25, 5);
    write_coordinates();
    Sleep(10);
    check_heading(270.0);//make sure robot turned a full 90 degrees
    write_coordinates();
    Sleep(10);
    check_x_plus(18.4);//correct for forward motion in the turn
    Sleep(10);
    move_forward(25, 10);//drive twelve inches in the +x direction
    write_coordinates();
    Sleep(10);
    check_x_plus(30.4);
    write_coordinates();
    Sleep(10);

    //should be at point C
    write_coordinates();//ensure robot is at point C
    Sleep(10);
    turn_right(25,5);
    Sleep(10);
    write_coordinates();
    check_y_minus(18.5);
    write_coordinates();
    Sleep(10);
    move_forward(-25,5);//move to point D
    Sleep(10);
    check_y_minus(24.9);//make sure robot is at correct y coordinate

    //robot should be at point D
    write_coordinates();//check coordinates
    Sleep(10);
    check_heading(180);
    move_forward(-75, 30);//full power to make it up the ramp
    write_coordinates();
    Sleep(10);
    check_y_minus(40.199);
    Sleep(100);
}//end of move_up_ramp_original



/************OIL SWITCH FUNCTIONS****************/

void reach_ramp_top(void)
{
    //starting from garage, go back to where the buttons were to avoid snow pile and trees
    //valuues need to be determined experimentally
    turn_left(30, 2.5);
    check_heading(315);
    move_forward(30, 5);
    check_x_plus(14.9);
    turn_right(30, 6);
    check_heading(230.199);
    move_forward(30, 7);
    check_heading(214.199);
    move_forward(30,5);
}

void move_down_ramp(void)
{
    move_forward(-30, 20);
    //assuming robot is starting at the top of the ramp
    check_heading(180);
    check_y_minus(40.199);
    move_forward(15,10);//drives slowly down ramp
    //position to oil switch
    turn_right(25, 2.5);
    //all of the ofllowing are approximations and need testing
    check_heading(120.0);
    move_forward(30, 31);
}

void toggle_switch(void)
{
    //if oildirec=0, toggle switch to the right
    if (RPS.OilDirec()==0)
    {
        saltservo.SetDegree(saltholdingangle);
        move_forward(30, 2);
    }
    //if oildirec=1, toggle to the left
    else if (RPS.OilDirec()==1)
    {
        move_forward(30, 2);
        saltservo.SetDegree(saltholdingangle);
    }
}

void find_top_of_ramp(void)
{
    //this is just a possibility
    //only work on this time permitting
    //compare rps coordinates to coordinates at top of ramp, and move accordingly
}


void move_up_ramp_original1(void)//starts at light and drives to crank
{
LCD.Clear();
//robot starts at pt A: (18.4, 31.3) 180 deg
write_coordinates();
move_forward(-25, 14);//drive 14 inches down
Sleep(10);
write_coordinates();
check_y_plus(18.4);//ensure that robot went far enough in y direction
Sleep(10);
//robot should be at point B: (18.4, 18.4) 180 deg
write_coordinates();//check that it is
Sleep(10);
turn_right(25, 5);
write_coordinates();
Sleep(10);
check_heading(270.0);//make sure robot turned a full 90 degrees
write_coordinates();
Sleep(10);
check_x_plus(18.4);//correct for forward motion in the turn
Sleep(10);
move_forward(25, 10);//drive twelve inches in the +x direction
write_coordinates();
Sleep(10);
check_x_plus(30.4);
write_coordinates();
Sleep(10);
//should be at point C
write_coordinates();//ensure robot is at point C
Sleep(10);
turn_left(25,5);
Sleep(10);
write_coordinates();
check_x_plus(30.4);
check_y_plus(18.5);
write_coordinates();
Sleep(10);
move_forward(25,5);//move to point D
Sleep(10);
check_y_plus(24.9);//make sure robot is at correct y coordinate
//robot should be at point D
write_coordinates();//check coordinates
Sleep(10);
check_heading(0);
move_forward(40, 18);//full power to make it up the ramp
write_coordinates();
Sleep(10);
check_y_plus(40.199);
Sleep(100);
}//end of move_up_ramp_original
/******MAIN*****/

int main(void)
{
    //final order for competition

    servo.SetDegree(0);
    saltservo.SetDegree(startangle);
    RPS.InitializeMenu();
    start_at_light();
  // move_to_saltbag();
    //scoop();
    //move_up_ramp();
    //position_to_crank();
    //crank();
    //position_to_buttons();
    //press_buttons();
    //move_to_garage();
    //deposit_salt();
     move_up_ramp_original1();
     position_to_crank();


//    position_to_crank_final();
     crank();
}

