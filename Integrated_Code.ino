#define BLYNK_TEMPLATE_NAME "Cogni Crib Part 1"
#define BLYNK_TEMPLATE_ID "TMPL3MakxAO3i"
#define BLYNK_AUTH_TOKEN "LBTQCq_bk2SBOsTAolfB3P8AerQXN9WO"

#include <WiFiClient.h>
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <ESP32Servo.h>
#include <WiFiUdp.h>

char auth[] = "LBTQCq_bk2SBOsTAolfB3P8AerQXN9WO";
char ssid[] = "SSN";
char pass[] = "Ssn1!Som2@Sase3#";

#define Wetness_Sensor 25
#define Sound_Sensor 32
#define Servo_Motor 2

int Wetness_Sensor_Value = 0;
bool isconnected = false;
bool servoOn = false;
int soundThreshold = 32;

Servo myservo;
BlynkTimer timer;
WiFiUDP udp;

// Virtual Pin numbers for Blynk
#define VPIN_BUTTON_25 V25
#define VPIN_BUTTON_2 V2
#define VPIN_BUTTON_32 V32

bool music_playing = false;

void checkBlynkStatus() {
  isconnected = Blynk.connected();
  if (isconnected == true) {
    sendData();
    Serial.println("Blynk Connected");
  } else {
    Serial.println("Blynk Not Connected");
  }
}

void getSensorData() {
  Wetness_Sensor_Value = digitalRead(Wetness_Sensor);
  Serial.println("Wetness Value: " + String(Wetness_Sensor_Value));
  if (Wetness_Sensor_Value == 0) {
    Serial.println("Wetness Detected");
  } else {
    Serial.println("Wetness Not Detected");
  }

  int soundValue = getSoundSensorValue();

  if (soundValue == 1) {
    Blynk.logEvent("Sound", "Sound Detected");
    Blynk.virtualWrite(V32, "Sound Detected");
    udp.beginPacket("10.123.35.137", 12345); // Replace with your PC's IP address and port
    udp.print("Sound Detected");
    udp.endPacket();
    servoOn = true;
    music_playing = true;
    servoControlLoop();  // Call the servo control function
  } else {
    Blynk.virtualWrite(V32, "Sound Not Detected");
    udp.beginPacket("10.123.35.137", 12345); // Replace with your PC's IP address and port
    udp.print("Sound Not Detected");
    udp.endPacket();
    servoOn = false;
    servoControlLoop();
    music_playing = false;
  }

  delay(3000);
}

void sendData() {
  if (Wetness_Sensor_Value == 0) {
    Blynk.logEvent("Wetness", "Wetness Detected");
    Blynk.virtualWrite(V25, "Wetness Detected");
  } else {
    Blynk.virtualWrite(V25, "No Wetness Detected.");
  }
  Blynk.virtualWrite(V2, servoOn);
}

int getSoundSensorValue() {
  unsigned long startMillis = millis();
  float peakToPeak = 0;
  unsigned int signalMax = 0;
  unsigned int signalMin = 1024;

  while (millis() - startMillis < 1000) {
    int sample = analogRead(Sound_Sensor);
    if (sample < signalMin) {
      signalMin = sample;
    }
    if (sample > signalMax) {
      signalMax = sample;
    }
  }

  peakToPeak = signalMax - signalMin;
  int db = map(peakToPeak, 0, 1023, 30, 90);
  Serial.print("Sound value: ");
  Serial.print(db);
  Serial.print(" db");
  Serial.println();

  if (db < soundThreshold) {
    Serial.println("Sound Not Detected");
    return 0;
  } else {
    Serial.println("Sound Detected");
    return 1;
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(Wetness_Sensor, INPUT);
  // Initialize the servo
  myservo.attach(Servo_Motor, 1000, 2000);  // Attach the servo to pin 2
  WiFi.begin(ssid, pass);
  timer.setInterval(5000L, checkBlynkStatus);
  Blynk.config(auth);
  delay(2000);
}

void loop() {
  getSensorData();
  Blynk.run();
  timer.run();
  servoControlLoop();  // Comment this line to avoid continuous servo movement
}

void servoControlLoop() {
  if (servoOn) {
    for (int pos = 0; pos <= 300; pos += 1) {
      myservo.write(pos);
      delay(4);  // Adjust the delay for smooth servo movement
    }
    for (int pos = 300; pos >= 0; pos -= 1) {
      myservo.write(pos);
      delay(4); // Adjust the delay for servo speed
    }
  }
 else {
    myservo.write(100);  // Adjust the default position if needed
  }
}

BLYNK_WRITE(V2) {
  int servoControl = param.asInt();
  if (servoControl == 1) {
    servoOn = true;
  } else {
    servoOn = false;
    // Uncomment the following line to reset the servo position when servo is turned off
    myservo.write(100);
  }
}