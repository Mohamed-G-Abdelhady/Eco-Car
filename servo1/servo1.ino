#include <Encoder.h>
Encoder myEnc(2,3);
long oldPosition  = -999;
float ang = 0.0 , angle = 0.0 ,offsetAngle = 0.0, Dangle = 0.0 ,old_Angle =0 , derivativeAngle = 0 ;
int stallTimer = 0;                           
int errorAngle = 0;
int in =0;
float minAngle = 0 , maxAngle = 82;
void setup() {
  Serial.begin(115200);
  pinMode(2 , INPUT);
  pinMode(3 , INPUT);
  pinMode(6 , OUTPUT);
  pinMode(5 , OUTPUT);
  pinMode(A5 , INPUT);
}

void loop()
{
Dangle = 0.9*Dangle + 0.1* map(analogRead(A0), -1 , 1023,minAngle,maxAngle);  // read desired angle from potentiometer 
if(Dangle < minAngle ) Dangle = minAngle; 
else if(Dangle > maxAngle) Dangle = maxAngle;
  
  old_Angle = angle;
   long newPosition = myEnc.read();      // read angles from encoder
  if (newPosition != oldPosition)
  {
    oldPosition = newPosition;
    ang = float(newPosition) / 556.0;
    angle =  ang - offsetAngle;
   Serial.write(int(angle));  //send angle to the other micro processor
  // Serial.print(angle); Serial.print('\t');
  // Serial.println(errorAngle);
  }
 //Serial.print(int(errorAngle)); Serial.print('\t');
 
 errorAngle = (Dangle - angle);  // error and derivative calaculations
 derivativeAngle = angle - old_Angle;
 
 if(abs(errorAngle) > 2)   
 {
   
   
 if(derivativeAngle == 0 )   //blocking protection 
 {
 if(stallTimer >  100) {stallTimer = 0; if(errorAngle < 0 ) offsetAngle = ang; else offsetAngle =  ang - maxAngle; }
 else stallTimer += 1;
 }
 else stallTimer = 0 ;     //end of blocking protection 
 
 
      if(errorAngle > 0) {digitalWrite(5 , HIGH); digitalWrite(6 , LOW);}  // actions
 else if(errorAngle < 0) {digitalWrite(6 , HIGH); digitalWrite(5 , LOW);}
 }
 else {digitalWrite(6 , LOW); digitalWrite(5 , LOW);}
  
}

 /* if(Serial.available())
  { in = Serial.read(); }
  
  if(in == 0) Dangle = 0;
 else if(in == '1') Dangle = 10;
 else if(in == '2') Dangle = 20;
 else if(in == '3') Dangle = 30;
 else if(in == '4') Dangle = 40;
 else if(in == '5') Dangle = 50;
 else if(in == '6') Dangle = 60;
 else if(in == '5') Dangle = 50;
 */
 
