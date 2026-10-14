int emgPin = A0;

const int N = 10;
int buffer[N];
int indexBuf = 0;

int baseline = 43;

void setup() {
  Serial.begin(9600);
}

void loop() {
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

  int threshold = 15;
  int state = (envelope > threshold) ? 1 : 0;

  Serial.print(raw);
  Serial.print(",");
  Serial.print(envelope);
  Serial.print(",");
  Serial.println(state);

  delay(1000);
}