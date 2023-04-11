// A program for a self-learning manipulator


// Potentiometer pins
#define PotenPlanform 0
#define PotenSector1 1
#define PotenSector2 2
#define PotenSector3 3
#define PotenArm 4

// Pins servo
#define PinServoPlatform 5
#define PinServoArm 9
#define PinServoSector1 11
#define PinServoSector2 3
#define PinServoSector3 10

// Other constants
#define RefPositionArm 70
#define RefPosition 1
#define RefPositionPlatform 90
#define ArrSize 200
#define debounce 150
#define KnopkaRec 7
#define KnopkaStart 8

#include <Servo.h>
Servo ServoPlatform;
Servo ServoArm;
Servo ServoSector1;
Servo ServoSector2;
Servo ServoSector3;

byte PoitArrayArm [ArrSize];
byte PoitArrayPlatform [ArrSize];
byte PoitArraySector1 [ArrSize];
byte PoitArraySector2 [ArrSize];
byte PoitArraySector3 [ArrSize];
byte PositServPlatMin = 35;
byte PositServPlatMax = 145;
byte PositServArmMin = 70;
byte PositServArmMax = 1;
int const MovePot [] = {381,259};

byte counter =0;// if the array increases, you need to change the data type
byte counterREC =0;
int buferPosPlatf, buferPosArm, buferPosSec1, buferPosSec2, buferPosSec3;
boolean buttRec, buttStart, cycleRecFlag, cycleStartFlag;
unsigned long button_timer;
int Potentiometer, bufer;

int PotenPosPlatf = 351;

// Reference positions of the potentiometer
int PotenStartPosPlatf = 388;
int PotenStartPosArm = 233;
int PotenStartPosSec1 = 162;
int PotenStartPosSec2 = 33;
int PotenStartPosSec3 = 174;


void setup() {
Serial.begin(9600);
ServoPlatform.attach(PinServoPlatform);
ServoArm.attach(PinServoArm);
ServoSector1.attach(PinServoSector1);
ServoSector2.attach(PinServoSector2);
ServoSector3.attach(PinServoSector3);

ServoPlatform.write(RefPositionPlatform); 
ServoArm.write (RefPositionArm);
ServoSector1.write (RefPosition);
ServoSector2.write (RefPosition);
ServoSector3.write (RefPosition);

pinMode(KnopkaRec, INPUT_PULLUP);
pinMode(KnopkaStart, INPUT_PULLUP);

delay(5000);
}

void loop() {
   
buttRec = !digitalRead(KnopkaRec);
if(buttRec && millis() - button_timer > debounce ){
cycleRecFlag = true;

button_timer = millis();
ServoPlatform.detach();
ServoArm.detach();
ServoSector1.detach();
ServoSector2.detach();
ServoSector3.detach();
delay(1000);
}
while(cycleRecFlag){
   RecPlatiArm (PotenPlanform, PotenPosPlatf,MovePot[0],PositServPlatMin, PositServPlatMax ,PoitArrayPlatform);
   Serial.print F(("Платформа - "));
   Serial.println(PoitArrayPlatform[counter]);
   Rec (PotenSector1, PotenStartPosSec1, PoitArraySector1);
   Serial.print F(("Сектор1 - "));
   Serial.println(PoitArraySector1[counter]);
   Rec (PotenSector2, PotenStartPosSec2, PoitArraySector2);
    Serial.print F(("Сектор2 - "));
   Serial.println(PoitArraySector2[counter]);
   Rec (PotenSector3, PotenStartPosSec3, PoitArraySector3);
    Serial.print F(("Сектор3 - "));
   Serial.println(PoitArraySector3[counter]);
  RecPlatiArm (PotenArm, PotenStartPosArm,MovePot[1], PositServArmMin,PositServArmMax, PoitArrayArm);
     Serial.print F(("Клешня - "));
   Serial.println( PoitArrayArm[counter]);
   
    if(counter >= ArrSize-1){   
    counterREC= counter; 
counter = 0;
cycleRecFlag = false;
break;
    }
counter++;
delay(250);

buttRec = !digitalRead(KnopkaRec);
if(buttRec && millis() - button_timer > debounce){
button_timer = millis();
 counterREC= counter;
counter = 0;
cycleRecFlag = false;
break;
}
  }
  delay(250);
buttStart = !digitalRead(KnopkaStart);
if(buttStart && millis() - button_timer > debounce){
cycleStartFlag=true;
button_timer = millis();
RefPoint();
ServoPlatform.attach(PinServoPlatform);
ServoArm.attach(PinServoArm);
ServoSector1.attach(PinServoSector1);
ServoSector2.attach(PinServoSector2);
ServoSector3.attach(PinServoSector3);
 RecPlatiArm (PotenPlanform, PotenPosPlatf,MovePot[0],PositServPlatMin, PositServPlatMax , buferPosPlatf);
   Rec (PotenSector1, PotenStartPosSec1, buferPosSec1);
   Rec (PotenSector2, PotenStartPosSec2, buferPosSec2);
   Rec (PotenSector3, PotenStartPosSec3, buferPosSec3);
   RecPlatiArm (PotenArm, PotenStartPosArm,MovePot[1], PositServArmMin,PositServArmMax, buferPosArm);
delay(500);
}
while(cycleStartFlag){
  
moveservo(ServoPlatform, buferPosPlatf,PoitArrayPlatform[counter]);
 buferPosPlatf=PoitArrayPlatform[counter];
moveservo (ServoSector1,  buferPosSec1, PoitArraySector1[counter]);
 buferPosSec1=PoitArraySector1[counter];
moveservo (ServoSector2,  buferPosSec2, PoitArraySector2[counter]);
 buferPosSec2=PoitArraySector2[counter];
moveservo (ServoSector3,  buferPosSec3, PoitArraySector3[counter]); 
 buferPosSec3=PoitArraySector3[counter];
moveservo (ServoArm, buferPosArm, PoitArrayArm[counter]);
  buferPosArm=PoitArrayArm[counter];
  if( counterREC-1==counter){
counter = 0;
cycleStartFlag = false;
//RefPoint();
delay(100);
break;
  }
  counter++;
  delay(50);
  
}

}

