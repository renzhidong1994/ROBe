//
// SonarSRF
// Arduino Library for controlling SRF sonar sensors
// http://www.arduino.cc/playground/Main/SonarSrf08
//
// MIT License
// Copyright(c) 2009 Zach Foresta
// Copyright(c) 2012 Leo Colombaro
// Copyright(c) 2012 Philipp A. Mohrenweiser
//

#include <Wire.h>
#include <SonarSRF08.h>//sonar
//following are IMU part
#include <LIS3MDL.h>//magnetometer
#include <LSM6.h>//accelerometer

LIS3MDL mag;
LSM6 imu;

#define MAIN_08_ADDRESS (0xF8 >> 1)//address for Sonar but only one Sonar is work as well
SonarSRF08 MainSonar;

// Setup Analogue Gain
// http://www.robot-electronics.co.uk/htm/srf08tech.html section "Analogue Gain"
#define GAIN_REGISTER 0x09
// Setup Range Location
// http://www.robot-electronics.co.uk/htm/srf08tech.html section "Changing the Range"
#define LOCATION_REGISTER 0x8C

char unit = 'c'; // 'i' for inches, 'c' for centimeters, 'm' for micro-seconds
char report[80];
char info[80];

void setup() {
    Serial.begin(9600);

    MainSonar.connect(MAIN_08_ADDRESS, GAIN_REGISTER, LOCATION_REGISTER);
    isConnected("SRF08", MainSonar.getSoft());

     Wire.begin();
  if (!mag.init())
  {
    Serial.println("Failed to detect and initialize magnetometer!");
    while (1);
  }
  mag.enableDefault();
   if (!imu.init())
  {
    Serial.println("Failed to detect and initialize IMU!");
    while (1);
  }
  imu.enableDefault();
}

void loop() {
    float sensorReading = 0;
    sensorReading = MainSonar.getRange(unit);
    distance("sensor", sensorReading);
    
    mag.read();//magnetometer 
  snprintf(report, sizeof(report), "M: %6d %6d %6d",
    mag.m.x, mag.m.y, mag.m.z);
  Serial.println(report);
  delay(500);
  
    imu.read();//IMU

  snprintf(info, sizeof(report), "A: %6d %6d %6d    G: %6d %6d %6d",
    imu.a.x, imu.a.y, imu.a.z,
    imu.g.x, imu.g.y, imu.g.z);
  Serial.println(info);
  delay(100);
}

// Print out distance
void distance(String reference, int sensorReading) {
    Serial.print("Distance from " + reference + ": ");
    Serial.print(sensorReading);
    Serial.println(unit);
}
// Print out do not detected
void isConnected(String reference, int sensorSoft) {
    if (sensorSoft >= 0)
    {
        Serial.print("Sensor " + reference + " connected (");
        Serial.print(sensorSoft);
        Serial.println(")");
    }
    else
    {
        Serial.println("Sensor " + reference + " not detected");
    }
}
