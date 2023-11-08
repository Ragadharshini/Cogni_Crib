#define BLYNK_TEMPLATE_ID "TMPL3MakxAO3i"
#define BLYNK_TEMPLATE_NAME "Cogni Crib Part 1"
#define BLYNK_AUTH_TOKEN "LBTQCq_bk2SBOsTAolfB3P8AerQXN9WO"

#include <ESP32Servo.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

// Blynk credentials
char auth[] = "LBTQCq_bk2SBOsTAolfB3P8AerQXN9WO";
char ssid[] = "Ragadharshini";
char pass[] = "Ragadharshini";

#define SERVO_PIN 2 // Replace with the GPIO pin you've connected to the servo
Servo myservo;    // Create a Servo object

// Virtual pin on the Blynk app
#define VPIN_BUTTON_2   V2

bool servoOn = false;

BlynkTimer timer;

BLYNK_WRITE(VPIN_BUTTON_2) {
  int servoControl = param.asInt();
  if (servoControl == 1) {
    servoOn = true;
  } else {
    servoOn = false;
  }
}

void setup() {
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  myservo.attach(SERVO_PIN);
  timer.setInterval(50L, servoControlLoop);
}

void loop() {
  Blynk.run();
  timer.run();
}

void servoControlLoop() {
  if (servoOn) {
    for (int pos = 0; pos <= 300; pos += 1) {
      myservo.write(pos);
      delay(5);
    }
    for (int pos = 300; pos >= 0; pos -= 1) {
      myservo.write(pos);
      delay(5);
    }
  } else {
    // Set servo to 0 position when "servoOn" becomes false
    myservo.write(100);
  }
}
