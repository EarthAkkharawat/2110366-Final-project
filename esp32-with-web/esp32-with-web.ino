
#include <Arduino.h>
#if defined(ESP32)
#include <WiFi.h>
#include <FirebaseESP32.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#endif
#include <Firebase_ESP_Client.h>

// Provide the token generation process info.
#include <addons/TokenHelper.h>

// Provide the RTDB payload printing info and other helper functions.
#include <addons/RTDBHelper.h>

// Replace with your network credentials
const char* ssid = "jade_Internet";
const char* password = "Jafe1846";


/* 2. Define the API Key */
#define API_KEY "AIzaSyDU3ZLPqguNfSx2TBqNUsPZnsruVY_H3tQ"
//AIzaSyDU3ZLPqguNfSx2TBqNUsPZnsruVY_H3tQ
/* 3. Define the RTDB URL */
#define DATABASE_URL "smartest-light-default-rtdb.firebaseio.com"


/* 4. Define the user Email and password that alreadey registerd or added in your project */
#define USER_EMAIL "akkharawatbct@gmail.com"
#define USER_PASSWORD "earth77"

// Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;


// Set web server port number to 80
//WiFiServer server(80);

// Variable to store the HTTP request
//String header;

// Auxiliar variables to store the current output state
String outputState = "off";

// Assign output variables to GPIO pins
const int output = 26;

// Current time
//unsigned long currentTime = millis();
// Previous time
//unsigned long previousTime = 0;
// Define timeout time in milliseconds (example: 2000ms = 2s)
//const long timeoutTime = 2000;


unsigned long sendDataPrevMillis = 0;
int count = 0;
bool signupOK = false;

int buttonstate;
int turnOFF;
float brig_val;

void setup() {
  Serial.begin(115200);
  // Initialize the output variables as outputs
  pinMode(output, OUTPUT);
  // Set outputs to LOW
  digitalWrite(output, LOW);

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
}

void loop() {

  if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0)) {
    sendDataPrevMillis = millis();
    
    if (Firebase.RTDB.getInt(&fbdo, "/test/buttonState")) {
      if (fbdo.dataType() == "int") {
        buttonstate = fbdo.intData();
        Serial.println(buttonstate);
      }
    }
    else {
      Serial.println(fbdo.errorReason());
    }

    if (Firebase.RTDB.getInt(&fbdo, "/test/turnOFF")) {
      if (fbdo.dataType() == "int") {
        turnOFF = fbdo.intData();
        Serial.println(turnOFF);
      }
    }
    else {
      Serial.println(fbdo.errorReason());
    }

    //Write an Int number on the database path test/int
    if (night && motion detected && buttonstate == 0) {
      if (Firebase.RTDB.setInt(&fbdo, "test/buttonState", 1)) {
        Serial.println("PASSED");
        Serial.println("PATH: " + fbdo.dataPath());
        Serial.println("TYPE: " + fbdo.dataType());
      }
      else {
        Serial.println("FAILED");
        Serial.println("REASON: " + fbdo.errorReason());
      }
    } else if (!night){
      if (Firebase.RTDB.setInt(&fbdo, "test/buttonState", 0)) {
        Serial.println("PASSED");
        Serial.println("PATH: " + fbdo.dataPath());
        Serial.println("TYPE: " + fbdo.dataType());
      }
      else {
        Serial.println("FAILED");
        Serial.println("REASON: " + fbdo.errorReason());
      }
    }else if (night && buttonstate == 1 && turnOFF == 1){
      if (Firebase.RTDB.setInt(&fbdo, "test/buttonState", 0)) {
        Serial.println("PASSED");
        Serial.println("PATH: " + fbdo.dataPath());
        Serial.println("TYPE: " + fbdo.dataType());
      }
      else {
        Serial.println("FAILED");
        Serial.println("REASON: " + fbdo.errorReason());
      }
    }

    // Write an Float number on the database path test/brightness
    if (Firebase.RTDB.setDouble(&fbdo, "test/brightness", 0.01 + random(0, 100))) {
      Serial.println("PASSED");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }
  }
}
