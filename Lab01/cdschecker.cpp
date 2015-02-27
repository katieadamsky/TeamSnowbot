#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include <FEHServo.h>
#include <FEHMotor.h>

 ButtonBoard board(FEHIO::Bank3);
 FEHMotor motor(FEHMotor::Motor0);
 AnalogInputPin cds(FEHIO::P0_0);

int main(void)
{
    LCD.Clear();
     while (true)
    {
    float v;
    v=cds.Value();
    Sleep(100);
    LCD.WriteLine(v);
    Sleep(1000);
    }
}
