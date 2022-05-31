#include <SoftwareSerial.h>
SoftwareSerial mySerial(4, 5);
long lastUART = 0;
void Read_Uart();    // UART STM
String LED1 = "OFF", LED2 = "OFF";
void setup()
{
  Serial.begin(9600);
  mySerial.begin(115200);

  Serial.println("UART Start");

  lastUART = millis();
}
void loop()
{
  Read_Uart();
  if (millis() - lastUART > 1000)
  {
    mySerial.print("10 11 01 00");
    Serial.println("Truyen : 1ON2ON3OFF4");
    lastUART = millis();
  }
}
void Read_Uart()
{
  String st = "";
  while (mySerial.available())
  {
    char inChar = (char)mySerial.read();
    st +=  inChar;
    Serial.println(mySerial.read());
  }
}
