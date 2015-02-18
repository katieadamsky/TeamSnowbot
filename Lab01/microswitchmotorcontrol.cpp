#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include <FEHServo.h>
#include <FEHMotor.h>

 ButtonBoard board(FEHIO::Bank3)
 FEHMotor motor(FEHMotor::Motor0);
 DigitalInputPin micro(FEHIO::P0_0)

int main(void)
{
     while (!MiddlePressed)
     {
    int v;
    v=micro.Value();
    sleep(100);
    if (v==1)
   { motor.SetPower(127);
    LCD.Write("Motor full power forward\n");
    }
    else if (v==0)
    {
    motor.SetPower(0);
    LCD.Write("Motor off\n");
   }Sleep(5000);
   }
    motor.SetPower(0);
    LCD.Write("Motor off\n");


    return 0;
}
