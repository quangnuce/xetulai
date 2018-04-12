#include <SoftwareSerial.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
Adafruit_LSM303_Mag_Unified mag = Adafruit_LSM303_Mag_Unified(12345);
 /*
 https://learn.adafruit.com/lsm303-accelerometer-slash-compass-breakout/coding
 Library:https://github.com/adafruit/Adafruit_LSM303DLHC
 wiring
  3.3v-3.3v
  GND-GND
  SCL-A5 (Uno)- SCL (Mega)
  SDA-A4 (Uno)- SDA (Mega)
   */
long lasttime;
volatile int count = 0;

/*
các hằng dùng trong chương trình
- Pin
- Các hệ số, vận tốc quay, tốc độ tối đa, các khoảng cách của cảm biến
*/
#define INA 3
#define INB 4
#define INC 5
#define IND 6



 #define SPA 9
 #define SPB 8
const int trig_left = 10, trig_right = 11;     // chân trig của HC-SR04
const int echo_left = 7, echo_right = 12;     // chân echo của HC-SR04
const int redpin=0;                          // chân A0 của hồng ngoại
float FL,FR,FF;           // biến lưu khoảng cách
float V,D,GR,Lat,Lng;
float Vo,Va,Vb,RL,RR,Do,DD,HR;
 float heading;
float orginalDirection;

int state;

