#include <SoftwareSerial.h>

SoftwareSerial BT(10, 11); //TX, RX respetively
String readdata;
String oldreaddata;
int speedA;
int speedB;
 #define INA 3
 #define INB 4
 #define INC 5
 #define IND 9
 #define SPA 8
 #define SPB 6
// #define LIGHT_FRONT1 36
 //#define LIGHT_FRONT2 38
 //#define LIGHT_LEFT 8
 //#define LIGHT_RIGHT 9
 // #define LIGHT_BACK 12
 
void setup() {
 BT.begin(9600);
 Serial.begin(9600);
  pinMode(INA, OUTPUT);
  pinMode(INB, OUTPUT);
  pinMode(INC, OUTPUT);
  pinMode(IND, OUTPUT);
  pinMode(SPA,OUTPUT);
  pinMode(SPB,OUTPUT);
  speedA=100;
  speedB=100;
 // pinMode(LIGHT_FRONT1, OUTPUT);
 // pinMode(LIGHT_FRONT2, OUTPUT);
 // pinMode(LIGHT_LEFT, OUTPUT);
 // pinMode(LIGHT_RIGHT, OUTPUT);
 // pinMode(LIGHT_BACK, OUTPUT);
}
//-----------------------------------------------------------------------// 
void loop() {
  while (BT.available()){  //Check if there is an available byte to read
  delay(10); //Delay added to make thing stable
  char c = BT.read(); //Conduct a serial read
  readdata += c; //build the string- "forward", "reverse", "left" and "right"
  } 
  if (readdata.length() > 0) {
    Serial.println(readdata);
     if(readdata == "len"){
       if(speedA<=245){
           speedA+=10;
        }
      if(speedB<=245){
           speedB+=10;
        }
     }
      if(readdata == "dung"){
         if(speedA>=10){
           speedA-=10;
        }
        if(speedB>=10){
           speedB-=10;
        }
     }
    oldreaddata=readdata;
    readdata="";
  }
  if(oldreaddata == "len")
  {
    digitalWrite(INA, HIGH);
    digitalWrite(INB, LOW);
    digitalWrite(INC,HIGH);
    digitalWrite(IND,LOW);
    analogWrite(SPA, speedA);
    analogWrite(SPB, speedB);
    
    Serial.print("speedA:");
    Serial.println(speedA);
    Serial.print("speedB");
    Serial.println(speedB);
   // digitalWrite(LIGHT_FRONT1, HIGH);
   // digitalWrite(LIGHT_FRONT2, HIGH);
   // digitalWrite(LIGHT_LEFT, LOW);
   // digitalWrite(LIGHT_RIGHT, LOW);
  //  digitalWrite(LIGHT_BACK, LOW);
    delay(100);
  }

  else if(oldreaddata == "lui")
  {
    digitalWrite(INA, LOW);
    digitalWrite(INB, HIGH);
    digitalWrite(INC, LOW);
    digitalWrite(IND,HIGH);
    analogWrite(SPA, speedA);
    analogWrite(SPB, speedB);
   
    Serial.print("speedA:");
    Serial.println(speedA);
    Serial.print("speedB");
    Serial.println(speedB);
    // digitalWrite(LIGHT_FRONT1, LOW);
   // digitalWrite(LIGHT_FRONT2, LOW);
   // digitalWrite(LIGHT_LEFT, LOW);
   // digitalWrite(LIGHT_RIGHT, LOW);
   // digitalWrite(LIGHT_BACK, HIGH);
    delay(100);
  }

  else if (oldreaddata == "phai")
  {
    digitalWrite (INA,HIGH);
    digitalWrite (INB,LOW);
    digitalWrite (INC,LOW);
    digitalWrite (IND,LOW);
     analogWrite(SPA, speedA);
    analogWrite(SPB, speedB);
   // digitalWrite(LIGHT_FRONT1, LOW);
   // digitalWrite(LIGHT_FRONT2, LOW);
   // digitalWrite(LIGHT_LEFT, LOW);
   // digitalWrite(LIGHT_RIGHT, HIGH);
    //digitalWrite(LIGHT_BACK, LOW);
    delay (100);
   
  }

 else if ( oldreaddata == "trai")
 {
   digitalWrite (INA, LOW);
   digitalWrite (INB, LOW);
   digitalWrite (INC, HIGH);
   digitalWrite (IND, LOW);
    analogWrite(SPA, speedA);
    analogWrite(SPB, speedB);
    
  // digitalWrite(LIGHT_FRONT1, LOW);
  // digitalWrite(LIGHT_FRONT2, LOW);
  // digitalWrite(LIGHT_LEFT, HIGH);
  // digitalWrite(LIGHT_RIGHT, LOW);
  // digitalWrite(LIGHT_BACK, LOW);
   delay (100);
 }

 else if (oldreaddata == "dung")
 {
   digitalWrite (INA, LOW);
   digitalWrite (INB, LOW);
   digitalWrite (INC, LOW);
   digitalWrite (IND, LOW);
 
 
  // digitalWrite(LIGHT_FRONT1, LOW);
  // digitalWrite(LIGHT_FRONT2, LOW);
  // digitalWrite(LIGHT_LEFT, LOW);
  // digitalWrite(LIGHT_RIGHT, LOW);
  // digitalWrite(LIGHT_BACK, LOW);
   delay (100);
 }

//readdata="";

} //Reset the variable
