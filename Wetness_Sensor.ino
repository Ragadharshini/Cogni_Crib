
//Wetness Sensor

/* Fill-in your Template ID (only if using Blynk.Cloud) */
#define BLYNK_TEMPLATE_ID "TMPL3MakxAO3i"
#define BLYNK_TEMPLATE_NAME "Cogni Crib Part 1"
#define BLYNK_AUTH_TOKEN "LBTQCq_bk2SBOsTAolfB3P8AerQXN9WO"

// Your WiFi Credentials.
  // Set password to "" for open networks.
  char ssid[] = "SSN";
  char pass[] = "Ssn1!Som2@Sase3#";

// define the GPIO connected with Sensors & LEDs

#define Wetness_Sensor 25

//#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
BlynkTimer timer;

int Wetness_Sensor_Value = 0;
bool isconnected = false;
char auth[] = BLYNK_AUTH_TOKEN;

#define VPIN_BUTTON_25    V25

void checkBlynkStatus() { // called every 2 seconds by SimpleTimer
  isconnected = Blynk.connected();
  if (isconnected == true) {
    sendData();
    Serial.println("Blynk Connected");
  }
  else{
    Serial.println("Blynk Not Connected");
  }
}

void getSensorData()
{
  
  Wetness_Sensor_Value = digitalRead(Wetness_Sensor);
  Serial.println("Wetness Value : " + String(Wetness_Sensor_Value));
  if (Wetness_Sensor_Value == 0 ){
    Serial.println("Wetness Detected");
  }
  else{
    Serial.println("Wetness Not Detected");
  }
  delay(5000);
}

void sendData()
{  
  if (Wetness_Sensor_Value == 0 )
  {
    Blynk.logEvent("Wetness", "Wetness Detected");
    Blynk.virtualWrite(VPIN_BUTTON_25, "Wetness Detected");
  }
  else if (Wetness_Sensor_Value == 1 )
  {
    
    Blynk.virtualWrite(VPIN_BUTTON_25, "No Wetness Detected.");
  } 
  
}

void setup()
{
  Serial.begin(9600);
 
  pinMode(Wetness_Sensor, INPUT);
  
  WiFi.begin(ssid, pass);
  timer.setInterval(5000L, checkBlynkStatus); // check if Blynk server is connected every 2 seconds
  Blynk.config(auth);
  delay(5000);
}

void loop()
{
  getSensorData();
  Blynk.run();
  timer.run();
}
