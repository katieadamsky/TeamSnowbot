#include <FEHBuzzer.cpp>
#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include <FEHMotor.h>
#include <FEHRPS.h>
#include <FEHServo.h>

int main (void)
{
  
  //measure 1
  Tone(370);//Gf
  Sleep(750);
  Tone(466);//Bf
  Sleep(125);
  Tone(415);//Af
  Sleep(125);
  Tone(466);//bf
  Sleep(500);
  Tone(311);
  Sleep(500);//ef
  
  //measure 2
  Tone(247);//Cf
  Sleep(750);
  Tone(494);//high cf
  Sleep(125);
  Tone(466);//bf
  Sleep(125);
  Tone(494);//high cf
  Sleep(250);
  Tone(466);//bf
  Sleep(250);
  Tone(415);//af
  Sleep(500);
  
  //measure 3
  Tone(415);
  Sleep(750);
  Tone(494);
  Sleep(125);
  Tone(466);
  Sleep(125);
  Tone(494);
  Sleep(500);
  Tone(311);
  Sleep(500);
  
  //measure 4
  Tone(349);//f
  Sleep(750);
  Tone(415);//af
  Sleep(125);
  Tone(370);//gf
  Sleep(125);
  Tone(415);//af
  Sleep(250);
  Tone(370);
  Sleep(250);
  Tone(349);
  Sleep(250);
  Tone(415);
  Sleep(250);
  
  //measure 5
  Tone(370);
  Sleep(750);
  Tone(349);
  Sleep(125);
  Tone(370);
  Sleep(125);
  Tone(415);
  Sleep(750);
  Tone(370);
  Sleep(125);
  Tone(415);
  Sleep(125);
  
  //measure 6
  Tone(466);
  Sleep(250);
  Tone(415);
  Sleep(250);
  Tone(370);
  Sleep(250);
  Tone(349);
  Sleep(250);
  Tone(311);
  Sleep(500);
  Tone(494);
  Sleep(500);
  
  //measure 7
  Tone(466);
  Sleep(1500);
  Tone(466);
  Sleep(125);
  Tone(494);
  Sleep(125);
  Tone(466);
  Sleep(125);
  Tone(415);
  Sleep(125);
  
  //measure 8
  Tone(466);
  Sleep(4000);
  
}
