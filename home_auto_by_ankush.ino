//By Ankush 

#include <ESP8266WiFi.h>//ESP8266 Core WiFi Library 
#include <WiFiClient.h>

const char* ssid = "homeauto"; // Input your hotspot name
const char* password = "password1234";
WiFiServer server(80); // Set web server port number to 80
String header;// Variable to store the HTTP request
// Auxiliar variables to store the current output state
String relay1State = "off";
String relay2State = "off";
String relay3State = "off";
String relay4State = "off";
// Assign output variables to GPIO pins
const int relay1 = 5; // GPIO5 D1
const int relay2 = 4; // GPIO4 D2
const int relay3 = 0; // GPIO0 D3
const int relay4 = 2; // GPIO2 D4

void setup() {
  Serial.begin(115200);
  
  // Initialize the output variables as outputs
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  pinMode(relay4, OUTPUT);
  // Set outputs to HIGH. relay active LOW
  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, HIGH);
  digitalWrite(relay3, HIGH);
  digitalWrite(relay4, HIGH);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  delay(150);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  server.begin();
  Serial.println("Server started");
}
void loop() {
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
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
            if (header.indexOf("GET /5/on") >= 0)
            {
              Serial.println("GPIO 5 on");
              relay1State = "on";
              digitalWrite(relay1, LOW);
            }
            else if (header.indexOf("GET /5/off") >= 0)
            {
              Serial.println("GPIO 5 off");
              relay1State = "off";
              digitalWrite(relay1, HIGH);
            }
            else if (header.indexOf("GET /4/on") >= 0) {
              Serial.println("GPIO 4 on");
              relay2State = "on";
              digitalWrite(relay2, LOW);
            }
            else if (header.indexOf("GET /4/off") >= 0) {
              Serial.println("GPIO 4 off");
              relay2State = "off";
              digitalWrite(relay2, HIGH);
            }
            else if (header.indexOf("GET /0/on") >= 0)
            {
              Serial.println("GPIO 0 on");
              relay3State = "on";
              digitalWrite(relay3, LOW);
            }
            else if (header.indexOf("GET /0/off") >= 0)
            {
              Serial.println("GPIO 0 off");
              relay3State = "off";
              digitalWrite(relay3, HIGH);
            }
            else if (header.indexOf("GET /2/on") >= 0) {
              Serial.println("GPIO 2 on");
              relay4State = "on";
              digitalWrite(relay4, LOW);
            }
            else if (header.indexOf("GET /2/off") >= 0) {
              Serial.println("GPIO 2 off");
              relay4State = "off";
              digitalWrite(relay4, HIGH);
            }

            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<title> Controll Server</title>");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            client.println("<style>html { font-family: Helvetica; border-style: ridge; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".switch { background-color: #00FF00; border: 2px solid #008CBA; color: white; padding: 12px 24px;");
            client.println("text-decoration: none; font-size: 20px; margin: 2px; cursor: pointer;}");
            client.println(".switch2 {background-color: #FF0000;}</style></head>");

            // Web Page Heading
            client.println("<body><h1>Command Center</h1>");

            // Display current state, and ON/OFF buttons for GPIO 5
            client.println("<p>Switch 1- State " + relay1State + "</p>");
            // If the relay1State is off, it displays the ON button
            if (relay1State == "off") {
              client.println("<p><a href=\"/5/on\"><button class=\"switch\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/5/off\"><button class=\"switch switch2\">OFF</button></a></p>");
            }

            // Display current state, and ON/OFF buttons for GPIO 4
            client.println("<p>Switch 2 - State " + relay2State + "</p>");
            // If the relay2State is off, it displays the ON button
            if (relay2State == "off") {
              client.println("<p><a href=\"/4/on\"><button class=\"switch\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/4/off\"><button class=\"switch switch2\">OFF</button></a></p>");
            }

            // Display current state, and ON/OFF buttons for GPIO 0
            client.println("<p>Switch 3 - State " + relay3State + "</p>");
            // If the relay1State is off, it displays the ON button
            if (relay3State == "off") {
              client.println("<p><a href=\"/0/on\"><button class=\"switch\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/0/off\"><button class=\"switch switch2\">OFF</button></a></p>");
            }

            // Display current state, and ON/OFF buttons for GPIO 2
            client.println("<p>Switch 4 - State " + relay4State + "</p>");
            // If the relay2State is off, it displays the ON button
            if (relay4State == "off") {
              client.println("<p><a href=\"/2/on\"><button class=\"switch\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/2/off\"><button class=\"switch switch2\">OFF</button></a></p>");
            }

            client.println("</body></html>");

            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
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
    Serial.println("User disconnected.");
    Serial.println("");
  }
}
