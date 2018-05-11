#include <stdio.h>
#include <math.h>

//NCKH2

//const float MaxValue = 3.40282e+038f;

//using namespace std;


  typedef struct {
    float rawValue;
    float FastF;
    float AvgF;
    float SlowF;
    float MaxBound;
    float AvgBound;
    float MinBound;
  }VelocityFuzzy;
  VelocityFuzzy velocity_fuzzy;

float defuzzy_velocity(){
  
  velocity_fuzzy.rawValue=(velocity_fuzzy.FastF * velocity_fuzzy.MaxBound + velocity_fuzzy.SlowF * velocity_fuzzy.MinBound + velocity_fuzzy.AvgF * velocity_fuzzy.AvgBound) / (velocity_fuzzy.FastF + velocity_fuzzy.SlowF + velocity_fuzzy.AvgF);
  return velocity_fuzzy.rawValue;
}

void fuzzy_velocity(){
 
  if (velocity_fuzzy.rawValue <= velocity_fuzzy.MinBound)
            {
                velocity_fuzzy.SlowF=1;
                velocity_fuzzy.FastF = 0;
                velocity_fuzzy.AvgF = 0;
            }
            else if (velocity_fuzzy.rawValue >= velocity_fuzzy.MaxBound)
            {
                velocity_fuzzy.SlowF = 0;
                velocity_fuzzy.FastF = 1;
                velocity_fuzzy.AvgF = 0;
            }
            else if (velocity_fuzzy.MinBound < velocity_fuzzy.rawValue && velocity_fuzzy.rawValue < velocity_fuzzy.AvgBound)
            {
                velocity_fuzzy.SlowF = (velocity_fuzzy.AvgBound - velocity_fuzzy.rawValue) / (velocity_fuzzy.AvgBound - velocity_fuzzy.MinBound);
                velocity_fuzzy.AvgF = (velocity_fuzzy.rawValue - velocity_fuzzy.MinBound) / (velocity_fuzzy.AvgBound - velocity_fuzzy.MinBound);
                velocity_fuzzy.FastF = 0;
            }
            else if (velocity_fuzzy.AvgBound <= velocity_fuzzy.rawValue && velocity_fuzzy.rawValue < velocity_fuzzy.MaxBound)
            {
                velocity_fuzzy.AvgF = (velocity_fuzzy.MaxBound - velocity_fuzzy.rawValue) / (velocity_fuzzy.MaxBound - velocity_fuzzy.AvgBound);
                velocity_fuzzy.FastF = (velocity_fuzzy.rawValue - velocity_fuzzy.AvgBound) / (velocity_fuzzy.MaxBound - velocity_fuzzy.AvgBound);
                velocity_fuzzy.SlowF = 0;
            }

}

typedef struct {
  float rawValue;
  float ShortF;
  float AvgF;
  float LongF;
  float ShortBound;
  float AvgBound;
  float LongBound;
}FrontFuzzy;
FrontFuzzy front_fuzzy;

void fuzzy(){
  if (front_fuzzy.rawValue <= front_fuzzy.ShortBound){
    front_fuzzy.ShortF = 1;
        front_fuzzy.AvgF = 0;
        front_fuzzy.LongF = 0;
  } else if (front_fuzzy.rawValue >= front_fuzzy.LongBound){
    front_fuzzy.ShortF = 0;
        front_fuzzy.AvgF = 0;
        front_fuzzy.LongF = 1;
  } else if (front_fuzzy.ShortBound<front_fuzzy.rawValue&&front_fuzzy.rawValue<front_fuzzy.AvgBound){
    front_fuzzy.ShortF = (front_fuzzy.AvgBound - front_fuzzy.rawValue) / (front_fuzzy.AvgBound - front_fuzzy.ShortBound);
        front_fuzzy.AvgF = (front_fuzzy.rawValue - front_fuzzy.ShortBound) / (front_fuzzy.AvgBound - front_fuzzy.ShortBound);
        front_fuzzy.LongF = 0;
  } else if (front_fuzzy.AvgBound <= front_fuzzy.rawValue && front_fuzzy.rawValue < front_fuzzy.LongBound){
    front_fuzzy.AvgF = (front_fuzzy.LongBound - front_fuzzy.rawValue) / (front_fuzzy.LongBound - front_fuzzy.AvgBound);
        front_fuzzy.LongF = (front_fuzzy.rawValue - front_fuzzy.AvgBound) / (front_fuzzy.LongBound - front_fuzzy.AvgBound);
      front_fuzzy.ShortF = 0;
  }

}

