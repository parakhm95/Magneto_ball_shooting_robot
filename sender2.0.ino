#include "I2Cdev.h"
#include<Serial.h>
#include "MPU6050_6Axis_MotionApps20.h"
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif
MPU6050 mpu;
#define OUTPUT_READABLE_YAWPITCHROLL
//#define LED_PIN 13 // (Arduino is 13, Teensy is 11, Teensy++ is 6)
bool blinkState = false;

bool dmpReady = false;  // set true if DMP init was successful
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer
Quaternion q;           // [w, x, y, z]         quaternion container
VectorInt16 aa;         // [x, y, z]            accel sensor measurements
VectorInt16 aaReal;     // [x, y, z]            gravity-free accel sensor measurements
VectorInt16 aaWorld;    // [x, y, z]            world-frame accel sensor measurements
VectorFloat gravity;    // [x, y, z]            gravity vector
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector
volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high
void dmpDataReady() {
    mpuInterrupt = true;
}
//int flex_left;
//int flex_right;
int led0=11;
int led1=13;
int fl;
int fr;
float pitch_foward=-20;
float pitch_back=15;
float roll_right= 25;
float roll_left= -15;
//setup pins
void transmit(int);
void setup(){
  pinMode(12,OUTPUT);
  digitalWrite(12,LOW);
  transmit(11);
    #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
        Wire.begin();
        TWBR = 24; 
    #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
        Fastwire::setup(400, true);
    #endif
    Serial.begin(115200);
    while (!Serial);
    Serial.println(F("Initializing I2C devices..."));
    mpu.initialize();
    Serial.println(F("Testing device connections..."));
    Serial.println(mpu.testConnection() ? F("MPU6050 connection successful") : F("MPU6050 connection failed"));
    //Serial.println(F("\nSend any character to begin DMP programming and demo: "));
    //while (Serial.available() && Serial.read()); 
    //while (!Serial.available());                 
    //while (Serial.available() && Serial.read());
    Serial.println(F("Initializing DMP..."));
    devStatus = mpu.dmpInitialize();
    mpu.setXGyroOffset(220);
    mpu.setYGyroOffset(76);
    mpu.setZGyroOffset(-85);
    mpu.setZAccelOffset(1788); 
    if (devStatus == 0) {
        Serial.println(F("Enabling DMP..."));
        mpu.setDMPEnabled(true);
        Serial.println(F("Enabling interrupt detection (Arduino external interrupt 0)..."));
        attachInterrupt(0, dmpDataReady, RISING);
        mpuIntStatus = mpu.getIntStatus();
        Serial.println(F("DMP ready! Waiting for first interrupt..."));
        dmpReady = true;
        packetSize = mpu.dmpGetFIFOPacketSize();
    } else {
        Serial.print(F("DMP Initialization failed (code "));
        Serial.print(devStatus);
        Serial.println(F(")"));
    }
    
    
    //pin setup
    pinMode(led0,OUTPUT);
    pinMode(led1,OUTPUT);
    pinMode(A0,INPUT);
    pinMode(A1,INPUT);
  pinMode(8,OUTPUT);
  pinMode(9,OUTPUT);
  pinMode(10,OUTPUT);
  pinMode(11,OUTPUT);
    
    
}
int i=500 ;
float p=0;
float r=0;
int fleft=0;
int fright=0;
void loop()
{ 
 label:  if (!dmpReady) return;
    while (!mpuInterrupt && fifoCount < packetSize) {
    }
    mpuInterrupt = false;
    mpuIntStatus = mpu.getIntStatus();
    fifoCount = mpu.getFIFOCount();
    if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
        mpu.resetFIFO();
        Serial.println(F("FIFO overflow!"));
 } else if (mpuIntStatus & 0x02) {
        while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();
        mpu.getFIFOBytes(fifoBuffer, packetSize);
        fifoCount -= packetSize;
            mpu.dmpGetQuaternion(&q, fifoBuffer);
            mpu.dmpGetGravity(&gravity, &q);
            mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
            p=ypr[1] * 180/M_PI;
            r=ypr[2] * 180/M_PI;
}
if(i>0)
{i--;
  goto label;
}

fleft=analogRead(A0);
fright=analogRead(A1);
if(fleft>400)fl=1;
if(fleft<350)fl=0;
if(fright>380)fr=1;
if(fright<340)fr=0;
if((fl==1||fl==0)&&(fr==1||fr==0))
{if(fl==1&&fr==0)
{digitalWrite(led0,HIGH);
digitalWrite(led1,LOW);
}
if(fl==0&&fr==1)
{digitalWrite(led0,LOW);
digitalWrite(led1,HIGH);
}
if(fl==1&&fr==1)
{digitalWrite(led0,HIGH);
digitalWrite(led1,HIGH);
}
if(fl==0&&fr==0)
{digitalWrite(led0,LOW);
digitalWrite(led1,LOW);
}

if((fl==1 && fr==1) && p<pitch_foward)
{transmit(1);
Serial.println("foward- 1");
}
if((fl==1 && fr==1) && p>pitch_back)
{transmit(2);
Serial.println("backward- 2");
}
if((fl==1 && fr==1) && r<roll_left)
{transmit(3);
Serial.println("left- 3");
}
if((fl==1 && fr==1)&& r>roll_right)
{transmit(4);
Serial.println("right- 4");
}
if(((fl==0 && fr==1)) && r>roll_right)
{transmit(5);
Serial.println("raise elbow - 5");
}
if(((fl==0 && fr==1)) && r<roll_left)
{transmit(6);
Serial.println("lower elbow - 6");
}
if(((fl==0 && fr==1)) && p<pitch_foward)
{transmit(7);
Serial.println("lower shoulder - 7");
}
if(((fl==0 && fr==1)) && p>pitch_back)
{transmit(8);
Serial.println("raise soulder - 8");
}
if(((fl==1 && fr==0)) && r>roll_right)
{transmit(9);
Serial.println("close cap - 9");
}
if((fl==1 && fr==0) && r<roll_left)
{transmit(10);
Serial.println("open cap - 10");
}

if((fl==1 && fr==0) && p>pitch_back)
{transmit(11);
Serial.println("raise arm - 11");
}

if(fl==0 && fr==0 && r<roll_left)
{transmit(12);
Serial.println("speed decrease  - 12");
}
if(fl==0 && fr==0 && r>roll_right)
{transmit(13);
Serial.println("speed increase - 13");
}
if(fl==0 && fr==0 && p<pitch_foward)
{transmit(14);
Serial.println("throw-14");
}
if(fl==0 && fr==0 && p>pitch_back)
{transmit(15);
Serial.println("load-15");
}
if(p>pitch_foward && p<pitch_back && r>roll_left && r<roll_right)
{transmit(0);
Serial.println("nothing- 0");
}
}
else
{transmit(0);
Serial.println("nothing- 0");
}
}
void transmit(int c)
{ int i;
  int k=7;
  int l;
   for(i=0;i<4;i++)
   {
     l=c%2;
     c=c/2;
     if(l==1)
     {
       digitalWrite(k,HIGH);
     }
     else if(l==0)
     {
       digitalWrite(k,LOW);
     }
     k--;
   }
}
