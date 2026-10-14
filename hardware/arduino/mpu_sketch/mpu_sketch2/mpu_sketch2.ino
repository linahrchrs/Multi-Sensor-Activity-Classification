#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

Adafruit_MPU6050 mpu;

void setup() {
  Serial.begin(115200);
  Wire.begin();

  if (!mpu.begin()) {
    Serial.println("ERROR: MPU6050 not found");
    while (1) delay(10);
  }

  delay(200);
}

void loop() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  // CSV format
  Serial.print("ax:"); Serial.print(a.acceleration.x); Serial.print(",");
  Serial.print("ay:"); Serial.print(a.acceleration.y); Serial.print(",");
  Serial.print("az:"); Serial.print(a.acceleration.z); Serial.print(",");
  Serial.print("gx:"); Serial.print(g.gyro.x); Serial.print(",");
  Serial.print("gy:"); Serial.print(g.gyro.y); Serial.print(",");
  Serial.print("gz:"); Serial.print(g.gyro.z);
  Serial.println();

  delay(100);  // 10 Hz
}