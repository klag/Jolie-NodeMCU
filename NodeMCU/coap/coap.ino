#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <coap.h>

const char ssid[] = "SPZ-Mobile";
const char pass[] = "ciaocomestai";

void callback_response(CoapPacket &packet, IPAddress ip, int port);
void callback_light(CoapPacket &packet, IPAddress ip, int port);
void callback_core(CoapPacket &packet, IPAddress ip, int port);
void callback_gettmp(CoapPacket &packet, IPAddress ip, int port);

WiFiUDP udp;
Coap coap(udp);
bool LEDSTATE;

void callback_light(CoapPacket &packet, IPAddress ip, int port) {
  char p[packet.payloadlen + 1];
  memcpy(p, packet.payload, packet.payloadlen);
  p[packet.payloadlen] = NULL;
  String message(p);
  if (message.equals("ON"))
    LEDSTATE = false;
  else if(message.equals("OFF"))
    LEDSTATE = true;
  if (LEDSTATE) {
    digitalWrite(LED_BUILTIN, HIGH) ; 
    coap.sendResponse(ip, port, packet.messageid, "OFF");
  } else { 
    digitalWrite(LED_BUILTIN, LOW) ; 
    coap.sendResponse(ip, port, packet.messageid, "ON");
  }
}

void callback_core(CoapPacket &packet, IPAddress ip, int port) {
  coap.sendResponse(ip, port, packet.messageid, "</42/GET_TEMPERATURE>,</42/LED_STATE>");
}

void callback_gettmp(CoapPacket &packet, IPAddress ip, int port) {
  coap.sendResponse(ip, port, packet.messageid, "24");
}

void callback_response(CoapPacket &packet, IPAddress ip, int port) {
  Serial.println("[Coap Response got]");
  
  char p[packet.payloadlen + 1];
  memcpy(p, packet.payload, packet.payloadlen);
  p[packet.payloadlen] = NULL;
  
  Serial.println(p);
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  WiFi.begin(ssid, pass);
  connect();
  
  LEDSTATE = true;
  coap.server(callback_light, "42/LED_STATE");
  coap.server(callback_core, "42/.well-known/core");
  coap.server(callback_gettmp, "42/GET_TEMPERATURE");
  coap.response(callback_response);
  coap.start();
}

void connect()
{
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  delay(1000);
  coap.loop();
}
