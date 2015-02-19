/*DC Motor Control with a CdS Cell
Written by Katie Adamsky
2/11/15*/

#include <FEHLCD.h>

#include <FEHIO.h>

#include <FEHUtility.h>

#include <FEHMotor.h>

int main(void)

{

    ButtonBoard button( FEHIO::Bank3 );

    FEHMotor motor(FEHMotor::Motor0);

    AnalogInputPin cds(FEHIO::P0_0); //cds cell

    DigitalInputPin micro(FEHIO::P1_0); //microswitch

    LCD.Clear( FEHLCD::Black );

    LCD.SetFontColor( FEHLCD::White );

     float v, d, motorvalue1=100, motorvalue2;

int i=0, m;

    //wait for middle button to be pressed

    while (!button.MiddlePressed())

      {

      }

      Sleep(500);

            motor.SetPercent(100);//set initial motor speed

   while( i==0 )

    {



       v=cds.Value(); //read in value from cds cell

       Sleep(500);

       d=cds.Value(); //read in a second value from the cds cell

           motorvalue2=(d-v)*68.97; //determine the change and multiply it by a scaling factor
           LCD.Write(motorvalue2);

  motorvalue1=motorvalue1-motorvalue2; //obtain new percentage for motor value

Sleep(500);

motor.SetPercent(motorvalue1); //change motor value

if (d>2.8)

{

motor.SetPercent(-100); 

}
Sleep(1000);

//determine if the program should continue running

m=micro.Value();

if (button.MiddlePressed())

{

i=1; //if middle button is pressed, program will stop
motor.SetPercent(0);

}

else if (m==0)

{

i=1; //if microswitch is pressed, program will also stop
motor.SetPercent(0);
}

    }//end while loop

    return 0;

}