typedef struct {
  float rawValue;
  float ShortF;
  float AvgF;
  float LongF;
  float ShortBound;
  float AvgBound;
  float LongBound;
}LeftFuzzy;
LeftFuzzy left_fuzzy;

void fuzzy_left(){
  if (left_fuzzy.rawValue <= left_fuzzy.ShortBound){
    left_fuzzy.ShortF = 1;
        left_fuzzy.AvgF = 0;
        left_fuzzy.LongF = 0;
  } else if (left_fuzzy.rawValue >= left_fuzzy.LongBound){
    left_fuzzy.ShortF = 0;
        left_fuzzy.AvgF = 0;
        left_fuzzy.LongF = 1;
  }else if (left_fuzzy.ShortBound < left_fuzzy.rawValue && left_fuzzy.rawValue < left_fuzzy.AvgBound){
        left_fuzzy.ShortF = (left_fuzzy.AvgBound - left_fuzzy.rawValue) / (left_fuzzy.AvgBound - left_fuzzy.ShortBound);
        left_fuzzy.AvgF = (left_fuzzy.rawValue - left_fuzzy.ShortBound) / (left_fuzzy.AvgBound - left_fuzzy.ShortBound);
        left_fuzzy.LongF = 0;
    }else if (left_fuzzy.AvgBound <= left_fuzzy.rawValue && left_fuzzy.rawValue < left_fuzzy.LongBound){
      left_fuzzy.AvgF = (left_fuzzy.LongBound- left_fuzzy.rawValue) / (left_fuzzy.LongBound - left_fuzzy.AvgBound);
        left_fuzzy.LongF = (left_fuzzy.rawValue - left_fuzzy.AvgBound) / (left_fuzzy.LongBound - left_fuzzy.AvgBound);
        left_fuzzy.ShortF = 0;
    }
}

typedef struct RightFuzzy{
  float rawValue;
  float ShortF;
  float AvgF;
  float LongF;
  float ShortBound;
  float AvgBound;
  float LongBound;
}RightFuzzy;
RightFuzzy right_fuzzy;

void fuzzy_right(){
  if (right_fuzzy.rawValue <= right_fuzzy.ShortBound){
    right_fuzzy.ShortF = 1;
        right_fuzzy.AvgF = 0;
        right_fuzzy.LongF = 0;
  } else if (right_fuzzy.rawValue >= right_fuzzy.LongBound){
    right_fuzzy.ShortF = 0;
        right_fuzzy.AvgF = 0;
        right_fuzzy.LongF = 1;
  }else if (right_fuzzy.ShortBound < right_fuzzy.rawValue && right_fuzzy.rawValue < right_fuzzy.AvgBound){
        right_fuzzy.ShortF = (right_fuzzy.AvgBound - right_fuzzy.rawValue) / (right_fuzzy.AvgBound - right_fuzzy.ShortBound);
        right_fuzzy.AvgF = (right_fuzzy.rawValue - right_fuzzy.ShortBound) / (right_fuzzy.AvgBound - right_fuzzy.ShortBound);
        right_fuzzy.LongF = 0;
    }else if (right_fuzzy.AvgBound <= right_fuzzy.rawValue && right_fuzzy.rawValue < right_fuzzy.LongBound){
      right_fuzzy.AvgF = (right_fuzzy.LongBound- right_fuzzy.rawValue) / (right_fuzzy.LongBound - right_fuzzy.AvgBound);
        right_fuzzy.LongF = (right_fuzzy.rawValue - right_fuzzy.AvgBound) / (right_fuzzy.LongBound - right_fuzzy.AvgBound);
        right_fuzzy.ShortF = 0;
    }
}

typedef struct {
  float rawValue;
  float LeftF;
  float AvgLeftF;
  float MidF;
  float AvgRightF;
  float RightF;
  float LeftBound;
  float AvgLeftBound;
  float AvgRightBound;
  float RightBound;
}RotationFuzzy;
RotationFuzzy rotation_fuzzy;

