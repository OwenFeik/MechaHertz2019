#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_L3GD20.h>
#include <Adafruit_L3GD20_U.h>

/*Initialize gyro component */
Adafruit_L3GD20_Unified gyro = Adafruit_L3GD20_Unified(20);

/*GLOBAL VARIABLES*/
#define TO_DEGREES 57.295779513082 //Coefficient to convert RAD to DEG
float gyroDrift = 0;
float bearing = 0;
float measuredtime, prevtime;
float duration = 0;
float calibratedspeed;
int counter = 0;
float speedsum = 0;

void setup() {
  
  Serial.begin(9600); 
  Serial.println("BOOTING");
  gyro.enableAutoRange(true); //Don't know what this does to be honest
  
  /*INCLUDE THE SEQUENCE BELOW OR IT WONT RUN*/
  if(!gyro.begin()){
    /* There was a problem detecting the L3GD20 ... check your connections */
    Serial.println("Ooops, no L3GD20 detected ... Check your wiring!");
    while(1);
  }
  Serial.println("STARTING GYROSCOPE");
  
  /*SET DELAY >70ms TO GET ACCURATE GYROSCOPE READINGS */
  /*TIME WILL LIKELY BE TAKEN UP BY OTHER CODE */
  delay(100); 

  //Drift calibration stuff, re-calibration might be necessary if the value seems inaccurate or after a while.
  gyroDrift = GyroZSpeed();
  
  //gyroDrift = 0;
  Serial.print("DRIFT:");Serial.println(gyroDrift);
  prevtime = millis();
}

void loop() {
  // put your main code here, to run repeatedly:
  
  //Serial.print("prevtime: ");Serial.println(prevtime);
  /*Do a bunch of code here who knows what's going on */
  //gyroDrift = GyroZSpeed();
  //Serial.print("DRIFT:");Serial.println(gyroDrift);
  

  calibratedspeed = GyroZSpeed() - gyroDrift;
  if (calibratedspeed < 0.15 && calibratedspeed > -0.15){
    calibratedspeed = 0;
  }
  else{
    
  }
  
  speedsum += calibratedspeed;
  
  if (counter == 99){ //effectively loop from 0 - 99 (inclusive)
    bearing += GyroZDistance(speedsum); //multiply by time difference
    prevtime = millis();
    speedsum = 0;
    Serial.print("TOTAL DISTANCE");Serial.println(bearing);
    counter = 0;
  }
  else{
    counter += 1;
  }
}

/* PROCESS HERE IS LARGLEY BASED OFF EXAMPLE CODE */
float GyroZSpeed(){
  sensors_event_t event;
  gyro.getEvent(&event);
  return event.gyro.z;
}
float GyroZDistance(float s){
  /*sensors_event_t event;
  gyro.getEvent(&event);
  Serial.print("SPEED: ");Serial.println(event.gyro.z);
  calibratedspeed = event.gyro.z - gyroDrift;*/

  duration = (millis()-prevtime)/1000; //convert to seconds as this is the units used for instrument
  Serial.print("AVGSPEED: ");Serial.println(s*0.01);
  return s*duration*0.01*TO_DEGREES; //TO_DEGREES is not needed but useful for intuition
  
}

/*PRINTS STUFF OUT, USE FOR DEBUGGING */
void displaySensorDetails(){
  sensor_t sensor;
  gyro.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" rad/s");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" rad/s");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" rad/s");  
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);
}
