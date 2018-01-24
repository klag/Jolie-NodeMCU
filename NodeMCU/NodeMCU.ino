#include <ESP8266WiFi.h>
#include <MQTTClient.h>
const char ssid[] = "SPZ-Mobile";
const char pass[] = "ciaocomestai";
WiFiClient net;
MQTTClient client;
unsigned long lastMillis = 0;
void connect();
void setup()
{
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  WiFi.begin(ssid, pass);
  client.begin("iot.eclipse.org", 1883, net);
  client.onMessage(messageReceived);
  connect();
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
  while (!client.connect(clientId.c_str()))
  {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("\nconnected!");
  client.subscribe("42/LED_STATE");
  client.subscribe("42/GET_TEMPERATURE");
}

void loop()
{
  client.loop();
  delay(10);
  if (!client.connected())
  {
    connect();
  }
}

void messageReceived(String & topic, String & payload)
{
  Serial.println("incoming: " + topic + " - " + payload);
  if (topic == "42/GET_TEMPERATURE")
  {
    String topicResponse = payload.substring(1, payload.length()-1);
    client.publish(topicResponse, "24");
  }
  else
  {
    if (payload == "ON")
    {
      digitalWrite(LED_BUILTIN, LOW);
    }
    else
    {
      if (payload == "OFF")
      {
        digitalWrite(LED_BUILTIN, HIGH);
      }
    }
  }
}
