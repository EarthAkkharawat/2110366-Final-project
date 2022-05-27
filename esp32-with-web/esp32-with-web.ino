/*********
  Rui Santos
  Complete project details at http://randomnerdtutorials.com
*********/

// Load Wi-Fi library
#include <WiFi.h>

// Replace with your network credentials
const char* ssid = "jade_Internet";
const char* password = "Jafe1846";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Auxiliar variables to store the current output state
String output26State = "off";
String output27State = "off";

// Assign output variables to GPIO pins
const int output26 = 26;
const int output27 = 27;

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0;
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

void setup() {
  Serial.begin(115200);
  // Initialize the output variables as outputs
  pinMode(output26, OUTPUT);
  pinMode(output27, OUTPUT);
  // Set outputs to LOW
  digitalWrite(output26, LOW);
  digitalWrite(output27, LOW);

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
//            if (header.indexOf("GET /26/on") >= 0) {
//              Serial.println("GPIO 26 on");
//              output26State = "on";
//              digitalWrite(output26, HIGH);
//            } else if (header.indexOf("GET /26/off") >= 0) {
//              Serial.println("GPIO 26 off");
//              output26State = "off";
//              digitalWrite(output26, LOW);
//            } else if (header.indexOf("GET /27/on") >= 0) {
//              Serial.println("GPIO 27 on");
//              output27State = "on";
//              digitalWrite(output27, HIGH);
//            } else if (header.indexOf("GET /27/off") >= 0) {
//              Serial.println("GPIO 27 off");
//              output27State = "off";
//              digitalWrite(output27, LOW);
//            }

            // Display the HTML web page
            //**********************************************************************************************************************************************
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name = \"viewport\" content = \"width=device - width, initial - scale = 1\">");
            client.println("<link rel = \"icon\" href = \"image/lightbub.png\"/>");
            client.println("< meta charset = \"UTF-8\" / > ");
            client.println("< meta http - equiv = \"X - UA - Compatible\" content = \"IE = edge\"/> ");
            client.println("<title>Smartest Light</title >");
            client.println("<style>html {font-family: \"Inter\"; color: rgb(255, 255, 255); background-color: #000000;}");
            client.println(".about {margin: auto;width: 50%;padding: 10px;}");
            client.println(".name {text-align: center;font-size: 30px;}");
            client.println(".about {border: 3px solid rgb(255, 113, 113);padding: 2rem;padding-top: 0.5rem;}");
            client.println("#list {font-size: 20px;}");
            client.println(".dot {height: 100px;width: 100px;background-color: rgb(102, 238, 120);border-radius: 50%;display: inline-block;justify-content: center;text-align: center;align-items: center;padding: 2rem;}");
            client.println(".dotready {height: 100px;width: 100px;background-color: rgb(255, 238, 0);border-radius: 50%;display: inline-block;justify-content: center;text-align: center;align-items: center;padding: 2rem;margin-top: 2rem;}");
            client.println(".main-func {display: inline-block;min-width: fit-content;min-height: fit-content;margin-top: 4rem;align-items: center;text-align: center;}");
            client.println(".Lightbulb {font-size: 12.5px;border: 3px solid rgb(255, 113, 113);padding: 2rem;display: inline-block;justify-content: center;width: 20rem;margin-left: 4rem;text-align: center;vertical-align: top;}");
            client.println(".brightness-sensor {font-size: 12.5px;border: 3px solid rgb(255, 113, 113);padding: 2rem;display: inline-block;justify-content: center;width: 20rem;margin-left: 4rem;text-align: center;align-items: center;vertical-align: top;}");
            client.println(".brightness {border: 3px solid rgb(250, 180, 51);padding: 1rem;}");
            client.println(".motion-sensor {font-size: 12.5px;border: 3px solid rgb(255, 113, 113);padding: 2rem;display: inline-block;justify-content: center;width: 20rem;margin-left: 4rem;text-align: center;align-items: center;vertical-align: top;}");
            client.println(".motion {border: 3px solid rgb(250, 180, 51);padding: 1rem;}");
            client.println(".bbutton {display: inline-block;margin-left: 4rem;vertical-align: top;}");
            client.println("#on-off{cursor: pointer;background-color: #000000;}</style></head>");
            client.println("<body><div class=\"name\"><h1>Smartest Light</h1></div>");
            client.println("<div class=\"about\"><h2>How to use (manual mode)</h2>");
            client.println("<body><div class=\"name\"><h1>Smartest Light</h1></div><ul id=\"list\"><li>You can turn on/off the light via the button located on the right.</li><li>If it's not dark yet, the lights will not turn on. Even if you pressthe button or not.</li><li>If it's not bright, you can turn off the lights.</li></ul></div>");
            client.println("<div class=\"main-func\">");
            client.println("<div class=\"Lightbulb\"><span align=\"center\" class=\"dot\"></span><h2>Status of the light bulb (Green = ON/Gray = OFF)</h2><span align=\"center\" class=\"dotready\"></span><h2>Status of the light bulb ready to turn on (Yellow = ready/Gray = not ready)</h2></div><div class=\"brightness-sensor\"><h2 class=\"brightness\">brightness</h2><h2 class=\"brightness-desc\">The brightness measured by the sensor</h2></div>");
            client.println("<div class=\"motion-sensor\"><h2 class=\"motion\">motion</h2><h2 class=\"motion-desc\">Status of the motion (Blue = detect motion/ Gray = no movement)</h2></div>");
            client.println("<div class=\"bbutton\"><button id = \"on-off\" type=\"button\"> <img src=\"image/button-removebg-preview.png\" height =\"200\" width=\"200\" /></button></div></div></body></html>");



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
