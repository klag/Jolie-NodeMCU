#include <ESP8266WiFi.h>
#include <MQTTClient.h>
#include <WiFiUDP.h>
#include <coap.h>
const char ssid[] = "SPZ-Mobile";
const char pass[] = "ciaocomestai";
bool LEDSTATE;
WiFiClient wifi_client;
MQTTClient mqtt_client;
WiFiUDP udp;
Coap coap(udp);
void connect();
void callback_response(CoapPacket &packet, IPAddress ip, int port);
void callback_42_led_state(CoapPacket & packet, IPAddress ip, int port);
void setup()
{
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  LEDSTATE = true;
  WiFi.begin(ssid, pass);
  // MQTT
  mqtt_client.begin("iot.eclipse.org", 1883, wifi_client);
  mqtt_client.onMessage(messageReceived);
  connect();
  // CoAP
  Serial.println("Setup Callback Led State");
  coap.server(callback_42_led_state, "42/LED_STATE");
  Serial.println("Setup CoAP Response Callback");
  coap.response(callback_response);
  coap.start();
}

void connect()
{
  Serial.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(1000);
  }
  Serial.print("\nconnecting...");
  String clientId = "ESP8266Client-";
  clientId += String(random(0xffff), HEX);
  while (!mqtt_client.connect(clientId.c_str()))
  {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("\nconnected!");
  printWifiStatus();
  mqtt_client.subscribe("42/LED_STATE");
  mqtt_client.subscribe("42/GET_TEMPERATURE");
}

void loop()
{
  mqtt_client.loop();
  delay(10);
  if (!mqtt_client.connected())
  {
    connect();
  }
}

void messageReceived(String & topic, String & payload)
{
  Serial.println("incoming: " + topic + " - " + payload);
  if (topic == "42/GET_TEMPERATURE")
  {
    String topicResponse = payload.substring(1, payload.length() - 1);
    mqtt_client.publish(topicResponse, "24");
  }
  else
  {
    if (payload.equals("OFF"))
      LEDSTATE = false;
    else
      if (payload.equals("ON"))
        LEDSTATE = true;
    if (LEDSTATE)
    {
      digitalWrite(LED_BUILTIN, LOW);
    }
    else
    {
      digitalWrite(LED_BUILTIN, HIGH);
    }
  }
}

void callback_42_led_state(CoapPacket & packet, IPAddress ip, int port)
{
  // send response
  char p[packet.payloadlen + 1];
  memcpy(p, packet.payload, packet.payloadlen);
  p[packet.payloadlen] = NULL;
  String message(p);
  if (message.equals("OFF"))
    LEDSTATE = false;
  else
    if (message.equals("ON"))
      LEDSTATE = true;
  if (LEDSTATE)
  {
    digitalWrite(LED_BUILTIN, LOW);
    coap.sendResponse(ip, port, packet.messageid, "ON");
  }
  else
  {
    digitalWrite(LED_BUILTIN, HIGH);
    coap.sendResponse(ip, port, packet.messageid, "OFF");
  }
}

// CoAP client response callback
void callback_response(CoapPacket &packet, IPAddress ip, int port) {
  Serial.println("[Coap Response got]");
  
  char p[packet.payloadlen + 1];
  memcpy(p, packet.payload, packet.payloadlen);
  p[packet.payloadlen] = NULL;
  
  Serial.println(p);
}

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

