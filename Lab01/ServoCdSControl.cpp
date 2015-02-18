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
servo.SetMin(500);
servo.SetMax(2500);

    //wait for middle button to be pressed

    while (!button.MiddlePressed())

      {

      }

      Sleep(500);

      servo.SetDegree(500);

   while( i==0 )

    {



       v=cds.Value(); //read in value from cds cell

       Sleep(500);

       d=cds.Value(); //read in a second value from the cds cell

           motorvalue=(d-v)*689.66; //determine the change and multiply it by a scaling factor
           LCD.Write(motorvalue);



Sleep(500);

servo.SetDegree(500+motorvalue);

Sleep(1000);

//determine if the program should continue running

m=micro.Value();

if (button.MiddlePressed())

{

i=1; //if middle button is pressed, program will stop
servo.SetDegree(500);

}

else if (m==0)

{

i=1; //if microswitch is pressed, program will also stop
servo.SetDegree(500);
}

    }//end while loop

    return 0;

}
