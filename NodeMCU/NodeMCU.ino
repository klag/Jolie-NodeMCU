#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <coap.h>
#include <MQTTClient.h>

const char ssid[] = "SPZ-Mobile";
const char pass[] = "ciaocomestai";

void callback_response(CoapPacket &packet, IPAddress ip, int port);
void callback_light(CoapPacket &packet, IPAddress ip, int port);
void callback_core(CoapPacket &packet, IPAddress ip, int port);
void callback_gettmp(CoapPacket &packet, IPAddress ip, int port);

WiFiClient wifi_client;
MQTTClient mqtt_client;
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

void messageReceived(String & topic, String & payload)
{
  Serial.println("incoming: " + topic + " - " + payload);
  if (topic.equals("42/GET_TEMPERATURE"))
  {
    String topicResponse = payload.substring(1, payload.length() - 1);
    mqtt_client.publish(topicResponse, "24");
  }
  if (topic.equals("42/LED_STATE"))
  {
    if (payload.equals("OFF"))
    {
      digitalWrite(LED_BUILTIN, HIGH);
    }
    else
    {
      digitalWrite(LED_BUILTIN, LOW);
    }
  }
}

void setup()
{
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  WiFi.begin(ssid, pass);
  
  mqtt_client.begin("iot.eclipse.org", 1883, wifi_client);
  mqtt_client.onMessage(messageReceived);
  
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
  
  String clientId = "ESP8266Client-";
  clientId += String(random(0xffff), HEX);
  while (!mqtt_client.connect(clientId.c_str())) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("\nMQTT connected!");
  mqtt_client.subscribe("42/LED_STATE");
  mqtt_client.subscribe("42/GET_TEMPERATURE");
}

void loop() {
  mqtt_client.loop();
  delay(10);
  if (!mqtt_client.connected())
  {
    connect();
  }
  delay(1000);
  coap.loop();
}