/*
Các cấu trúc
các biến số:
Các biến có được từ cảm biến:
- vận tốc hiện tại: V
- hướng hiện tại của xe (độ lệch so với hướng Nam): D
- khoảng cách đến chướng ngại vật: trước (FF), trái(FL), phải(FR)
- gia tốc góc (GR)
- tọa độ (Lat,Lng)
Các biến tính toán, trung gian:
- Vận tốc đích: Vo
- góc rẽ trái, phải:  RL, RR
- hướng đến đích : Do
- Góc lệch so với đích: DD
- Trạng thái: state =1,2,3,...
- Cò thông báo: Đang rẽ (rotating), gặp chướng ngại vật (Ob)
*/
void setup() 
{
    Serial.begin(9600);
    pinMode(trig_left,OUTPUT);    // chân trig sẽ phát tín hiệu
    pinMode(echo_left,INPUT);     // chân echo sẽ nhận tín hiệu
    pinMode(trig_right,OUTPUT);
    pinMode(echo_right,INPUT); 
    pinMode(redpin,OUTPUT);       // chân của hồng ngoại
   
    Serial.println("Magnetometer Test"); Serial.println("");
  
  /* Initialise the sensor */
    if(!mag.begin())
    {
    /* There was a problem detecting the LSM303 ... check your connections */
      Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    //  while(1);
    }
    lasttime=millis();
        pinMode(2, INPUT_PULLUP); 
    
    //pinMode(18, INPUT_PULLUP); 
    //attachInterrupt(0, pulse, LOW); 
    pinMode(INA, OUTPUT);
    pinMode(INB, OUTPUT);
    pinMode(INC, OUTPUT);
    pinMode(IND, OUTPUT);
    pinMode(SPA,OUTPUT);
    pinMode(SPB,OUTPUT);
    
    attachInterrupt(digitalPinToInterrupt(2), pulse, FALLING); //Ngắt cạnh xuống
   state=1;

}
//-----------------------------------------------------------------------// 
void loop() {
  //Trạng thái khởi động: State=1
  if(state==1){
    Init();
  }
  else if(state==2){
    //trạng thái vận hành
     Serial.print("State 2 ");
    ThuThapDuLieu();
     Serial.println("Thu thap du lieu xong");
    DieuKhien();
    VanHanh();
  }

} 
/* các hàm xử lý */
void Init(){
//khởi tạo hệ thống , đợi để các cảm biến cho kết quả đo chính xác (trừ GPS), tính hướng đích Do

 orginalDirection=thuHuongDi();
 if(!mag.begin())
    {
    /* There was a problem detecting the LSM303 ... check your connections */
      Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
     // while(1);
    }
    else{
      state=2;
      }
  
}
void ThuThapDuLieu(){
  //thu thập dữ liệu từ sensor khoảng cách
  thuKhoangCach();
  thuHuongDi();
 // thuKhoangCach();
  //thuVanToc();
 // thuToaDo();
 // thuGiaTocGoc();
}
void DieuKhien(){
  //2 trường hợp: khi gặp chướng ngại vật và trường hợp còn lại, mỗi trường hợp áp dụng các luật mờ tương ứng, kết quả sau khi giải
  //mờ là vận tốc đích Vo, góc rẽ trái, phải (RL,RR)
  //điều khiển thử nghiệm:
  if(FF>0.6){
    //di thang
    RR=0;RL=0; Vo=0.2;
  }
  else if(FL>1){
    RR=0;RL=1; Vo=0.1;
  }
  else if(FR>1){
   RR=1;RL=0;Vo=0.1;
  }
  if(FF<0.15)
      Vo=-1;
  if(FF>0.6&&FR>1&&FL>1){
       
  }
  Serial.print("DeltaDirection= ");
  Serial.println(heading-orginalDirection);
   Serial.print("RR= ");
    Serial.println(RR);
     Serial.print("RL= ");
    Serial.println(RL);
     Serial.print("Vo= ");
    Serial.println(Vo);
}
void VanHanh(){
  //gồm 2 chế độ : chế độ rẽ và chế độ đi thẳng
  //chế độ rẽ (nếu góc RL,RR đủ lớn) : căn cứ trên vận tốc đích Vo và các góc rẽ mà thiết lập điện áp trên 2 bánh
  //chế độ đi thẳng: căn cứ trên gia tốc góc để giữ cho xe chạy thẳng với tốc độ tối đa, sử dụng PCI đơn giản
HR = RL - RR;
  //Vo = 130;
  int Vmin=80;
  if (abs(HR) < 0.05) {
    Va = Vb = V = Vo * (255 - Vmin) + Vmin;
  }
  if (abs(HR) > 0.05) {
    if (HR < 0) {
      Va = Vo * (225 - Vmin) + Vmin;
      Vb = Va * (1 - abs(HR));
    }
    else if (HR > 0) {
      Vb = Vo * (225 - Vmin) + Vmin;
      Va = Vb * (1 - abs(HR));
    }
  }
  if (abs(HR) < 0.05) {

    digitalWrite(INA, HIGH);
    digitalWrite(INC, HIGH);
    digitalWrite(INB, LOW);
    digitalWrite(IND, LOW);
    analogWrite(SPA, Va);
    analogWrite(SPB, Vb);
    delay(100)
    ;
  }
  if (abs(HR) > 0.05) {

    if (HR < 0) {
      digitalWrite (INC, HIGH);
      digitalWrite (INA, HIGH);
      digitalWrite (INB, LOW);
      digitalWrite (IND, LOW);
      analogWrite(SPA, Va);
      analogWrite(SPB, Vb);
      delay (100);
    }
    else if (HR > 0) {

      digitalWrite (INC, HIGH);
      digitalWrite (INA, HIGH);
      digitalWrite (INB, LOW);
      digitalWrite (IND, LOW);
      analogWrite(SPA, Va);
      analogWrite(SPB, Vb);
      delay (100);
    }
  }
  if(Vo<0){
       digitalWrite (INC, LOW);
      digitalWrite (INA, LOW);
      digitalWrite (INB, HIGH);
      digitalWrite (IND, HIGH);
      analogWrite(SPA, Vmin);
      analogWrite(SPB, Vmin);
      delay (100);
    }
    
     Serial.print("HR= ");
    Serial.println(HR);
     Serial.print("Va= ");
    Serial.println(Va);
     Serial.print("Vb= ");
    Serial.println(Vb);
    
}
void thuKhoangCach(){
  //thu nhận dữ liệu từ sensor khoảng cách, cho ra các khoảng cách FF,FR,FL tính bằng m
    unsigned long duration_left, duration_right; // biến đo thời gian
    
    /* Phát xung từ chân trig */
    digitalWrite(trig_left,0);   // tắt chân trig
    delayMicroseconds(2);
    digitalWrite(trig_left,1);   // phát xung từ chân trig
    delayMicroseconds(5);   // xung có độ dài 5 microSeconds
    digitalWrite(trig_left,0);   // tắt chân trig
    
    /* Tính toán thời gian */
    // Đo độ rộng xung HIGH ở chân echo. 
    duration_left = pulseIn(echo_left,HIGH);
    // Tính khoảng cách đến vật.
    FL = (duration_left/2/29.412/100);
    
    //right
    digitalWrite(trig_right,0);
    delayMicroseconds(2);
    digitalWrite(trig_right,1);
    delayMicroseconds(5);
    digitalWrite(trig_right,0);
    duration_right = pulseIn(echo_right,HIGH);  
    FR = (duration_right/2/29.412/100);
    
    //front
    int i;
    i=analogRead(redpin);
    FF=((float)6762/(i-9))-4/100;
    FF=(float)FF/100;
    /* In kết quả ra Serial Monitor */
    Serial.print("front= ");
    Serial.print(FF);
    Serial.println("m");
    Serial.print("left= ");
    Serial.print(FL);
    Serial.println("m");
    Serial.print("right= ");
    Serial.print(FR);
    Serial.println("m");
    delay(200);
}
float thuHuongDi(){
  //thu nhận dữ liệu từ la bàn số, tính toán hướng hiện tại D và góc lệch so với hướng đích DD 
  /* Get a new sensor event */ 
   Serial.print("Compass Heading: ");
  sensors_event_t event; 
  mag.getEvent(&event);
  
  float Pi = 3.14159f;
  
  // Calculate the angle of the vector y,x
  heading = (atan2(event.magnetic.y,event.magnetic.x) * 180) / Pi;
  
  // Normalize to 0-360
  if (heading < 0)
  {
    heading = 360 + heading;
  }
  Serial.print("Compass Heading: ");
  Serial.println(heading);
  return heading;
  //delay(500);
}
void thuVanToc(){
  //thu nhận tốc độ hiện tại V của xe bằng encoder, quy ra m/s
  float n=(float)count/20;

  //float v=1000/(millis()-lasttime)*n;
  float v=20/(100*(millis()-lasttime))*n;
  lasttime=millis();
  Serial.println(count);
  count=0;
  Serial.println(v);
}
void thuToaDo(){
  //thu nhận tọa độ hiện tại, quy ra kinh độ vĩ độ Lat,Lng
}
void thuGiaTocGoc(){
  //thu nhận và tính toán gia tốc góc GR từ cảm biến gia tốc
}
void pulse() {
   count++;
}