float defuzzy_rotation(){
  if(rotation_fuzzy.MidF==1) return 0;
  if (rotation_fuzzy.LeftF != 0 || rotation_fuzzy.RightF != 0||rotation_fuzzy.AvgRightF!=0||rotation_fuzzy.AvgLeftF!=0||rotation_fuzzy.MidF!=0)
            {
                rotation_fuzzy.rawValue = 
                (rotation_fuzzy.LeftF * rotation_fuzzy.LeftBound + rotation_fuzzy.RightF *
                rotation_fuzzy.RightBound+rotation_fuzzy.AvgLeftF*rotation_fuzzy.AvgLeftBound+rotation_fuzzy.AvgRightF*
                rotation_fuzzy.AvgRightBound+rotation_fuzzy.MidF*0)
                / (rotation_fuzzy.LeftF + rotation_fuzzy.RightF+rotation_fuzzy.AvgLeftF+rotation_fuzzy.AvgRightF+rotation_fuzzy.MidF);
            }

            else rotation_fuzzy.rawValue=0;
            return rotation_fuzzy.rawValue;

}

float getMin(float s[],int n) //CHU Y: KHAI BAO MANG DONG**
    {
       float min = 9999;
       for(int i = 0; i < n; i++)
        {
            if ( s[i] < min) min = s[i];
        }
        if (min == 9999) return 0;
        return min;
    }
float getMax(float s[], int n)
    {
        float max = 0;
        for (int i = 0; i < n; i++)
        {
            if (s[i] != 0 && s[i] > max) max = s[i];
        }
        return max;
    }

float rule_for_vout(FrontFuzzy f,RightFuzzy r,LeftFuzzy l,VelocityFuzzy v){
  //VelocityFuzzy Vout;
  float outputVout_Slow[2];
  float outputVout_Fast[1];
  float outputVout_Avg[2];

  //f=Short&l=Short&r=Short->Vout=Slow
  float inputRule1[3];
  inputRule1[0]=f.ShortF;
  inputRule1[1]= l.ShortF;
    inputRule1[2]= r.ShortF;
    outputVout_Slow[0]=getMin(inputRule1,3);
//    printf("outputVout_Slow[0]:%f\n ",outputVout_Slow[0]);

    //f=Long->Vout=Fast f xa -> v nhanh
    float inputRule2[1];
    inputRule2[0] = f.LongF;
    outputVout_Fast[0] = getMin(inputRule2, 1);
//    printf("outputVout_Fast[0]:%f\n ",outputVout_Fast[0]);

    //f=Avg->Vout=Avg
    float inputRule3[1];
    inputRule3[0] = f.AvgF;
    outputVout_Avg[0] = getMin(inputRule3, 1);
//    printf("outputVout_Avg[0]:%f\n ",outputVout_Avg[0]);

    //f=Short->Vout=short-> v cham
    float inputRule4[1];
    inputRule4[0] = f.ShortF;
    outputVout_Slow[1] = getMin(inputRule4, 1);
//    printf(" outputVout_Slow[1]:%f\n ", outputVout_Slow[1]);

    velocity_fuzzy.SlowF=getMax(outputVout_Slow, 2);
    velocity_fuzzy.FastF =getMax(outputVout_Fast,1);
    velocity_fuzzy.AvgF = getMax(outputVout_Avg, 2);

    return defuzzy_velocity();

    //printf("v.SlowF:%f,v.FastF:%f,v.AvgF:%f \n",v.SlowF,v.FastF,v.AvgF);

}

typedef struct {
  float rawValue;
    float LeftF;
    float RightF;
    float MidF;
    float LeftBound;
    float RightBound;
}DirectionTarget;
DirectionTarget direction_target;

