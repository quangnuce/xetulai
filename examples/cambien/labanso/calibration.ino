#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>

Adafruit_LSM303_Mag_Unified mag = Adafruit_LSM303_Mag_Unified(12345);



float MagMinX, MagMaxX;
float MagMinY, MagMaxY;


long lastDisplayTime;
int count=0;
void setup(void) 
{
  Serial.begin(9600);
  Serial.println("LSM303 Calibration"); Serial.println("");
  MagMinX=999999;
  MagMaxX=-999999;
  MagMinY=999999;
   MagMaxY=-999999;
  /* Initialise the accelerometer */

  /* Initialise the magnetometer */
  if(!mag.begin())
  {
    /* There was a problem detecting the LSM303 ... check your connections */
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    while(1);
  }
  lastDisplayTime = millis();
}

void loop(void) 
{
  /* Get a new sensor event */ 

  sensors_event_t magEvent; 
  
 
  mag.getEvent(&magEvent);
 
 
  
      if (magEvent.magnetic.x < MagMinX) MagMinX = magEvent.magnetic.x;
  if (magEvent.magnetic.x > MagMaxX) MagMaxX = magEvent.magnetic.x;
  
  if (magEvent.magnetic.y < MagMinY) MagMinY = magEvent.magnetic.y;
  if (magEvent.magnetic.y > MagMaxY) MagMaxY = magEvent.magnetic.y;

    
  



  if ((millis() - lastDisplayTime) > 1000)  // display once/second
  {

    Serial.print("Mag Minimums: "); Serial.print(MagMinX); Serial.print("  ");Serial.print(MagMinY); Serial.print("  ");
    Serial.print("Mag Maximums: "); Serial.print(MagMaxX); Serial.print("  ");Serial.print(MagMaxY); Serial.print("  ");  Serial.println();
    lastDisplayTime = millis();
  }
}
