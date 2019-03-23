//By Ankush
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

/* Put your SSID & Password */
const char* ssid = "HomeAuto";  // Enter SSID here
const char* password = "password1234";  //Enter Password here

/* Put IP Address details */
IPAddress local_ip(192,143,1,1);
IPAddress gateway(192,143,1,1);
IPAddress subnet(255,255,255,0);

ESP8266WebServer server(80);

// Initializing Relay Input Pins.
// Using NC(Normally Closed) connection for connecting the devices.

const int SWITCH1pin = 14; //GPIO Pin 14 or D5
bool SWITCH1status = LOW;

const int SWITCH2pin = 12;
bool SWITCH2status = LOW; //GPIO Pin 12 or D6

const int SWITCH3pin = 13;
bool SWITCH3status = LOW; //GPIO Pin 13 or D7

const int SWITCH4pin = 15;
bool SWITCH4status = LOW; //GPIO Pin 15 or D8


void setup() {
  Serial.begin(115200);
  
  // initialize pin as OUTPUT
  pinMode(SWITCH1pin, OUTPUT);
  pinMode(SWITCH2pin, OUTPUT);
  pinMode(SWITCH3pin, OUTPUT);
  pinMode(SWITCH4pin, OUTPUT);

  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(100);
  
  server.on("/", handle_OnConnect);
  server.on("/switch1on", handle_switch1on);
  server.on("/switch1off", handle_switch1off);
  server.on("/switch2on", handle_switch2on);
  server.on("/switch2off", handle_switch2off);
  server.on("/switch3on", handle_switch3on);
  server.on("/switch3off", handle_switch3off);
  server.on("/switch4on", handle_switch4on);
  server.on("/switch4off", handle_switch4off);
  server.onNotFound(handle_NotFound);
  
  server.begin();
  Serial.println("Server started");
}
void loop() {
  server.handleClient();
  if(SWITCH1status)
  {digitalWrite(SWITCH1pin, HIGH);}
  else
  {digitalWrite(SWITCH1pin, LOW);}
  
  if(SWITCH2status)
  {digitalWrite(SWITCH2pin, HIGH);}
  else
  {digitalWrite(SWITCH2pin, LOW);}

  if(SWITCH3status)
  {digitalWrite(SWITCH3pin, HIGH);}
  else
  {digitalWrite(SWITCH3pin, LOW);}

  if(SWITCH4status)
  {digitalWrite(SWITCH4pin, HIGH);}
  else
  {digitalWrite(SWITCH4pin, LOW);}
}

void handle_OnConnect() {
  SWITCH1status = LOW;
  SWITCH2status = LOW;
  SWITCH3status = LOW;
  SWITCH4status = LOW;
  
  Serial.println("GPIO 14 Status: OFF | GPIO 12 Status: OFF | GPIO 13 Status: OFF | GPIO 15 Status: OFF");
  server.send(200, "text/html", SendHTML(SWITCH1status,SWITCH2status,SWITCH3status,SWITCH4status)); 
}

void handle_switch1on() {
  SWITCH1status = HIGH;
  Serial.println("GPIO 14 Status: ON");
  server.send(200, "text/html", SendHTML(true,SWITCH2status)); 
}

void handle_switch1off() {
  SWITCH1status = LOW;
  Serial.println("GPIO 14 Status: OFF");
  server.send(200, "text/html", SendHTML(false,SWITCH2status)); 
}

void handle_switch2on() {
  SWITCH2status = HIGH;
  Serial.println("GPIO 12 Status: ON");
  server.send(200, "text/html", SendHTML(SWITCH1status,true)); 
}

void handle_switch2off() {
  SWITCH2status = LOW;
  Serial.println("GPIO 12 Status: OFF");
  server.send(200, "text/html", SendHTML(SWITCH1status,false)); 
}

void handle_switch3on() {
  SWITCH3status = HIGH;
  Serial.println("GPIO 13 Status: ON");
  server.send(200, "text/html", SendHTML(SWITCH4status,true)); 
}

void handle_switch3off() {
  SWITCH3status = LOW;
  Serial.println("GPIO 13 Status: OFF");
  server.send(200, "text/html", SendHTML(SWITCH4status,false)); 
}

void handle_switch4on() {
  SWITCH4status = HIGH;
  Serial.println("GPIO 15 Status: ON");
  server.send(200, "text/html", SendHTML(SWITCH3status,true)); 
}

void handle_switch4off() {
  SWITCH4status = LOW;
  Serial.println("GPIO 15 Status: OFF");
  server.send(200, "text/html", SendHTML(SWITCH3status,false)); 
}




void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}

String SendHTML(uint8_t switch1stat,uint8_t switch2stat,uint8_t switch3stat,uint8_t switch4stat){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>Control Panel</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: border-style: ridge; inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr +=".button {display: block;width: 80px;border: 2px solid #008CBA; color: white; padding: 12px 24px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr +=".button-on {background-color: #00FF00;}\n";
  ptr +=".button-on:active {background-color: #00FF00;}\n";
  ptr +=".button-off {background-color: #FF0000;}\n";
  ptr +=".button-off:active {background-color: #FF0000;}\n";
  ptr +="p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<h1>Command Center </h1>\n";
  
   if(switch1stat)
  {ptr +="<p>SWITCH1 Status: ON</p><a class=\"button button-off\" href=\"/switch1off\">OFF</a>\n";}
  else
  {ptr +="<p>SWITCH1 Status: OFF</p><a class=\"button button-on\" href=\"/switch1on\">ON</a>\n";}

  if(switch2stat)
  {ptr +="<p>SWITCH2 Status: ON</p><a class=\"button button-off\" href=\"/switch2off\">OFF</a>\n";}
  else
  {ptr +="<p>SWITCH2 Status: OFF</p><a class=\"button button-on\" href=\"/switch2on\">ON</a>\n";}

  if(switch3stat)
  {ptr +="<p>SWITCH3 Status: ON</p><a class=\"button button-off\" href=\"/switch3off\">OFF</a>\n";}
  else
  {ptr +="<p>SWITCH3 Status: OFF</p><a class=\"button button-on\" href=\"/switch3on\">ON</a>\n";}

  if(switch4stat)
  {ptr +="<p>SWITCH4 Status: ON</p><a class=\"button button-off\" href=\"/switch4off\">OFF</a>\n";}
  else
  {ptr +="<p>SWITCH4 Status: OFF</p><a class=\"button button-on\" href=\"/switch4on\">ON</a>\n";}

  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}
