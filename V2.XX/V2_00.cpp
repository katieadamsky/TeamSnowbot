/*Performance Test 3: Pressing the buttons
Written by Katie Adamsky 2/18/15
RJF 8:00 Team B6*/

#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include <FEHMotor.h>
#include <FEHRPS.h>
#include <FEHServo.h>

#define counts_per_rotation 33.7408
//define different levels of motor power for easily modifying later
#define full_power 100
#define normal_power 75
#define half_power 50
#define order1 1
#define order2 2
#define order3 3
#define order4 4
#define order5 5
#define order6 6
#define topbutton 7
#define middlebutton 8
#define bottombutton 9

//Declarations for encoders & motors
ButtonBoard buttons(FEHIO::Bank3);
DigitalEncoder right_encoder(FEHIO::P0_0);
DigitalEncoder left_encoder(FEHIO::P0_2);
FEHMotor right_motor(FEHMotor::Motor0);
FEHMotor left_motor(FEHMotor::Motor1);
AnalogInputPin cds(FEHIO::P0_4); //CdS cell
FEHServo servo(FEHServo::Servo0);

void press_buttons(void){
  //WHAT IS THE FUNCTION TO INTERPRET RPS DATA
  
}
