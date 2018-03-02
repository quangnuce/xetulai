#include <SoftwareSerial.h>

/*
các hằng dùng trong chương trình
- Pin
- Các hệ số, vận tốc quay, tốc độ tối đa, các khoảng cách của cảm biến
*/
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
float V,D,FF,FL,FR,GR,Lat,Lng;
float Vo,RL,RR,Do,DD;
int state;

void setup() {
 BT.begin(9600);
 Serial.begin(9600);
 

}
//-----------------------------------------------------------------------// 
void loop() {
  //Trạng thái khởi động: State=1
  if(state==1){
    Init();
  }
  else if(state==2){
    //trạng thái vận hành
    ThuThapDuLieu();
    DieuKhien();
    VanHanh();
  }

} 
/* các hàm xử lý */
void Init(){
//khởi tạo hệ thống , đợi để các cảm biến cho kết quả đo chính xác (trừ GPS), tính hướng đích Do
}
void ThuThapDuLieu(){
  //thu thập dữ liệu từ sensor khoảng cách
  thuKhoangCach();
  thuHuongDi();
  thuKhoangCach();
  thuVanToc();
  thuToaDo();
  thuGiaTocGoc();
}
void DieuKhien(){
  //2 trường hợp: khi gặp chướng ngại vật và trường hợp còn lại, mỗi trường hợp áp dụng các luật mờ tương ứng, kết quả sau khi giải
  //mờ là vận tốc đích Vo, góc rẽ trái, phải (RL,RR)
  //điều khiển thử nghiệm:
  if(FF>1){
    //di thang
    RR=0;RF=0; Vo=1;
  }
  else if(FL>1){
    RR=0;RL=1; Vo=0.5;
  }
  else if(FR>1){
   RR=1;RF=0;Vo=0.5;
  }

}
void VanHanh(){
  //gồm 2 chế độ : chế độ rẽ và chế độ đi thẳng
  //chế độ rẽ (nếu góc RL,RR đủ lớn) : căn cứ trên vận tốc đích Vo và các góc rẽ mà thiết lập điện áp trên 2 bánh
  //chế độ đi thẳng: căn cứ trên gia tốc góc để giữ cho xe chạy thẳng với tốc độ tối đa, sử dụng PCI đơn giản
  //
}
void thuKhoangCach(){
  //thu nhận dữ liệu từ sensor khoảng cách, cho ra các khoảng cách FF,FR,FL tính bằng m
}
void thuHuongDi(){
  //thu nhận dữ liệu từ la bàn số, tính toán hướng hiện tại D và góc lệch so với hướng đích DD 
}
void thuVanToc(){
  //thu nhận tốc độ hiện tại V của xe bằng encoder, quy ra m/s
}
void thuToaDo(){
  //thu nhận tọa độ hiện tại, quy ra kinh độ vĩ độ Lat,Lng
}
void thuGiaTocGoc(){
  //thu nhận và tính toán gia tốc góc GR từ cảm biến gia tốc
}