float Abs(float a){
  if (a<0){
    a=-a;
  }
  return a;
}
void fuzzy_direction(){
  if (direction_target.rawValue < 0)
            {
                direction_target.LeftF = direction_target.rawValue / direction_target.LeftBound;
                direction_target.RightF = 0;
                direction_target.MidF = 1 - direction_target.LeftF;
            }
            else
            {
                direction_target.LeftF = 0;
                direction_target.RightF = direction_target.rawValue / direction_target.RightBound;
                direction_target.MidF = 1 - direction_target.RightF;
            }
          //  if(Abs(direction_target.rawValue)<1) { direction_target.LeftF = 0;direction_target.RightF = 0;direction_target.MidF = 1; }

}
float defuzzy_direction(){
   if (direction_target.LeftF != 0 || direction_target.RightF != 0||direction_target.MidF!=0)
            {
                direction_target.rawValue = (direction_target.LeftF * direction_target.LeftBound + direction_target.RightF * direction_target.RightBound+direction_target.MidF*0) / (direction_target.LeftF + direction_target.RightF+direction_target.MidF);
            }

            else direction_target.rawValue = 0;
            return direction_target.rawValue;
}
float ruleForRotation(FrontFuzzy f, LeftFuzzy l, RightFuzzy r, VelocityFuzzy v, DirectionTarget target){
    float outRotation_AvgLeft[2];//2 rule
    float outRotation_AvgRight[2];//2 rule
    float outRotation_Left[7];//2 rule
    float outRotation_Right[8];//2 rule
    float outRotation_Mid[2];//2 rule

    //4 luat de dam bao xe luon huong ve dich
    //D=Left&L=Long&&F=Long->Rotation=Left
   
    float inputRule1[3];
    inputRule1[0] = target.LeftF; inputRule1[1] = l.LongF; inputRule1[2] = f.LongF;
    outRotation_Left[0] = getMin(inputRule1, 3);
    printf("outRotation_Left[0]:%f\n",outRotation_Left[0]);

    //D=Right&R=Long&F=Long->Rotation=Right
    float inputRule2[3];
    inputRule2[0] = target.RightF; inputRule2[1] = r.LongF; inputRule2[2] = f.LongF;
    outRotation_Right[0] = getMin(inputRule2, 3);
  printf("outRotation_Right[0]:%f\n",outRotation_Right[0]);

    //D=Left&L=AvgL&F=Long->Rotation=AvgLeft
    float inputRule11[3];
    inputRule11[0] = target.LeftF; inputRule11[1] = l.AvgF; inputRule11[2] = f.LongF;
    outRotation_AvgLeft[0] = getMin(inputRule11, 3);
    printf("outRotation_AvgLeft[0]:%f\n",outRotation_AvgLeft[0]);

    //D=Right&R=AvgR&F=Long->Rotation=AvgRight
    float inputRule21[3];
    inputRule21[0] = target.RightF; inputRule21[1] = r.AvgF; inputRule21[2] = f.LongF;
    outRotation_AvgRight[0] = getMin(inputRule21, 3);
    printf("outRotation_AvgRight[0]:%f\n",outRotation_AvgRight[0]);

  //4 luat de dam bao xe luon re dung huong khi gap vat can
    //f=Avg&D=Left&L=Avg->Rotation=AvgLeft
    float inputRule3[3];
    inputRule3[0] = f.AvgF; inputRule3[1] = target.LeftF; inputRule3[2] = l.AvgF;
    outRotation_AvgLeft[1] = getMin(inputRule3, 3);
  printf("outRotation_AvgLeft[1]:%f\n",outRotation_AvgLeft[1]);

    //f=Avg&D=Right&R=Avg->Rotation=AvgRight
    float inputRule4[3];
    inputRule4[0] = f.AvgF; inputRule4[1] = target.RightF; inputRule4[2] = r.AvgF;
    outRotation_AvgRight[1] = getMin(inputRule4, 3);
  printf("outRotation_AvgRight[1]:%f\n",outRotation_AvgRight[1]);

    //f=Short&L=Avg&D=Left->Rotation=Left
    float inputRule5[3];
    inputRule5[0] = f.ShortF; inputRule5[1] = target.LeftF; inputRule5[2] = l.AvgF;
    outRotation_Left[1] = getMin(inputRule5, 3);
  printf("outRotation_Left[1]:%f\n",outRotation_Left[1]);

    //f=Short&L=Long&D=Left->Rotation=Left
    float inputRule51[3];
    inputRule51[0] = f.ShortF; inputRule51[1] = target.LeftF; inputRule51[2] = l.LongF;
    outRotation_Left[2] = getMin(inputRule51, 3);
    printf("outRotation_Left[2]:%f\n",outRotation_Left[2]);

    //f=Short&R=Avg&D=Right->Rotation=Right
    float inputRule6[3];
    inputRule6[0] = f.ShortF; inputRule6[1] = target.RightF; inputRule6[2] = r.AvgF;
    outRotation_Right[1] = getMin(inputRule6, 3);
  //printf("outRotation_Right[1]:%f\n",outRotation_Right[1]);

    //f=Short&R=Long&D=Right->Rotation=Right
    float inputRule60[3];
    inputRule60[0] = f.ShortF; inputRule60[1] = target.RightF; inputRule60[2] = r.LongF;
    outRotation_Right[2] = getMin(inputRule60, 3);
  printf("outRotation_Right[2]:%f\n",outRotation_Right[2]);

//luat mid
    //f=Short&D=MidF&R=Long&L=Long->Rotation=Right
    float inputRule65[4];
    inputRule65[0] = f.ShortF; inputRule65[1] = target.MidF; inputRule65[2] = r.LongF; inputRule65[3] = l.LongF;
    outRotation_Right[5] = getMin(inputRule65, 4);
  printf("outRotation_Right[5]:%f\n",outRotation_Right[5]);

    //f=Short&D=MidF&R=Long&L=Short->Rotation=Right
    float inputRule61[4];
    inputRule61[0] = f.ShortF; inputRule61[1] = target.MidF; inputRule61[2] = r.LongF; inputRule61[3] = l.ShortF;
    outRotation_Right[3] = getMin(inputRule61, 4);
  printf("outRotation_Right[3]:%f\n",outRotation_Right[3]);

    //f=Avg&D=MidF&R=Long&L=Short->Rotation=Right
    float inputRule62[4];
    inputRule62[0] = f.AvgF; inputRule62[1] = target.MidF; inputRule62[2] = r.LongF; inputRule62[3] = l.ShortF;
    outRotation_Right[4] = getMin(inputRule62, 4);
  printf("outRotation_Right[4]:%f\n",outRotation_Right[4]);

    //f=Short&D=MidF&L=Long&R=Short->Rotation=Left
    float inputRule63[4];
    inputRule63[0] = f.ShortF; inputRule63[1] = target.MidF; inputRule63[2] = l.LongF; inputRule63[3] = r.ShortF;
    outRotation_Left[3] = getMin(inputRule63, 4);
  printf("outRotation_Left[3]:%f\n",outRotation_Left[3]);

    //f=Avg&D=MidF&L=Long&R=Short->Rotation=Left
    float inputRule64[4];
    inputRule64[0] = f.AvgF; inputRule64[1] = target.MidF; inputRule64[2] = l.LongF; inputRule64[3] = r.ShortF;
    outRotation_Left[4] = getMin(inputRule64, 4);
    printf("outRotation_Left[4]:%f\n",outRotation_Left[4]);

    //f=Avg&D=MidF&L=Long&R=Avg->Rotation=Left
    float inputRule66[4];
    inputRule66[0] = f.AvgF; inputRule66[1] = target.MidF; inputRule66[2] = l.LongF; inputRule66[3] = r.AvgF;
    outRotation_Left[5] = getMin(inputRule66, 4);
  printf("outRotation_Left[5]:%f\n",outRotation_Left[5]);
  
  //f=Avg&D=MidF&L=Long&R=Long->Rotation=Right
    float inputRule67[4];
    inputRule67[0] = f.AvgF; inputRule67[1] = target.MidF; inputRule67[2] = l.LongF; inputRule67[3] = r.LongF;
    outRotation_Right[6] = getMin(inputRule67, 4);
  printf("outRotation_Right[6]:%f\n",outRotation_Right[6]);
 //f=Short&D=MidF&L=Long&R=Long->Rotation=Right
    float inputRule68[4];
    inputRule68[0] = f.ShortF; inputRule68[1] = target.MidF; inputRule68[2] = l.LongF; inputRule68[3] = r.LongF;
    outRotation_Right[7] = getMin(inputRule68, 4);
  printf("outRotation_Right[7]:%f\n",outRotation_Right[7]);
    //2 luat lam xe re khi sap va cham hai ben
    //r=Short->Rotation=Left;
    float inputRule7[2];
    inputRule7[0] = r.ShortF;
    outRotation_Left[6] = getMin(inputRule7, 1);
    printf("outRotation_Left[6]:%f\n",outRotation_Left[6]);
    //l=Short->Rotation=Right;
    float inputRule8[2];
    inputRule8[0] = l.ShortF;
    outRotation_Right[6] = getMin(inputRule8, 1);
  printf("outRotation_Right[6]:%f\n",outRotation_Right[6]);

    rotation_fuzzy.AvgRightF = getMax(outRotation_AvgRight, 2);
    rotation_fuzzy.AvgLeftF = getMax(outRotation_AvgLeft, 2);
    rotation_fuzzy.RightF = getMax(outRotation_Right, 7);
    rotation_fuzzy.LeftF = getMax(outRotation_Left, 7);
  rotation_fuzzy.MidF=target.MidF;//luat mid
    printf("%f,%f,%f,%f",rotation_fuzzy.AvgRightF,rotation_fuzzy.AvgLeftF,rotation_fuzzy.RightF,rotation_fuzzy.LeftF);
    defuzzy_rotation();

}
//fron_fuzzy.rawValue,right_fuzzy.rawValue,left_fuzzy.rawValue,speed,direction_target.rawValue
void initControl(float a, float b, float c, float d, float e){ // g?n rawvalue cho c?c gi? tr?
   front_fuzzy.rawValue=a;
   right_fuzzy.rawValue=b;
   left_fuzzy.rawValue=c;
   velocity_fuzzy.rawValue=d;
   direction_target.rawValue=e;
   
}

