#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include <FEHMotor.h>
#include <FEHRPS.h>
#define LINE_ON_RIGHT 0
#define ON_LINE_FIRST 1
#define LINE_ON_LEFT 2
#define ON_LINE_SECOND 3
AnalogInputPin opto(FEHIO::P0_0);
FEHMotor rightmotor(FEHMotor::Motor0);
FEHMotor leftmotor(FEHMotor::Motor1);
int main() {
int state = LINE_ON_RIGHT; // Set the initial state
float o;
while (true) { // I will follow this line forever!
o=opto.Value();
LCD.WriteLine(o);
switch(state) {
// If the line is on my right...
case LINE_ON_RIGHT:
// Set motor powers for right turn
rightmotor.SetPercent(25);
leftmotor.SetPercent(75);
Sleep(250);
rightmotor.SetPercent(0);
leftmotor.SetPercent(0);
if ( o>2.7 ) {
state = ON_LINE_FIRST; // update a new state
}
break;
// If I am on the line, but zigging to the right...
case ON_LINE_FIRST:
rightmotor.SetPercent(25);
leftmotor.SetPercent(75);
Sleep(250);
rightmotor.SetPercent(0);
leftmotor.SetPercent(0);
if( o<2.8 ) {
state = LINE_ON_LEFT; // update a new state
}
break;
// If the line is on my left...
case LINE_ON_LEFT:
rightmotor.SetPercent(75);
leftmotor.SetPercent(25);
Sleep(250);
rightmotor.SetPercent(0);
leftmotor.SetPercent(0);
if ( o>3.0 ) {
state = ON_LINE_SECOND; // update a new state
}
break;
// If I am on the line, but zagging to the left...
case ON_LINE_SECOND:
rightmotor.SetPercent(75);
leftmotor.SetPercent(25);
Sleep(250);
rightmotor.SetPercent(0);
leftmotor.SetPercent(0);
if( o<2.8 ) {
state = LINE_ON_RIGHT; // update a new state
}
break;
default:LCD.WriteLine("ERROR."); // Error. Something is very wrong.
break;
}//end switch case
// Sleep a bit
Sleep(500);
}
}
