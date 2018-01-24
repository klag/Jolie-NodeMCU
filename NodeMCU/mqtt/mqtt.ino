#include <ESP8266WiFi.h>
#include <MQTTClient.h>

const char ssid[] = "SPZ-Mobile";
const char pass[] = "ciaocomestai";

WiFiClient wifi_client;
MQTTClient mqtt_client;

void setup()
{
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  WiFi.begin(ssid, pass);
  mqtt_client.begin("iot.eclipse.org", 1883, wifi_client);
  mqtt_client.onMessage(messageReceived);
  connect();
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
