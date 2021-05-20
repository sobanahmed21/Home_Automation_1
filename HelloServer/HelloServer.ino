#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#ifndef STASSID
#define STASSID "Taha Ahmad 2"
#define STAPSK  "88888888"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;

ESP8266WebServer server(80);

const int motorOut=D1,bulb1Out=D2,bulb2Out=D3,fanOut=D4;
String motorState="0",bulb1State="0",bulb2State="0",fanState="0";

void handleRoot() {
  String key,value;
  for (uint8_t i = 0; i < server.args()-1; i++) {
    key=server.argName(i);
    value=server.arg(i);
  }
  if(key=="Motor"){
    if(value == "0"){server.send(200,"text/plain","1");digitalWrite(motorOut, HIGH);motorState="1";}
    else if(value == "1"){server.send(200,"text/plain","0");digitalWrite(motorOut, LOW);motorState="0";}
  }
  else if(key=="Bulb1"){
    if(value == "0"){server.send(200,"text/plain","1");digitalWrite(bulb1Out, 0);bulb1State="1";}
    else if(value == "1"){server.send(200,"text/plain","0");digitalWrite(bulb1Out, 1);bulb1State="0";}
  }
  else if(key=="Bulb2"){
    if(value == "0"){server.send(200,"text/plain","1");digitalWrite(bulb2Out, 0);bulb2State="1";}
    else if(value == "1"){server.send(200,"text/plain","0");digitalWrite(bulb2Out, 1);bulb2State="0";}
  }
  else if(key=="Fan"){
    if(value == "0"){server.send(200,"text/plain","1");digitalWrite(fanOut, 0);fanState="1";}
    else if(value == "1"){server.send(200,"text/plain","0");digitalWrite(fanOut, 1);fanState="0";}
  }
}

void handleNotFound() {
  server.send(200, "text/plain","Motor:"+motorState+"*Bulb1:"+bulb1State+"*Bulb2:"+bulb2State+"*Fan:"+fanState);
  Serial.println("check");
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);

}

void setup(void) {
  pinMode(motorOut,OUTPUT);
  pinMode(bulb1Out,OUTPUT);
  pinMode(bulb2Out,OUTPUT);
  pinMode(fanOut,OUTPUT);
  
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  delay(500);
  WiFi.begin(ssid, password);
  Serial.println("");
  
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }
  server.on("/init", []() {
    server.send(200, "text/plain","Motor:"+motorState+"*Bulb1:"+bulb1State+"*Bulb2:"+bulb2State+"*Fan:"+fanState);
    Serial.println("init");
    Serial.println("Motor:"+motorState+"*Bulb1:"+bulb1State+"*Bulb2:"+bulb2State+"*Fan:"+fanState);
  });
  server.on("/check", []() {
    server.send(200, "text/plain","Motor:"+motorState+"*Bulb1:"+bulb1State+"*Bulb2:"+bulb2State+"*Fan:"+fanState);
    Serial.println("check");
    Serial.println("Motor:"+motorState+"*Bulb1:"+bulb1State+"*Bulb2:"+bulb2State+"*Fan:"+fanState);
  });
  server.on("/", handleRoot);
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
  MDNS.update();
}
