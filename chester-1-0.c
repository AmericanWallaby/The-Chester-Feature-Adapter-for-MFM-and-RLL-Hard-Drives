// The Chester PCB rev. 1.0
// C code revision 1.0 for Arduino Nano and ATmega328P Compatibles
// Drives MFM/RLL head positioner in proportion with input from system's mass storage status LED via drive's ST-412/506 34 pin access connector.

#define STEP_PIN      7
#define DIR_PIN       6
#define SELECT_PIN    8
#define TRACK0_PIN    5
#define HEAD0_PIN     3
#define HDD_LED_PIN   2        

volatile bool serial_mode = false;
unsigned long last_real_activity = 0;

void setup() {
  pinMode(STEP_PIN,   OUTPUT); digitalWrite(STEP_PIN,   HIGH);
  pinMode(DIR_PIN,    OUTPUT); digitalWrite(DIR_PIN,    HIGH);
  pinMode(SELECT_PIN, OUTPUT); digitalWrite(SELECT_PIN, LOW);
  pinMode(TRACK0_PIN,  INPUT_PULLUP);
  pinMode(HEAD0_PIN,  OUTPUT); digitalWrite(HEAD0_PIN,  LOW);

  pinMode(HDD_LED_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(HDD_LED_PIN), hdd_pulse, CHANGE);

  randomSeed(analogRead(A0) + micros());
  Serial.begin(9600);
  delay(100);
  Serial.println("ST-412 Ready");
}

void loop() {
  if (Serial.available() && !serial_mode) {
    serial_mode = true;
    while (Serial.available()) Serial.read();
  }

  if (serial_mode) {
    // Normal serial control (STNOISE.COM or Linux)
    if (Serial.available()) {
      String cmd = Serial.readStringUntil('\n');
      cmd.trim();
      cmd.toUpperCase();
      if (cmd == "RECAL") recalibrate();
      else if (cmd.startsWith("SEEK ")) parse_and_seek(cmd);
    }
  }
  else {
    if (millis() - last_real_activity < 400) {       
      int steps = random(40, 280);
      bool inward = random(0, 2);
      seek(steps, inward);
      delay(random(30, 120));
    }
  }
}


void hdd_pulse() {
  if (digitalRead(HDD_LED_PIN) == LOW) {           
    last_real_activity = millis();
  }
}

void parse_and_seek(String cmd) {
  int sp = cmd.indexOf(' ', 5);
  if (sp < 0) return;
  int steps = cmd.substring(5, sp).toInt();
  String dir = cmd.substring(sp + 1);
  seek(steps, (dir == "IN"));
}

void seek(int steps, bool inward) {
  if (steps < 1) return;
  digitalWrite(DIR_PIN, inward ? LOW : HIGH);
  for (int i = 0; i < steps; i++) {
    digitalWrite(STEP_PIN, LOW);
    delayMicroseconds(300);
    digitalWrite(STEP_PIN, HIGH);
    delay(0);
  }
}

void recalibrate() {
  digitalWrite(DIR_PIN, HIGH);
  while (digitalRead(TRACK0_PIN) == HIGH) {
    while (digitalRead(4) == HIGH);                // SEEK_COMPLETE (pin 4)
    digitalWrite(STEP_PIN, LOW);  delay(3);
    digitalWrite(STEP_PIN, HIGH); delay(2);
  }
  Serial.println("At Track 0");
}