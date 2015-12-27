#include <Wire.h>  
#include "LiquidCrystal_I2C.h"
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address

#define magnet A1//pin connected to magnet switch
int angle = 0;

//velocity algorithm definitions
int s = 0 , s_old = 0; // magnet sensor return 0 or 1
float t = 0; // the timer for s = 0 in seconds
float r = 0; // timer  for s = 1
float dt = 1.0/10000.0; //time incremental = 1 / interrupt frequency
float v = 0 ;  //velocity
float n = 1.0; //number of magnets
float dis = 2*M_PI*0.3 / n; //wheel perimeter in meter
float v_min = 2; //min velocity in kph
float t_max = dis / v_min * 60*60/1000; //max time in seconds

void setup()
{
    lcd.begin(16,2);
    welcome();
 cli();//stop interrupts
  //set timer2 interrupt at 10kHz
  TCCR2A = 0;// set entire TCCR2A register to 0
  TCCR2B = 0;// same for TCCR2B
  TCNT2  = 0;//initialize counter value to 0
  // set timer count for 10khz increments
  OCR2A = 199;// = 16*10^6 / (frequency * prescalar) - 1 < 2^8 for timer2 8 bit
  // turn on CTC mode
  TCCR2A |= (1 << WGM21);
  // Set CS21 bit for 8 prescaler
  TCCR2B |= (1 << CS21);   
  // enable timer compare interrupt
  TIMSK2 |= (1 << OCIE2A);

  sei();//allow interrupts
  //END TIMER SETUP
  
Serial.begin(115200);
pinMode(magnet, INPUT);

}

ISR(TIMER2_COMPA_vect) 
{
  s_old = s;
  s = digitalRead(magnet);
  
  if(s == 0 & s_old ==0) 
{
if(t >= t_max) v = 0;
else t += dt;
r = 0;
}
else if(s ==1 & s_old ==0) 
{
v = dis / t; 
t = 0;
r = 0;
}
else if (s==1 & s_old ==1)
{
if(r >= t_max) v = 0;
else r = r + dt;
t = 0;
}
else
{
t = 0;
r = 0;
}

}



void loop(){
 if(Serial.available()) {angle = Serial.read();}
 
  //print kph once 0.01 second

Serial.print('s');
Serial.print(int(v));
Serial.print(',');
Serial.print(int(angle));
Serial.print('#');
Serial.print('\n');

lcd.setCursor(5,0);
lcd.print(int(v));
lcd.setCursor(7,0);
lcd.print("KPH");
lcd.setCursor(0,1);
lcd.print("Power:");
lcd.setCursor(6,1);
lcd.print(int(angle));
  
  delay(10);
                     
}

void welcome()
{
  lcd.setBacklight(LOW);
  delay(300);
  lcd.setBacklight(HIGH);
  delay(300);
  lcd.setBacklight(LOW);
  delay(300);
  lcd.setBacklight(HIGH);
  delay(300);
  lcd.setCursor(0,0);
  lcd.print("Shell Eco-Marathon");
  lcd.setCursor(0,1);
  lcd.print("Cairo Uni Team");
  delay(5000);
  lcd.clear();
}
  
