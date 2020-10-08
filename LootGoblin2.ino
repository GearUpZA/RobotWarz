#include <SBUS.h>
SBUS sbus(Serial1);

int i;
int CH1, CH2, CH3, CH5;
int LDelayTick, RDelayTick;
bool LDelay = false; 
bool RDelay = false;
//Motor 1 will be the left motor
const int motorLDirPin = 11; // Motor 1
const int motorRDirPin = 10; // Motor 2
const int spinnerArmPin = 9; // Motor 3

const int motorLPWMPin = 6; // Motor 1
const int motorRPWMPin = 5; // Motor 2
const int spinnerPWMPin = 3; // Motor 3

bool motorLDir = false;
bool motorRDir = false;
bool motorLDirPrev = false;
bool motorRDirPrev = false;

bool spinnerArm = false;

bool powerOn = true;

void setup()
{
  sbus.begin();
  LDelayTick = 0;
  RDelayTick = 0;
  pinMode(motorLDirPin, OUTPUT);
  pinMode(motorRDirPin, OUTPUT);
  pinMode(spinnerArmPin, OUTPUT);
  pinMode(motorLPWMPin, OUTPUT);
  pinMode(motorRPWMPin, OUTPUT);
  pinMode(spinnerPWMPin, OUTPUT);

  
  Serial.begin(115200);
  Serial.println("SBUS Status");
}

//Channel 2 forward back 83 to -83
//Channel 1 left right (right is positive)
//Channel 3 weapon positive is full speed
//Channel 5 is arm

// this is timer2, which triggers ever 1ms and processes the incoming SBUS datastream
//ISR(TIMER2_COMPA_vect)
//{
//  sbus.process();
//}

int rightMotor;
int leftMotor;
int spinnerMotor;
  
void loop()
{
  delay(1);
  sbus.process();

  CH1 = sbus.getNormalizedChannel(1); //left right
  CH2 = sbus.getNormalizedChannel(2); //forward back
  CH3 = sbus.getNormalizedChannel(3); //spinner
  CH5 = sbus.getNormalizedChannel(5);


  
  //if there is a failsafe, write 0 to all motor outputs
  if ((sbus.getFailsafeStatus() == SBUS_FAILSAFE_ACTIVE) || (powerOn))
  {
    analogWrite(motorLDirPin,0);
    analogWrite(motorRDirPin,0);
    analogWrite(motorLPWMPin,0);
    analogWrite(motorRPWMPin,0);
    analogWrite(spinnerPWMPin,0);

    digitalWrite(spinnerArmPin,0);
    
    //Motor arming
    if (CH5 > 10) powerOn = false;
  }
  else
  {
    //Motor mixing and contraining
    leftMotor = constrain(CH2+CH1,-83, 83);
    rightMotor = constrain(CH2-CH1,-83, 83);
    spinnerMotor = constrain(CH3, -83, 83);

    //Motor direction hysterisis
    if (leftMotor > 10) motorLDir = true;
    if (leftMotor < -10) motorLDir = false;
    if (rightMotor > 10) motorRDir = true;
    if (rightMotor < -10) motorRDir = false;

    //Motor arming
    if (CH5 > 10) spinnerArm = true;
    if (CH5 < -10) spinnerArm = false;
    
    if (motorLDirPrev != motorLDir) LDelay = true;
    if (motorRDirPrev != motorRDir) RDelay = true;
    
    motorLDirPrev = motorLDir;
    motorRDirPrev = motorRDir;

    //write to the motor direction pin
//    digitalWrite(motorLDirPin,motorLDir);
//    digitalWrite(motorRDirPin,motorRDir);
//    digitalWrite(spinnerArmPin,spinnerArm);

    //scale motor outputs
    leftMotor = abs(leftMotor*3);
    rightMotor = abs(rightMotor*3);
    if (spinnerArm){
      spinnerMotor = (spinnerMotor+83)*3/2;
    }
    else
    { 
      spinnerMotor = 0;
    }

    //add deadzone
    if (leftMotor < 40) leftMotor = 0;
    if (rightMotor < 40) rightMotor = 0;
    if (spinnerMotor < 40) spinnerMotor = 0;

    //left motor output depending on relay switching
    if (LDelay)
    {
      analogWrite(motorLPWMPin,0);
        analogWrite(motorLDirPin,0);
      LDelayTick++;
      if (LDelayTick > 200)
      {
        LDelayTick = 0;
        LDelay = false;
      }
    }
    else
    {
      if(motorLDir){
        analogWrite(motorLDirPin,0);
        analogWrite(motorLPWMPin,leftMotor);
      }
      else{
        analogWrite(motorLPWMPin,0);
        analogWrite(motorLDirPin,leftMotor);
      }
    }

    //right motor output depending on relay switching
    if (RDelay)
    {
      analogWrite(motorRPWMPin,0);
        analogWrite(motorRDirPin,0);
      RDelayTick++;
      if (RDelayTick > 200)
      {
        RDelayTick = 0;
        RDelay = false;
      }
    }
    else
    {      
      if(motorRDir){
        analogWrite(motorRDirPin,0);
        analogWrite(motorRPWMPin,rightMotor); 
      } 
      else{
        analogWrite(motorRPWMPin,0);
        analogWrite(motorRDirPin,rightMotor);
      }
    }
    analogWrite(spinnerPWMPin,spinnerMotor);  
  }
  
  i=(i+1)%300;
  if (i == 0){
    printSBUSStatus();
  }
}

