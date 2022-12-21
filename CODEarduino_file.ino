#include<LiquidCrystal.h>
LiquidCrystal lcd(12,11,5,4,3,2);
int value=0;
float AVOL;
int Vis;
int volt=A0;
#include "EmonLib.h"                   // Include Emon Library
EnergyMonitor emon1;                   // Create an instance
int pin = 13;
float rads = 57.29577951; // 1 radian = approx 57 deg.
float degree = 360;
float frequency = 50;
float nano = 1 * pow (10,-6); // Multiplication factor to convert nano seconds into seconds

// Define floats to contain calculations
float rads = 57.29577951; // 1 radian = approx 57 deg.
float degree = 360;
float frequency = 50;
float nano = 1 * pow (10,-6); // Multiplication factor to convert nano seconds into seconds
float pf;
float angle;
float pf_max = 0;
float angle_max = 0;
int ctr;
float AP;
float Energy;

void setup() {
  emon1.current(1, 60.5);
  lcd.begin(16,2);
  lcd.clear();
  pinMode(volt,INPUT);
  int pin = 13;
}
void loop()
 {
  //VOLTAGE CODE
  
 value = analogRead(volt);
 AVOL=value*(5.0/1024);
 Vis=AVOL*(230.0/0.78);
 //CURRENT CODE
 double Irms = emon1.calcIrms(1480);
 // POWER FACTOR CODE
 
 for (ctr = 0; ctr <= 4; ctr++) // Perform 4 measurements then reset
  {
  // 1st line calculates the phase angle in degrees from differentiated time pulse
  // Function COS uses radians not Degree's hence conversion made by dividing angle / 57.2958
   angle = ((((pulseIn(pin, HIGH)) * nano)* degree)* frequency);
  // pf = cos(angle / rads);
  
   if (angle > angle_max) // Test if the angle is maximum angle
     {
      angle_max = angle; // If maximum record in variable "angle_max"
      pf_max = cos(angle_max / rads); // Calc PF from "angle_max"
     }
   }
   if (angle_max > 360) // If the calculation is higher than 360 do following...
   {
    angle_max = 0; // assign the 0 to "angle_max"
    pf_max = 1; // Assign the Unity PF to "pf_max"
   }
   if (angle_max == 0) // If the calculation is higher than 360 do following...
   {
    angle_max = 0; // assign the 0 to "angle_max"
    pf_max = 1; // Assign the Unity PF to "pf_max"
   }
    angle = 0; // Reset variables for next test
   angle_max = 0;
   lcd.setCursor(0, 0);
   lcd.print("Vis:");
   lcd.print(Vis);
   lcd.setCursor(8, 0);
   lcd.print("Irms:");
   lcd.print(Irms);
   lcd.print("PF=");
   lcd.print(pf_max);
   lcd.print("EG");
   lcd.print(Energy);
   AP=Vis*Irms*pf_max;//AP =APPARENT POWER
   Energy = Energy + (AP * (2.05/60/60/1000));    //Calculate    kilowatt hours used
delay(2000);
}
