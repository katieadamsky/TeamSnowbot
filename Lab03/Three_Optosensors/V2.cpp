#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include <FEHMotor.h>
#include <FEHRPS.h>
AnalogInputPin center(FEHIO::P0_0);
AnalogInputPin right(FEHIO::P0_2);
AnalogInputPin left(FEHIO::P0_4);
FEHMotor rightmotor(FEHMotor::Motor0);
FEHMotor leftmotor(FEHMotor::Motor1);
#define totheleft 1
#define littletotheleft 2
#define centered 3
#define littletotheright 4
#define totheright 5
int main(void)
{
float c=0, r=0, l=0;
int previous;
while (true)
{
//read values from optosensors
c=center.Value();
r=right.Value();
l=left.Value();
LCD.Write("Right");
LCD.WriteLine(r);
LCD.Write("Center");
LCD.WriteLine(c);
LCD.Write("Left");
LCD.WriteLine(l);
if ( c<2.8 && r>3.0 && l<2.8)//on left side of line
{
rightmotor.SetPercent(10);//motors will do a right turn
leftmotor.SetPercent(75);
Sleep(200);

previous=totheleft;
}
else if (c>2.8 && r>2.8 && 1<2.8)//still on left side, but closer to following line
{
rightmotor.SetPercent(60);//motors still turn to the right, but less sharply
leftmotor.SetPercent(75);
Sleep(200);

}
else if (c>2.8 && r<2.8 && l<2.8)//perfectly on line
{
rightmotor.SetPercent(75);//motors will drive straight
leftmotor.SetPercent(75);
Sleep(200);

}
else if (c>2.8 && r<2.8 && l>2.8)//on right side of the line, but still very close to it
{
rightmotor.SetPercent(75);//motors will turn slightly to the left
leftmotor.SetPercent(60);
Sleep(200);

}
else if (c<2.8 && r<2.8 && l>2.8)//far to the right side of the line
{
rightmotor.SetPercent(75);//motors will turn more sharply to the left
leftmotor.SetPercent(10);
Sleep(200);
previous=totheright;
}
else if (c<2.5 && r<2.8 && l<2.8)//no sensors can see the line
{
LCD.WriteLine("ERROR: TOO FAR FROM LINE.");
switch (previous){
    case totheright:
        LCD.WriteLine("To the right of the line");
        rightmotor.SetPercent(75);
        leftmotor.SetPercent(25);
        Sleep(250);
        break;
    case totheleft:
        LCD.WriteLine("To the left of the line");
        rightmotor.SetPercent(25);
        leftmotor.SetPercent(75);
        Sleep(250);
        break;
    default:
    LCD.WriteLine("ERROR: State machine failure");
        break;
}
}
else
{
    LCD.WriteLine("ERROR");
}
rightmotor.SetPercent(0);
leftmotor.SetPercent(0);
Sleep(100);
}
}
