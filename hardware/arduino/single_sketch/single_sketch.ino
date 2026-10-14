#include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// ---------- TEMP ----------
#define ONE_WIRE_BUS 2
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature tempSensors(&oneWire);

// ---------- EMG ----------
const int emgPin = A0;
const int N = 5;
int buffer[N];
int indexBuf = 0;
int baseline = 60;
int threshold = 20;

// ---------- IMU ----------
const int MPU = 0x68;

void setup() {
  Serial.begin(9600);

  // Temp
  tempSensors.begin();

  // EMG buffer init
  for (int i = 0; i < N; i++) {
    buffer[i] = 0;
  }

  // IMU init
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);   // power management register
  Wire.write(0);      // wake up MPU6050
  Wire.endTransmission(true);
}

void loop() {
  // -------- TEMP --------
  tempSensors.requestTemperatures();
  float tempC = tempSensors.getTempCByIndex(0);

  // -------- EMG --------
  int raw = analogRead(emgPin);
  int centered = raw - baseline;
  int rectified = abs(centered);

  buffer[indexBuf] = rectified;
  indexBuf = (indexBuf + 1) % N;

  long sum = 0;
  for (int i = 0; i < N; i++) {
    sum += buffer[i];
  }

  int envelope = sum / N;
  int emgState = (envelope > threshold) ? 1 : 0;

  // -------- IMU --------
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);   // starting register for accelerometer
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 6, true);

  int ax = 0, ay = 0, az = 0;
  if (Wire.available() >= 6) {
    ax = Wire.read() << 8 | Wire.read();
    ay = Wire.read() << 8 | Wire.read();
    az = Wire.read() << 8 | Wire.read();
  }

  // -------- ONE SYNCHRONIZED OUTPUT LINE --------
  Serial.print("TEMP:");
  if (tempC != DEVICE_DISCONNECTED_C) {
    Serial.print(tempC);
  } else {
    Serial.print("NaN");
  }

  Serial.print(",EMG_RAW:");
  Serial.print(raw);

  Serial.print(",EMG_ENV:");
  Serial.print(envelope);

  Serial.print(",EMG_STATE:");
  Serial.print(emgState);

  Serial.print(",AX:");
  Serial.print(ax);

  Serial.print(",AY:");
  Serial.print(ay);

  Serial.print(",AZ:");
  Serial.println(az);

  delay(1000);
}