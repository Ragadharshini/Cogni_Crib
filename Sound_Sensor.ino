//Sound Sensor
#define BLYNK_TEMPLATE_NAME "Cogni Crib Part 1"
#define BLYNK_TEMPLATE_ID "TMPL3MakxAO3i"
#define BLYNK_AUTH_TOKEN "LBTQCq_bk2SBOsTAolfB3P8AerQXN9WO"

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

// Blynk credentials
char auth[] = "LBTQCq_bk2SBOsTAolfB3P8AerQXN9WO";
char ssid[] = "SSN"; // Replace with your Wi-Fi SSID
char pass[] = "Ssn1!Som2@Sase3#"; // Replace with your Wi-Fi password

#define SOUND_SENSOR_PIN 32
int soundThreshold = 32; // Adjust the sound threshold

BlynkTimer timer;

void setup() {
  pinMode(SOUND_SENSOR_PIN, INPUT);
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(1000L, checkSoundSensor); // Check sound sensor every 1 second
}

void loop() {
  Blynk.run();
  timer.run();
}

void checkSoundSensor() {
  int soundValue = getSoundSensorValue();

  if (soundValue == 1) {
    Blynk.logEvent("Sound", "Sound Detected");
    Blynk.virtualWrite(V32, "Sound Detected");
  } else {
    Blynk.virtualWrite(V32, "Sound Not Detected");
  }
}

int getSoundSensorValue() {
  unsigned long startMillis = millis();
  float peakToPeak = 0;
  unsigned int signalMax = 0;
  unsigned int signalMin = 1024;

  while (millis() - startMillis < 1000) { // Collect data for 1 second
    int sample = analogRead(SOUND_SENSOR_PIN);
    if (sample < signalMin) {
      signalMin = sample;
    }
    if (sample > signalMax) {
      signalMax = sample;
    }
  }

  peakToPeak = signalMax - signalMin;
  int db = map(peakToPeak, 0, 1023, 30, 90); // Adjust the mapping based on your sensor's characteristics
  Serial.println(db);

  if (db < soundThreshold) {
    Serial.println("Sound Not Detected");
    return 0; // Baby not crying
  } else {
    Serial.println("Sound Detected");
    return 1; // Baby crying
  }
}
