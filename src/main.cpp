#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

/* Put your SSID & Password */
const char* ssid = "FlutterESP32";  // Enter SSID here
const char* password = "12345678";  //Enter Password here

int doors[] = {0,2,4,5};
bool doors_state[4];

//Functions

void turn_off_all();
void handle_OnConnect() ;
void handle_NotFound();
void handle_ledOn0();
void handle_ledOff0();
void handle_ledOn2();
void handle_ledOff2();
void handle_ledOn4();
void handle_ledOff4();
void handle_ledOn5();
void handle_ledOff5();
String SendHTML();

/* Put IP Address details */
IPAddress local_ip(192,168,1,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

WebServer server(80);



void setup() {
  Serial.begin(115200);
  pinMode(0, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  
  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(100);
  
  server.on("/", handle_OnConnect);
  server.on("/led_on0", handle_ledOn0);
  server.on("/led_off0", handle_ledOff0);
  server.on("/led_on2", handle_ledOn2);
  server.on("/led_off2", handle_ledOff2);
  server.on("/led_on4", handle_ledOn4);
  server.on("/led_off4", handle_ledOff4);
  server.on("/led_on5", handle_ledOn5);
  server.on("/led_off5", handle_ledOff5);
  server.on("/all_off", turn_off_all);
  server.onNotFound(handle_NotFound);
  server.begin();
  Serial.println("HTTP server started");
  turn_off_all();
}
void loop() {
  server.handleClient();
  for(int i =0; i< sizeof(doors)/sizeof(int); i++){
    digitalWrite(doors[i], doors_state[i]);
  }
}

void turn_off_all(){
  for(int i =0; i< sizeof(doors)/sizeof(int); i++){
    doors_state[i] = LOW;
    digitalWrite(doors[i], LOW);
  }
  server.send(200, "text/html", SendHTML()); 
}

void handle_OnConnect() {
  //turn_off_all();
  server.send(200, "text/html", SendHTML()); 
}

void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}

void handle_ledOn0() {
  doors_state[0] = HIGH;
  server.send(200, "text/html", SendHTML()); 
}

void handle_ledOff0() {
  doors_state[0] = LOW;
  server.send(200, "text/html", SendHTML());  
}

void handle_ledOn2() {
  doors_state[1] = HIGH;
  server.send(200, "text/html", SendHTML()); 
}

void handle_ledOff2() {
  doors_state[1] = LOW;
  server.send(200, "text/html", SendHTML()); 
}

void handle_ledOn4() {
  doors_state[2] = HIGH;
  server.send(200, "text/html", SendHTML()); 
}

void handle_ledOff4() {
  doors_state[2] = LOW;
  server.send(200, "text/html", SendHTML()); 
}

void handle_ledOn5() {
  doors_state[3] = HIGH;
  server.send(200, "text/html", SendHTML()); 
}

void handle_ledOff5() {
  doors_state[3] = LOW;
  server.send(200, "text/html", SendHTML()); 
}

String SendHTML(){
  String ptr = "";
  ptr +="[\n";

  for(int i =0; i < sizeof(doors)/sizeof(int); i++){
    if(i != (sizeof(doors)/sizeof(int) - 1)){
      ptr +=  "{\ndoor :" + String(doors[i]) +" ,";
      ptr += "state : " + String(doors_state[i]) + "\n},\n";
    }else{
      ptr +=  "{\ndoor :" + String(doors[i]) +" ,";
      ptr += "state : " + String(doors_state[i]) + "\n}";
    }
  }
  ptr +="]";
  return ptr;
}
