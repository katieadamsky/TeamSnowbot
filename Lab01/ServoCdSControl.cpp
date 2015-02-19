/*Servo Motor Control with a CdS Cell
Written by Katie Adamsky 2/11/15
RJF 8:00 Team B6*/
#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include <FEHServo.h>
#include <FEHMotor.h>

// declare servo arm

int main(void)
{

    ButtonBoard button( FEHIO::Bank3 );

    FEHServo servo(FEHServo::Servo7);

    AnalogInputPin cds(FEHIO::P0_0); //cds cell

    DigitalInputPin micro(FEHIO::P1_0); //microswitch

    LCD.Clear( FEHLCD::Black );

    LCD.SetFontColor( FEHLCD::White );

     float v, d, motorvalue;

int i=0, m;
//set initial servo angles
servo.SetMin(522);
servo.SetMax(2279);

    //wait for middle button to be pressed

    while (!button.MiddlePressed())

      {

      }

      Sleep(500);

      servo.SetDegree(522);

   while( i==0 )

    {

           d=cds.Value(); //read in a second value from the cds cell

           motorvalue=d*60; //determine the change and multiply it by a scaling factor
           LCD.WriteLine(motorvalue);



Sleep(5);

servo.SetDegree(motorvalue);

Sleep(10);

//determine if the program should continue running

m=micro.Value();

if (button.MiddlePressed())

{

i=1; //if middle button is pressed, program will stop
servo.SetDegree(522);

}

else if (m==0)

{

i=1; //if microswitch is pressed, program will also stop
servo.SetDegree(522);
}

    }//end while loop

    return 0;

}
