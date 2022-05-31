
#if defined(ESP32)
#include <WiFi.h>
#include <FirebaseESP32.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#endif

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
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Auxiliar variables to store the current output state
String outputState = "off";

// Assign output variables to GPIO pins
const int output = 26;

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0;
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

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
  server.begin();
  Serial.println();

  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);


  Firebase.begin(&config, &auth);

  // Comment or pass false value when WiFi reconnection will control by your code or third party library
  Firebase.reconnectWiFi(true);

  Firebase.setDoubleDigits(5);
}

void loop() {
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected() && currentTime - previousTime <= timeoutTime) {  // loop while the client's connected
      currentTime = millis();
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            // turns the GPIOs on and off
            if (header.indexOf("GET /on") >= 0) {
              // if (value from light sensor > xxx) {
              Serial.println("LED on");
              outputState = "on";
              //digitalWrite(output26, HIGH); send signal to STM32 to toggle LED high
            } else if (header.indexOf("GET /off") >= 0) {
              Serial.println("LED off");
              outputState = "off";
              //digitalWrite(output, LOW); send signal to STM32 to toggle LED low      

            // Display the HTML web page **********************************************************************************************************************************************
            client.println("<!DOCTYPE html><html>");
            client.println("<meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\" /> ");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\" /");
            client.println("<link rel = \"icon\" href = \"image/lightbub.png\"/>");
            client.println("<meta charset = \"UTF-8\"/> ");
            client.println("<title>Smartest Light</title >");
            client.println("<style>html {font-family: \"Inter\"; color: rgb(255, 255, 255); background-color: #000000; display: block; align-items: center; text-align: center;}");
            client.println(".about {margin: auto;width: 50%;padding: 10px;}");
            client.println(".name {text-align: center;font-size: 30px;}");
            client.println(".about {border: 3px solid rgb(255, 113, 113);padding: 2rem;padding-top: 0.5rem; width: 40rem;}");
            client.println("#list {font-size: 20px;text-align: left;}");
            client.println(".dot {height: 100px;width: 100px;background-color: rgb(102, 238, 120);border-radius: 50%;display: inline-block;justify-content: center;text-align: center;align-items: center;padding: 2rem;}");
            client.println(".dotready {height: 100px;width: 100px;background-color: rgb(255, 238, 0);border-radius: 50%;display: inline-block;justify-content: center;text-align: center;align-items: center;padding: 2rem;margin-top: 2rem;}");
            client.println(".main-func {display: inline-block; margin-top: 4rem;align-items: center;text-align: center; justify-content: center;}");
//            client.println(".Lightbulb {font-size: 12.5px;border: 3px solid rgb(255, 113, 113);padding: 2rem;display: inline-block;justify-content: center;width: 20rem;margin-left: 4rem;text-align: center;vertical-align: top;}");
            client.println(".brightness-sensor {font-size: 12.5px;border: 3px solid rgb(255, 113, 113);padding: 2rem;display: inline-block;justify-content: center;width: 20rem;margin-left: 4rem;text-align: center;align-items: center;vertical-align: top;}");
            client.println(".brightness {border: 3px solid rgb(250, 180, 51);padding: 1rem;}");
//            client.println(".motion-sensor {font-size: 12.5px;border: 3px solid rgb(255, 113, 113);padding: 2rem;display: inline-block;justify-content: center;width: 20rem;margin-left: 4rem;text-align: center;align-items: center;vertical-align: top;}");
            client.println(".motion {border: 3px solid rgb(250, 180, 51);padding: 1rem;}");
            client.println(".bbutton {display: inline-block;margin-left: 4rem;vertical-align: top;}");
            client.println("#on{cursor: pointer;border: none;font-size: 40px;color: rgb(0, 0, 0); background-color: rgb(255, 230, 2);padding: 50px 50px; }</style></head>");
            client.println("#off{cursor: pointer;border: none;font-size: 40px;color: rgb(0, 0, 0); background-color: rgb(175, 174, 168);padding: 50px 50px; }</style></head>");
            client.println("<body><div class=\"name\"><h1>Smartest Light</h1></div>");
            client.println("<div class=\"about\"><h2>How to use (manual mode)</h2>");
            client.println("<ul id=\"list\"><li>You can turn on/off the light via the button located on the right.</li><li>If it's not dark yet, the lights will not turn on. Even if you pressthe button or not.</li><li>If it's not bright, you can turn off the lights.</li></ul></div>");
            client.println("<div class=\"main-func\">");
//            client.println("<div class=\"Lightbulb\"><span align=\"center\" class=\"dot\"></span><h2>Status of the light bulb (Green = ON/Gray = OFF)</h2><span align=\"center\" class=\"dotready\"></span><h2>Status of the light bulb ready to turn on (Yellow = ready/Gray = not ready)</h2></div>
            client.println("<div class=\"brightness-sensor\"><h2 class=\"brightness\"></h2><h2 class=\"brightness-desc\">The brightness measured by the sensor</h2></div>");
//            client.println("<div class=\"motion-sensor\"><h2 class=\"motion\">motion</h2><h2 class=\"motion-desc\">Status of the motion (Blue = detect motion/ Gray = no movement)</h2></div>");
            
            
            // Display current state, and ON/OFF buttons
            // If the outputState is off, it displays the ON button       
            if (outputState=="off") {
              client.println("<p><a href=\"/on\"><button id = \"on\" type=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/off\"><button id = \"off\" type=\"button\">OFF</button></a></p>");
            } 
            
//            client.println("<div class=\"bbutton\"><button id = \"on-off\" type=\"button\">ON</button></div></div></body></html>");

            //*********************************************************************************************************************************************
            
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
}
