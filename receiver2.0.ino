#include<Serial.h>
#include<Servo.h>
int command=500;
int left_motor_f = 5;
int  left_motor_b = 10;
int right_motor_f = 6;
int right_motor_b = 11;
int loader=8;
int thrower=3;
int speed_throw=110;
Servo lower,upper,cap,indicator;
int lp=9;
int up=12;
int cp=13;
int laser_pin=7;
int cap_angle=70;
int lower_angle=160;
int upper_angle=180;
int indicator_angle=144;
void setup()
{pinMode(left_motor_f,OUTPUT);
pinMode(13,OUTPUT);
pinMode(right_motor_f,OUTPUT);
pinMode(right_motor_b,OUTPUT);
pinMode(left_motor_b,OUTPUT);
pinMode(loader,OUTPUT);
pinMode(thrower,OUTPUT);
lower.attach(lp);
upper.attach(up);
cap.attach(cp);
indicator.attach(2);
lower.write(lower_angle);
upper.write(upper_angle);
indicator.write(indicator_angle);
cap.write(70);
}
int recieve()
{ 
  int i;
  int j;
  int k=0;
  
 
    j=digitalRead(A5);
    k=(2*k)+j;
    
    j=digitalRead(A4);
    k=(2*k)+j;
    
    j=digitalRead(A3);
    k=(2*k)+j;
    
    j=digitalRead(A2);
    k=(2*k)+j;
    
  command=k;
  return command;
  
  
}

void foward()
{//Serial.println("foward - 0");
analogWrite(left_motor_f,100);
analogWrite(left_motor_b,20);
analogWrite(right_motor_f,128);
analogWrite(right_motor_b,20);
}
void backward()
{//Serial.println("backward - 1");
analogWrite(left_motor_f,(158));
analogWrite(left_motor_b,255);
analogWrite(right_motor_f,(148));
analogWrite(right_motor_b,255);
}
void left()
{//Serial.println("left - 2");
analogWrite(left_motor_f,128+30);
analogWrite(left_motor_b,255);
analogWrite(right_motor_f,128);
analogWrite(right_motor_b,30);
}
void right()
{//Serial.println("right - 3");
analogWrite(left_motor_f,128);
analogWrite(left_motor_b,30);
analogWrite(right_motor_f,128+30);
analogWrite(right_motor_b,255);
}
void close_gate()
{//Serial.println("gate close - 9");
while(cap_angle<180)
{cap_angle+=1;
cap.write(cap_angle);
delay(10);
}
}
void open_gate() 
{//Serial.println("open gate - 10");
while(cap_angle>70)
{cap_angle-=1;
cap.write(cap_angle);
delay(10);
}
}
void lower_shoulder()
{//Serial.println("lower arm- 4");
if(lower_angle>15 && upper_angle>55)
{
  if(lower_angle>140&&lower_angle<160)
{
upper_angle++;
}
upper_angle-=1;
upper.write(upper_angle);
lower_angle--;
lower.write(lower_angle);
delay(20);
}
}

void raise_shoulder()
{//Serial.println("raise arm- 5");
if(lower_angle<160 && upper_angle<180)
{
  if(lower_angle>140&&lower_angle<160)
{
upper_angle--;
}upper_angle+=1;
upper.write(upper_angle);
lower_angle+=1;
lower.write(lower_angle);
delay(20);
}
}
void  raise_elbow()
{upper_angle++;
upper.write(upper_angle);
delay(20);
}
void  lower_elbow()
{upper_angle--;
upper.write(upper_angle);
delay(20);
}
void raise_arm()
{ nothing();
  // nothing();
  while(lower_angle<170)
  {                                  
    lower.write(lower_angle); 
    lower_angle=lower_angle+5;          
    delay(25); 
  }

  while(upper_angle>0)
  {                                  
    upper.write(upper_angle);
    upper_angle=upper_angle-5;       
    delay(25); 
  }
cap.write(120);
delay(500);
 

 /* analogWrite(thrower,force);
  delay(7000+2*(force*10));
  digitalWrite(loader,HIGH);
  delay(3000);
  analogWrite(thrower,0);
  digitalWrite(loader,LOW);
  */
  
 while(upper_angle<180)
  {                                  
    upper.write(upper_angle);
    upper_angle=upper_angle+5;       
    delay(25);
  upper.write(upper_angle);
}
 while(lower_angle>160)
  {                                  
    lower.write(lower_angle); 
    lower_angle=lower_angle-5;          
    delay(25); 
  }
  cap_angle=0;
  cap.write(cap_angle);
  cap_angle=70;
  cap.write(cap_angle);
  
  
}

void load_ball()
{  
  
cap.write(120);
delay(300);
cap.write(160);
delay(2000);
  
}
void speed_increase()
{if(indicator_angle>0)
 {speed_throw+=3;
indicator_angle-=18;
indicator.write(indicator_angle);
delay(1000);
 }
}
void speed_decrease()
{if(indicator_angle<180)
 {speed_throw-=3;
indicator_angle+=18;
indicator.write(indicator_angle);
delay(1000);
 }
}
void throw_ball()
{analogWrite(thrower,speed_throw);
  delay(7000);
  digitalWrite(loader,HIGH);
  delay(2400);
  analogWrite(thrower,0);
  digitalWrite(loader,LOW);
}
void nothing()
{//Serial.println("nothing - 11");
digitalWrite(left_motor_f,LOW);
digitalWrite(left_motor_b,LOW);
digitalWrite(right_motor_f,LOW);
digitalWrite(right_motor_b,LOW);
digitalWrite(loader,LOW);
analogWrite(thrower,0);
}

int confirm_recieve()
{
  int old_comm=recieve();
  delay(2);
  int new_comm=recieve();
  while(new_comm!=old_comm)
  {
    old_comm=new_comm;
    new_comm=recieve();
    delay(2);
  }
  return new_comm;
}

void loop()
{
  command=confirm_recieve();
switch(command)
{case 1: foward();break;
case 2: backward();break;
case 3: left();break;
case 4: right();break;
case 5: nothing();break;
case 6: nothing();break;
case 7: lower_shoulder();break;
case 8: raise_shoulder();break;
case 9: close_gate();break;
case 10: open_gate();break;
case 11: raise_arm();break; 
case 12:speed_decrease();break;
case 13:speed_increase();break;
case 14:throw_ball(); break;
case 15:load_ball();break;
case 0: nothing();break;
default:nothing();

}//recieve number
//act accordingly
}























































































































































































































































































