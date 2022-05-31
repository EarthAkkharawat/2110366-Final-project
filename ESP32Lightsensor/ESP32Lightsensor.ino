#include <SparkFunTSL2561.h>
#include <Wire.h>
#include <SoftwareSerial.h>
SoftwareSerial mySerial(4, 5);
#include <Arduino.h>
#if defined(ESP32)
#include <WiFi.h>
#include <FirebaseESP32.h>
#endif
#include <Firebase_ESP_Client.h>

#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>


#define API_KEY "AIzaSyDU3ZLPqguNfSx2TBqNUsPZnsruVY_H3tQ"
#define DATABASE_URL "smartest-light-default-rtdb.firebaseio.com"
/* 4. Define the user Email and password that alreadey registerd or added in your project */
#define USER_EMAIL "akkharawatbct@gmail.com"
#define USER_PASSWORD "earth77"

// Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

SFE_TSL2561 light;

// Global variables:
long lastUART = 0;
boolean gain;     // Gain setting, 0 = X1, 1 = X16;
unsigned int ms;  // Integration ("shutter") time in milliseconds
double lux;
String state = "00";
int buttonState;

void setup()
{
  // Initialize the Serial port:

  Serial.begin(115200);
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

  /* Sign up */
  if (Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("ok");
    signupOK = true;
  }
  else {
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  config.token_status_callback = tokenStatusCallback;

  Firebase.begin(&config, &auth);

  // Comment or pass false value when WiFi reconnection will control by your code or third party library
  Firebase.reconnectWiFi(true);

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
  if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0)) {
    sendDataPrevMillis = millis();
    if (light.getData(data0, data1))
    {
      // Resulting lux value
      boolean good;  // True if neither sensor is saturated

      // Perform lux calculation:

      good = light.getLux(gain, ms, data0, data1, lux);

      // Print out the results:

      Serial.println(lux);
    }
    Serial.print(" lux: ");
    Serial.println(lux);
    // Write an Float number on the database path test/brightness
    if (Firebase.RTDB.setDouble(&fbdo, "test/brightness", lux)) {
      Serial.println("PASSED");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }
    
    if (millis() - lastUART > 1000)
    {
      checkState();
      mySerial.print(state);
      Serial.println(state);
      Serial.println("Commu ok");

      if (Firebase.RTDB.setString(&fbdo, "test/State", state)) {
        Serial.println("PASSED");
        Serial.println("PATH: " + fbdo.dataPath());
        Serial.println("TYPE: " + fbdo.dataType());
      }
      else {
        Serial.println("FAILED");
        Serial.println("REASON: " + fbdo.errorReason());
      }
      lastUART = millis();
    }
  }
}

void checkState() {

  if (Firebase.RTDB.getInt(&fbdo, "/test/buttonState")) {
    if (fbdo.dataType() == "int") {
      buttonState = fbdo.intData();
      Serial.println(buttonState);
    }
  }
  else {
    Serial.println(fbdo.errorReason());
  }

  if (lux > 25.0 && buttonState == 0) {
    state = "00";
  } else if (lux > 25 && buttonState == 1) {
    state == "01";
  } else if (lux <= 25 && buttonState == 0) {
    state == "10";
  } else if (lux <= 25.0 && buttonState == 1) {
    state = "11";
  }
}
