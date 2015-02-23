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
  float c, r, l;
  int previous;
  while (true);
  {
    //read values from optosensors
    c=center.Value();
    r=right.Value();
    l=left.Value();
    LCD.WriteLine(r);
    if ( c<2 && r>2.7 && l<2)//on left side of line
    {
    rightmotor.SetPercent(25);//motors will do a right turn
    leftmotor.SetPercent(75);
    Sleep(250);
    rightmotor.SetPercent(0);
    leftmotor.SetPercent(0);
    previous=totheleft;
    }
    else if (c>2.7 && r>2.7 && 1<2)//still on left side, but closer to following line
    {
    rightmotor.SetPercent(60);//motors still turn to the right, but less sharply
    leftmotor.SetPercent(75);
    Sleep(250);
    rightmotor.SetPercent(0);
    leftmotor.SetPercent(0);
    previous=littletotheleft;
    }
    else if (c>2.7 && r<2 && l<2)//perfectly on line
    {
    rightmotor.SetPercent(75);//motors will drive straight
    leftmotor.SetPercent(75);
    Sleep(250);
    rightmotor.SetPercent(0);
    leftmotor.SetPercent(0);
    previous=centered;
    }
    else if (c>2.7 && r<2 && l>2.7)//on right side of the line, but still very close to it
    {
    rightmotor.SetPercent(75);//motors will turn slightly to the left
    leftmotor.SetPercent(60);
    Sleep(250);
    rightmotor.SetPercent(0);
    leftmotor.SetPercent(0);
    previous=littletotheright;
    }
    else if (c<2 && r<2 && l>2.7)//far to the right side of the line
    {
    rightmotor.SetPercent(75);//motors will turn more sharply to the left
    leftmotor.SetPercent(25);
    Sleep(250);
    rightmotor.SetPercent(0);
    leftmotor.SetPercent(0);
    previous=totheright;
    }
    else if (c<2 && r<2 && l<2)
    {
      LCD.WriteLine("ERROR: TOO FAR FROM LINE.");
      switch (previous){
        case //THIS IS WHERE YOIU LEFT OFF LAST TIME
      }
    }
  }
}
