#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <coap.h>
#include <MQTTClient.h>

const char ssid[] = "your_wifi_essid";
const char pass[] = "your_wifi_password";

char temperature_topic[] = "42/GET_TEMPERATURE"; 
char led_topic[] = "42/LED_STATE";
char discovery_service[] = "42/.well-known/core";

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

char * concatenate_strings(char * str1, char * str2) {
  char * new_str ;
  new_str[0] = '\0';
  strcat(new_str,str1);
  strcat(new_str,str2);
  return new_str;  
}

void callback_core(CoapPacket &packet, IPAddress ip, int port) {
  char * discovery = "";
  char * discovery_begin = "</";
  char * discovery_end = ">";
  char * discovery_separator = ",";
  concatenate_strings(discovery, discovery_begin);
  concatenate_strings(discovery, temperature_topic);
  concatenate_strings(discovery, discovery_end);
  concatenate_strings(discovery, discovery_separator);
  concatenate_strings(discovery, discovery_begin);
  concatenate_strings(discovery, led_topic);
  concatenate_strings(discovery, discovery_end);
  coap.sendResponse(ip, port, packet.messageid, discovery);
}

void callback_gettmp(CoapPacket &packet, IPAddress ip, int port) {
  char temperature[] = "24"; //feel free to take it from a real sensor
  coap.sendResponse(ip, port, packet.messageid, temperature);
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
  if (topic.equals(temperature_topic))
  {
    String topicResponse = payload.substring(1, payload.length() - 1);
    mqtt_client.publish(topicResponse, "24");
  }
  if (topic.equals(led_topic))
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
  coap.server(callback_light, led_topic);
  coap.server(callback_core, discovery_service);
  coap.server(callback_gettmp, temperature_topic);
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
  mqtt_client.subscribe(led_topic);
  mqtt_client.subscribe(temperature_topic);
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
