#include <SparkFunTSL2561.h>
#include <Wire.h>
#include <SoftwareSerial.h>
SoftwareSerial mySerial(4, 5);
#include <Arduino.h>
//#include <FirebaseArduino.h>
#if defined(ESP32)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif
#include <Firebase_ESP_Client.h>

#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>

const char* ssid = "jade_Internet";
const char* password = "Jafe1846";


#define API_KEY "AIzaSyDU3ZLPqguNfSx2TBqNUsPZnsruVY_H3tQ"
#define DATABASE_URL "smartest-light-default-rtdb.firebaseio.com"
//#define FIREBASE_HOST "smartest-light.firebaseapp.com"
//#define FIREBASE_AUTH "Ov3ghk8E1TcepwCTwBTKVnbz3Xl1"
/* 4. Define the user Email and password that alreadey registerd or added in your project */
#define USER_EMAIL "akkharawatbct@gmail.com"
#define USER_PASSWORD "earth77"

// Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

SFE_TSL2561 light;

// Global variables:
unsigned long sendDataPrevMillis = 0;
int count = 0;
bool signupOK = false;

long lastUART = 0;
boolean gain;     
unsigned int ms;  
double lux = 0.0;

String state = "00";
int buttonState=0;
void checkState();

void setup()
{
  // Initialize the Serial port:

  Serial.begin(9600);
  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  //  server.begin();
  Serial.println();
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;

  // Assign the user sign in credentials
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  /* Sign up */
    if (Firebase.signUp(&config, &auth, "", "")) {
      Serial.println("ok");
      signupOK = true;
    }
    else {
      Serial.printf("%s\n", config.signer.signupError.message.c_str());
    }

    Firebase.begin(&config, &auth);

  Firebase.setDoubleDigits(3);

  Serial.println("TSL2561 example sketch");

  light.begin();
  gain = 0;
  unsigned char time = 2;
  light.setTiming(gain, time, ms);
  light.setPowerUp();
  //Communication
  mySerial.begin(115200);

  Serial.println("UART Start");

  lastUART = millis();
}

void loop()
{
  delay(ms);

  unsigned int data0, data1;
  bool a,b,c,d;
  a = Firebase.ready();
  Serial.println(a);
  b = signupOK = 1;
  Serial.println(b);
  c = (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0);
  Serial.println(c);
  
  if (Firebase.ready() && signupOK) {
    sendDataPrevMillis = millis();
    if (light.getData(data0, data1))
    {
      // Resulting lux value
      boolean good;  // True if neither sensor is saturated

      // Perform lux calculation:
      good = light.getLux(gain, ms, data0, data1, lux);
    }
    Serial.print(" lux: ");
    Serial.println(lux);
    // Write an Float number on the database path test/brightness
    if (Firebase.RTDB.setDouble(&fbdo, "test/brightness", lux)) {
//      Serial.println("PASSED");
//      Serial.println("PATH: " + fbdo.dataPath());
//      Serial.println("TYPE: " + fbdo.dataType());
    }
    else {
//      Serial.println("FAILED");
//      Serial.println("REASON: " + fbdo.errorReason());
    }

    if (millis() - lastUART > 400)
    {
      checkState();
      
      mySerial.print(state);
      Serial.println(state);
      Serial.println("Commu ok");

      if (Firebase.RTDB.setString(&fbdo, "test/State", state)) {
//        Serial.println("PASSED");
//        Serial.println("PATH: " + fbdo.dataPath());
//        Serial.println("TYPE: " + fbdo.dataType());
      }
      else {
//        Serial.println("FAILED");
//        Serial.println("REASON: " + fbdo.errorReason());
      }
      lastUART = millis();
    }
  }
}

void checkState() {

  if (Firebase.RTDB.getInt(&fbdo, "/test/buttonState")) {
    if (fbdo.dataType() == "int") {
      buttonState = fbdo.intData();
    }
  }
  else {
//    Serial.println(fbdo.errorReason());
  }

  if (lux > 25.0 && buttonState == 0) {
    Serial.println("xxxx");
    state = "00";
  } else if (lux > 25.0 && buttonState == 1) {
    Serial.println("xtxx");
    state = "01";
  } else if (lux <= 25.0 && buttonState == 0) {
    state = "10";
  } else if (lux <= 25.0 && buttonState == 1) {
    state = "11";
  }
}
