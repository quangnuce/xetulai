#include <SoftwareSerial.h>
#include <TinyGPS.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
#include "NCKH.c"

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
TinyGPS gps;
/*
các hằng dùng trong chương trình
- Pin
- Các hệ số, vận tốc quay, tốc độ tối đa, các khoảng cách của cảm biến
*/
#define INA 3
#define INB 4
#define INC 5
#define IND 6
#define SPA 8
#define SPB 9
//const int trig_left = 10, trig_right = 11;     // chân trig của HC-SR04
//const int echo_left = 7, echo_right = 12;     // chân echo của HC-SR04
const int fpin=A0;  //front chân A0 của hồng ngoại
const int lpin=A1;//left
const int rpin=A2;//right 
float FL,FR,FF;           // biến lưu khoảng cách
float V,D,GR,Lat,Lng;
float Vo,Va,Vb,RL,RR,Do,DD,HR;
int state;
float dlat,dlong;
float clat,clong;
float TargetHeading;//heading huong dich (rad)
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
    Serial1.begin(9600);
   // pinMode(trig_left,OUTPUT);    // chân trig sẽ phát tín hiệu
   // pinMode(echo_left,INPUT);     // chân echo sẽ nhận tín hiệu
   // pinMode(trig_right,OUTPUT);
   // pinMode(echo_right,INPUT); 
    pinMode(fpin,INPUT); 
     pinMode(rpin,INPUT); 
      pinMode(lpin,INPUT);  
     // chân của hồng ngoại
    //Serial.println(TinyGPS::library_version());
    Serial.println("Magnetometer Test"); Serial.println("");
  
  /* Initialise the sensor */
    mag.enableAutoRange(true);
    if(!mag.begin())
    {
    /* There was a problem detecting the LSM303 ... check your connections */
      Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    //  while(1);
    }
    lasttime=millis();
        pinMode(2, INPUT_PULLUP); 
    
    pinMode(18, INPUT_PULLUP); 
    //attachInterrupt(0, pulse, LOW); 
   attachInterrupt(digitalPinToInterrupt(2), pulse, FALLING); //Ngắt cạnh xuống // ham pulse chua co
   state=1;
   DD=0;
    dlat=20.941429;
   dlong=106.314871;
   clat=21.003998;//toa do nha A1 -DHXD
   clong=105.841931;
   constructControl();
   pinMode (11,OUTPUT);
   pinMode(12,OUTPUT);
}
//-----------------------------------------------------------------------// 
void loop() {
  //Trạng thái khởi động: State=1
  if(state==1){
    Init();
  }
  else if(state==2){
    //trạng thái vận hành
     Serial.println("State 2 ");
    ThuThapDuLieu();
    DieuKhien();
    VanHanh();
  }

} 
/* các hàm xử lý */
void Init(){
//khởi tạo hệ thống , đợi để các cảm biến cho kết quả đo chính xác (trừ GPS), tính hướng đích Do
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
  thuToaDo();
  thuHuongDi();
 // thuKhoangCach();
  thuVanToc();
  
 // thuGiaTocGoc();
}
void DieuKhien(){
   //2 trường hợp: khi gặp chướng ngại vật và trường hợp còn lại, mỗi trường hợp áp dụng các luật mờ tương ứng, kết quả sau khi giải
  //mờ là vận tốc đích Vo, góc rẽ trái, phải (RL,RR)
  //điều khiển thử nghiệm:
  /*if(FF>1){
    //di thang
    RR=0;RL=0; Vo=1;
  }
  else if(FL>1){
    RR=0;RL=1; Vo=0.5;
  }
  else if(FR>1){
   RR=1;RL=0;Vo=0.5;
  }*/
  //gắn dữ liệu cho NCKH.c
  //float speed;
  Serial.print("DD before control: ");Serial.println(DD);
  Serial.print("front= ");
  Serial.print(FF);
  Serial.println("m");
  Serial.print("left= ");
  Serial.print(FL);
  Serial.println("m");
  Serial.print("right= ");
  Serial.println(FR);
  initControl(FF,FR,FL,Vo,DD);//gán rawvalue cho bên NCKH.c
  
  calControlValue();//hàm bên NKCH.c: tính các RR,RL,Vo và gán cho cấu trúc Result
  HR=getHR_fromControl();//lấy dữ liệu từ cấu trúc Result bên NCKH.c
  Vo=getVo_fromControl();
  Serial.print("HR: ");Serial.println(HR);
  Serial.print("Vo: ");Serial.println(Vo);
  
  
  //RL=getRL_fromControl();
  Vo=getVo_fromControl();
  if(FF<0.30)
      Vo=-1;
}
void VanHanh(){
 //gồm 2 chế độ : chế độ rẽ và chế độ đi thẳng
  //chế độ rẽ (nếu góc RL,RR đủ lớn) : căn cứ trên vận tốc đích Vo và các góc rẽ mà thiết lập điện áp trên 2 bánh
  //chế độ đi thẳng: căn cứ trên gia tốc góc để giữ cho xe chạy thẳng với tốc độ tối đa, sử dụng PCI đơn giản
  //HR = RL - RR;
  //Vo = 130;
   int Vmin=80;
   float absHR=HR<0?-HR:HR;
  if (absHR < 0.05) {
    Va = Vb = V = Vo * (255 - Vmin) + Vmin;
  }
  if (absHR > 0.05) {
    if (HR < 0) {
      Va = Vo * (225 - Vmin) + Vmin;
      Vb = Va * (1 - absHR);
    }
    else if (HR > 0) {
      Vb = Vo * (225 - Vmin) + Vmin;
      Va = Vb * (1 - absHR);
    }
  }
  if(Va<Vb){
    digitalWrite(12,HIGH);
    digitalWrite(11,LOW);
  }
  else{
    digitalWrite(11,HIGH);
    digitalWrite(12,LOW);
  }
  if (absHR < 0.05) {

    digitalWrite(INA, HIGH);
    digitalWrite(INC, HIGH);
    digitalWrite(INB, LOW);
    digitalWrite(IND, LOW);
    analogWrite(SPA, Va);
    analogWrite(SPB, Vb);
    delay(200)
    ;
  }
  if (absHR > 0.05) {

    if (HR < 0) {
      digitalWrite (INC, HIGH);
      digitalWrite (INA, HIGH);
      digitalWrite (INB, LOW);
      digitalWrite (IND, LOW);
      analogWrite(SPA, Va);
      analogWrite(SPB, Vb);
      delay (200);
    }
    else if (HR > 0) {

      digitalWrite (INC, HIGH);
      digitalWrite (INA, HIGH);
      digitalWrite (INB, LOW);
      digitalWrite (IND, LOW);
      analogWrite(SPA, Va);
      analogWrite(SPB, Vb);
      delay (200);
    }
  }
  if(Vo<0){
       digitalWrite (INC, LOW);
      digitalWrite (INA, LOW);
      digitalWrite (INB, HIGH);
      digitalWrite (IND, HIGH);
      analogWrite(SPA, Vmin);
      analogWrite(SPB, Vmin);
      delay (2000);
    }
   
  Serial.print("Va: ");Serial.println(Va);
  Serial.print("Vb: ");Serial.println(Vb);
}
void thuKhoangCach(){
  //thu nhận dữ liệu từ sensor khoảng cách, cho ra các khoảng cách FF,FR,FL tính bằng m
  //  unsigned long duration_left, duration_right; // biến đo thời gian
    
    /* Phát xung từ chân trig */
   // digitalWrite(trig_left,0);   // tắt chân trig
   // delayMicroseconds(2);
    //digitalWrite(trig_left,1);   // phát xung từ chân trig
   // delayMicroseconds(5);   // xung có độ dài 5 microSeconds
   //digitalWrite(trig_left,0);   // tắt chân trig
    
    /* Tính toán thời gian */
    // Đo độ rộng xung HIGH ở chân echo. 
  //  duration_left = pulseIn(echo_left,HIGH);
    // Tính khoảng cách đến vật.
   // FL = (duration_left/2/29.412/100);
    
    //right
  //  digitalWrite(trig_right,0);
  //  delayMicroseconds(2);
   // digitalWrite(trig_right,1);
   // delayMicroseconds(5);
   // digitalWrite(trig_right,0);
   // duration_right = pulseIn(echo_right,HIGH);  
    //FR = (duration_right/2/29.412/100);
    
    //front
    float f;
    f=analogRead(fpin);
    FF=(10650.08 * pow(f,-0.935) - 10)/100;
    //right
    float r;
    r=analogRead(rpin);
    FR=((6762.0/(r-9))-4)/100;
   // FR=(4800.00/(r-20))/100;
     //left
    float l;
    l=analogRead(lpin);
    FL=((6762.0/(l-9))-4)/100;
    /* In kết quả ra Serial Monitor */
   // Serial.print("front= ");
   // Serial.print(FF);
   // Serial.println("m");
   // Serial.print("left= ");
   // Serial.print(FL);
   // Serial.println("m");
   // Serial.print("right= ");
   // Serial.print(FR);
   // Serial.println("m");
    //delay(200);
}
void thuHuongDi(){
  //thu nhận dữ liệu từ la bàn số, tính toán hướng hiện tại D và góc lệch so với hướng đích DD 
  /* Get a new sensor event */ 
 
  sensors_event_t event; 
  mag.getEvent(&event);
  
  float Pi = 3.14159;
  
  // Calculate the angle of the vector y,x
  float heading = (atan2(event.magnetic.y-31.305,event.magnetic.x-233.26) * 180) / Pi;
  //Serial.print("MagX= ");Serial.print(event.magnetic.x-233.26);Serial.print(",MagY= ");Serial.println(event.magnetic.y-31.305);
  // Normalize to 0-360
  //if (heading < 0)
  //{
   // heading = 360 + heading;
  //}
  //tinh chinh ve huong Bac that
  heading=heading+86.75;
  if(heading>180) heading=heading-360;
  Serial.print("Compass Heading: ");
  Serial.println(heading);
  //tinh goc lech so voi goc dich
  float delta=TargetHeading-heading;
  Serial.print("Delta=");Serial.println(delta);
  if(delta>180) delta=-(360-delta);
  if(delta<-180) delta=360+delta;
  DD=delta/180;
  Serial.print("DD=");Serial.println(DD);
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
void pulse() {
   count++;
}
void thuToaDo(){
   //thu nhận tọa độ hiện tại, quy ra kinh độ vĩ độ Lat,Lng
   bool newData = false;
   while (Serial1.available())
    {
      char c = Serial1.read();
      // Serial.write(c); // uncomment this line if you want to see the GPS data flowing
      if (gps.encode(c)) // Did a new valid sentence come in?
        newData = true;
    }
  if (newData)
  {
    float flat, flon;
    unsigned long age;
    gps.f_get_position(&flat, &flon, &age);
   // Serial.print("LAT=");
   // Serial.print(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 21.003998 : flat, 6);
   // Serial.print(" LON=");
   // Serial.print(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 105.841931 : flon, 6);
    clat=flat ==TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat;
    clong=flon ==TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon;
   // Serial.print(" SAT=");
   // Serial.print(gps.satellites() == TinyGPS::GPS_INVALID_SATELLITES ? 0 : gps.satellites());
   // Serial.print(" PREC=");
   // Serial.print(gps.hdop() == TinyGPS::GPS_INVALID_HDOP ? 0 : gps.hdop());
  }
  //tinh goc dich
   float Pi = 3.14159;
   float rclat=clat*Pi/180;
   float rclong=clong*Pi/180;
   float rdlat=dlat*Pi/180;
   float rdlong=dlong*Pi/180;
  // float deltalong=rclong>rdlong?(rclong-rdlong):(rdlong-rclong);
  float deltalong=rdlong-rclong;
   //a - c, b- d
   float X=cos(rdlat)*sin(deltalong);
   float Y=cos(rclat) * sin(rdlat)-sin(rclat) * cos(rdlat) * cos(deltalong);
   TargetHeading=atan2(X,Y)*180/Pi;
 //  Serial.print(sin(deltalong));Serial.print(",");Serial.println(cos(deltalong));
   Serial.print(" TargetHeading=");  
  Serial.println(TargetHeading);
   
}
void thuGiaTocGoc(){
  //thu nhận và tính toán gia tốc góc GR từ cảm biến gia tốc
}