void moveservo (Servo &myservo,byte rok1,byte rok2){
    while(rok1<rok2){ 
      for(byte angel=rok1; angel<=rok2; angel++){
      myservo.write(angel);
      Serial.println(analogRead(angel));
      delay(20);
    }
    break;
    }
    while(rok1>rok2){
      for(byte angel=rok1; angel>=rok2; angel--){
      myservo.write(angel);
      Serial.println(analogRead(angel));
      delay(20);
  }
  break;
  }
    }

    void RefPoint(){ 
   RecPlatiArm (PotenPlanform, PotenPosPlatf,MovePot[0],PositServPlatMin, PositServPlatMax, buferPosPlatf);
   Rec (PotenSector1, PotenStartPosSec1, buferPosSec1);
   Rec (PotenSector2, PotenStartPosSec2, buferPosSec2);
   Rec (PotenSector3, PotenStartPosSec3, buferPosSec3);
   RecPlatiArm (PotenArm, PotenStartPosArm,MovePot[1], PositServArmMin,PositServArmMax, buferPosArm);
moveservo (ServoSector1,  buferPosSec1, RefPosition);
moveservo (ServoSector2,  buferPosSec2, RefPosition);
moveservo (ServoSector3,  buferPosSec3, RefPosition); 
moveservo (ServoArm,buferPosArm, RefPositionArm);
moveservo (ServoPlatform,buferPosPlatf,RefPositionPlatform);
ServoPlatform.write(RefPositionPlatform); 
ServoArm.write (RefPositionArm);
ServoSector1.write (RefPosition);
ServoSector2.write (RefPosition);
ServoSector3.write (RefPosition);
}



void Rec ( int const &PinPotentiometera, int const &PotenStartPos, byte Arr[]){
if(counter>=ArrSize-1) return;
Potentiometer = analogRead(PinPotentiometera);
 bufer = constrain(Potentiometer,PotenStartPos, (PotenStartPos+736));
  Arr[counter] = map(bufer, PotenStartPos,(PotenStartPos+736), 0, 180);
}

void Rec ( int const &PinPotentiometera, int const &PotenStartPos, int &buferPos){
if(counter>=ArrSize-1) return;
Potentiometer = analogRead(PinPotentiometera);
 bufer = constrain(Potentiometer,PotenStartPos, (PotenStartPos+736));
 buferPos = map(bufer, PotenStartPos,(PotenStartPos+736), 0, 180);
}



void RecPlatiArm( int const &PinPotentiometera, int const &PotenPos,int const &MovePot, byte const &PositServMin,byte const &PositServMax, byte Arr[] ){

Potentiometer = analogRead(PinPotentiometera);
 bufer = constrain(Potentiometer,PotenPos, (PotenPos+MovePot));
   Arr[counter] = map(bufer, PotenPos,(PotenPos+MovePot),PositServMin, PositServMax);
}

void  RecPlatiArm ( int const &PinPotentiometera, int const &PotenPos,int const &MovePot, byte const &PositServMin,byte const &PositServMax, int &buferPos){
if(counter>=ArrSize-1) return;
Potentiometer = analogRead(PinPotentiometera);
 bufer = constrain(Potentiometer,PotenPos, (PotenPos+MovePot));
  buferPos = map(bufer, PotenPos,(PotenPos+MovePot),PositServMin, PositServMax);
}
 