void printSBUSStatus()
{
//  Serial.print("Ch1  ");
//  Serial.println(sbus.getNormalizedChannel(1));
//  Serial.print("Ch2  ");
//  Serial.println(sbus.getNormalizedChannel(2));
//  Serial.print("Ch3  ");
//  Serial.println(sbus.getNormalizedChannel(3));
//  Serial.print("Ch4  ");
//  Serial.println(sbus.getNormalizedChannel(4));
//  Serial.print("Ch5  ");
//  Serial.println(sbus.getNormalizedChannel(5));
//  Serial.print("Ch6  ");
//  Serial.println(sbus.getNormalizedChannel(6));
//  Serial.print("Ch7  ");
//  Serial.println(sbus.getNormalizedChannel(7));
//  Serial.print("Ch8  ");
//  Serial.println(sbus.getNormalizedChannel(8));
//  Serial.print("Ch9  ");
//  Serial.println(sbus.getNormalizedChannel(9));
//  Serial.print("Ch10 ");
//  Serial.println(sbus.getNormalizedChannel(10));
//  Serial.print("Ch11 ");
//  Serial.println(sbus.getNormalizedChannel(11));
//  Serial.print("Ch12 ");
//  Serial.println(sbus.getNormalizedChannel(12));
//  Serial.print("Ch13 ");
//  Serial.println(sbus.getNormalizedChannel(13));
//  Serial.print("Ch14 ");
//  Serial.println(sbus.getNormalizedChannel(14));
//  Serial.print("Ch15 ");
//  Serial.println(sbus.getNormalizedChannel(15));
//  Serial.print("Ch16 ");
//  Serial.println(sbus.getNormalizedChannel(16));
//  Serial.println();
  Serial.print("Failsafe: ");
  if (sbus.getFailsafeStatus() == SBUS_FAILSAFE_ACTIVE) {
    Serial.println("Active");
  }
  if (sbus.getFailsafeStatus() == SBUS_FAILSAFE_INACTIVE) {
    Serial.println("Not Active");
  }
  Serial.print("LeftMotor ");
  Serial.println(leftMotor);
  Serial.print("LeftMotor Dir ");
  Serial.println(motorLDir);
  Serial.print("LeftMotor Delay ");
  Serial.println(LDelay);
  Serial.print("RightMotor ");
  Serial.println(rightMotor);
  Serial.print("RigthMotor Dir ");
  Serial.println(motorRDir);
  Serial.print("RightMotor Delay ");
  Serial.println(RDelay);
  Serial.print("Spinner Arm ");
  Serial.println(spinnerArm);
  Serial.print("Spinner Speed ");
  Serial.println(spinnerMotor);

//  Serial.print("Data loss on connection: ");
//  Serial.print(sbus.getFrameLoss());
//  Serial.println("%");
//
//  Serial.print("Frames: ");
//  Serial.print(sbus.getGoodFrames());
//  Serial.print(" / ");
//  Serial.print(sbus.getLostFrames());
//  Serial.print(" / ");
//  Serial.println(sbus.getDecoderErrorFrames());
//
//  Serial.print("Time diff: ");
  //Serial.println(millis() - sbus.getLastTime());
}
