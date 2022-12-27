#include <WiFi.h>
#include <FirebaseESP32.h>
#include <DHT.h>

// Provide the token generation process info
#include <addons/TokenHelper.h> 

// Provide the RTDB payload printing info and other helper functions
#include <addons/RTDBHelper.h>

#define WIFI_SSID "Bklong"
#define WIFI_PASSWORD "k123456789"

#define USER_EMAIL "datkhang@gmail.com"
#define USER_PASSWORD "123456"

#define API_KEY "AIzaSyA6PC-kCy-a7sRMff8UpdsZm-Pkl_nLnxQ"
#define DATABASE_URL "https://iotproject-10806-default-rtdb.firebaseio.com"

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

#define DHTPIN 13
#define DHTTYPE DHT11
#define POWER_PIN 33
#define WATER_SENSOR_SIGNAL_PIN 34

#define LIGHT_PATH "Control/Light"
#define FAN_PATH "Control/Fan"
#define SOUND_PATH "Control/Sound"

#define LIGHT_PORT 27
#define FAN_PORT 26
#define SOUND_PORT 25

#define TEMP_PATH "RealTime/Temp"
#define HUMD_PATH "RealTime/Humd"
#define RAIN_PATH "RealTime/Rain"

DHT dht(DHTPIN, DHTTYPE);

float temperature = 0;
float humidity = 0;
float rainAmount = 0;

// Connects your ESP to the internet using the network credentials provided
void initWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println("OK");
}

// Login Firebase with Email and Password to the Firebase authentication object
void firebaseAuthentication() {
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;

  config.token_status_callback = tokenStatusCallback;  // see addons/TokenHelper.h
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void readSensorAndPush() {
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();
  digitalWrite(POWER_PIN, HIGH);
  delay(10);
  rainAmount = analogRead(WATER_SENSOR_SIGNAL_PIN);
  digitalWrite(POWER_PIN, LOW);
  pushValueToFirebase(humidity, temperature, rainAmount);
}

void pushValueToFirebase(float humd, float temp, float rain) {
  if (Firebase.RTDB.setFloat(&fbdo, TEMP_PATH, temp)) {
    Serial.print("Temp = ");
    Serial.print(temp);
  }
  if (Firebase.RTDB.setFloat(&fbdo, HUMD_PATH, humd)) {
    Serial.print("Humd = ");
    Serial.print(humd);
  }
  if (Firebase.RTDB.setFloat(&fbdo, RAIN_PATH, rain)) {
    Serial.print("Rain = ");
    Serial.print(rain);
  }
  Serial.println();
}

void deviceControl() {
  if (Firebase.RTDB.getInt(&fbdo, LIGHT_PATH)) digitalWrite(LIGHT_PORT, fbdo.intData());
  if (Firebase.RTDB.getInt(&fbdo, FAN_PATH)) digitalWrite(FAN_PORT, fbdo.intData());
  if (Firebase.RTDB.getInt(&fbdo, SOUND_PATH)) digitalWrite(SOUND_PORT, fbdo.intData());
}

void setup() {
  Serial.begin(115200);
  initWiFi();
  firebaseAuthentication();
  pinMode(LIGHT_PORT, OUTPUT);
  pinMode(FAN_PORT, OUTPUT);
  pinMode(SOUND_PORT, OUTPUT);
  dht.begin();
}


void loop() {
  if (Firebase.isTokenExpired()) {
    Firebase.refreshToken(&config);
    Serial.println("Refresh token");
  }
  readSensorAndPush();
  deviceControl();
}