typedef struct {
  float resultHR;
  float resultVo;
}Result;
Result result;
void calControlValue(){
  
  //  FrontFuzzy f;
 // printf("Nhap f=");
 // scanf("%f",&front_fuzzy.rawValue);
  fuzzy();
  //printf("front_fuzzy.ShortF= %f",front_fuzzy.ShortF);
 // printf("front_fuzzy.AvgF= %f",front_fuzzy.AvgF);
  //printf("front_fuzzy.LongF= %f\n",front_fuzzy.LongF);

//  RightFuzzy r;
  //printf("Nhap r=");
  //scanf("%f",&right_fuzzy.rawValue);
  fuzzy_right();
 // printf("right_fuzzy.ShortF= %f",right_fuzzy.ShortF);
  //printf("right_fuzzy.AvgF= %f",right_fuzzy.AvgF);
  //printf("right_fuzzy.LongF= %f\n",right_fuzzy.LongF);

//  LeftFuzzy l;
  //printf("Nhap l=");
  //scanf("%f",&left_fuzzy.rawValue);
  fuzzy_left();
  //printf("left_fuzzy.ShortF= %f",left_fuzzy.ShortF);
  //printf("left_fuzzy.AvgF= %f",left_fuzzy.AvgF);
  //printf("left_fuzzy.LongF= %f\n",left_fuzzy.LongF);

//  VelocityFuzzy v;
  velocity_fuzzy.rawValue=1;
  fuzzy_velocity();
  fuzzy_direction();
  float speed ;
  //float speed1;
  speed= rule_for_vout(front_fuzzy,right_fuzzy,left_fuzzy,velocity_fuzzy);
  //v.rawValue=speed; //dua ra V
  //printf("Van toc la : %d",speed);

  //DirectionTarget target;
  //printf("Nhap target:");
 // scanf("%f\n",&direction_target.rawValue);
  ruleForRotation(front_fuzzy,left_fuzzy,right_fuzzy,velocity_fuzzy,direction_target);
  //printf("Huong : %d",direction_target.rawValue);
  result.resultHR=rotation_fuzzy.rawValue;  
  result.resultVo=speed; 
  
}
float getHR_fromControl(){
  return result.resultHR;
}

float getVo_fromControl(){
  return result.resultVo;
}
int constructControl(){
  //hang so velocity_fuzzy;
  velocity_fuzzy.MaxBound=1;
  velocity_fuzzy.AvgBound=0.5 ;
  velocity_fuzzy.MinBound=0.2 ;
  //hang so front_fuzzy
  front_fuzzy.ShortBound=0.1;
  front_fuzzy.AvgBound=0.5;
  front_fuzzy.LongBound=1;
  //hang so left_fuzzy
  left_fuzzy.ShortBound=0.1;
  left_fuzzy.AvgBound=0.3;
  left_fuzzy.LongBound=1;
  //hang so right_fuzzy
  right_fuzzy.ShortBound=0.1;
  right_fuzzy.AvgBound=0.3;
  right_fuzzy.LongBound=1;
  //hang so rotation_fuzzy
  rotation_fuzzy.LeftBound=-1;
  rotation_fuzzy.AvgLeftBound=-0.5;
  rotation_fuzzy.AvgRightBound=0.5;
  rotation_fuzzy.RightBound=1;
  //hang so direction_target
  direction_target.LeftBound = -1;
  direction_target.RightBound =1;
  return 0;
}
