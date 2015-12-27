#include <Encoder.h>
Encoder myEnc(2,3);
long oldPosition  = -999;
float angle = 0.0 , Dangle = 0.0 ;
int errorAngle = 0;
int in =0;

void setup() {
  Serial.begin(115200);
  pinMode(2 , INPUT);
  pinMode(3 , INPUT);
  pinMode(6 , OUTPUT);
  pinMode(7 , OUTPUT);
  pinMode(A5 , INPUT);
}

void loop()
{
  
Dangle = 0.9*Dangle + 0.1* map(analogRead(A5), -1 , 1023,0,76); 
if(Dangle < 0 ) Dangle = 0; 
else if(Dangle > 75) Dangle = 75;
  
 /* if(Serial.available())
  { in = Serial.read(); }
  
  if(in == 0) Dangle = 0;
 else if(in == '1') Dangle = 10;
 else if(in == '2') Dangle = 20;
 else if(in == '3') Dangle = 30;
 else if(in == '4') Dangle = 40;
 else if(in == '5') Dangle = 50;
 else if(in == '6') Dangle = 60;
 else if(in == '7') Dangle = 70;
 */
  
   long newPosition = myEnc.read();
  if (newPosition != oldPosition)
  {
    oldPosition = newPosition;
    angle = float(newPosition) / 556.0;
    Serial.write(int(angle));
  }
 //Serial.print(int(errorAngle)); Serial.print('\t');
 
 errorAngle = (Dangle - angle)*-1;
      if(errorAngle < -2) {digitalWrite(7 , HIGH); digitalWrite(6 , LOW);}
 else if(errorAngle > 2 ) {digitalWrite(6 , HIGH); digitalWrite(7 , LOW);}
 else {digitalWrite(6 , LOW); digitalWrite(7 , LOW);}
 
 
}
