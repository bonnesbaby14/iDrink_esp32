#include "secrets.h"
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "WiFi.h"

#define AWS_IOT_PUBLISH_TOPIC "esp32/pub"
#define AWS_IOT_SUBSCRIBE_TOPIC "esp32/sub"

int tequila = 16;
int granadina = 17;
int vodka = 18;
int jugo = 19;

WiFiClientSecure net = WiFiClientSecure();
PubSubClient client(net);

void connectAWS()
{

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.println("Connecting to Wi-Fi");

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  // Configure WiFiClientSecure to use the AWS IoT device credentials

  net.setCACert(AWS_CERT_CA);
  net.setCertificate(AWS_CERT_CRT);
  net.setPrivateKey(AWS_CERT_PRIVATE);

  // Connect to the MQTT broker on the AWS endpoint we defined earlier
  client.setServer(AWS_IOT_ENDPOINT, 8883);

  // Create a message handler
  client.setCallback(messageHandler);

  Serial.println("Connecting to AWS IOT");

  while (!client.connect(THINGNAME))
  {
    Serial.print(".");
    delay(100);
  }

  if (!client.connected())
  {
    Serial.println("AWS IoT Timeout!");
    return;
  }

  // Subscribe to a topic
  client.subscribe(AWS_IOT_SUBSCRIBE_TOPIC);

  Serial.println("AWS IoT Connected!");
}

void publishMessage()
{
  StaticJsonDocument<200> doc;
  doc["status"] = "activo";
  char jsonBuffer[512];
  serializeJson(doc, jsonBuffer); // print to client

  client.publish(AWS_IOT_PUBLISH_TOPIC, jsonBuffer);
}

void messageHandler(char *topic, byte *payload, unsigned int length)
{
  Serial.print("incoming: ");
  Serial.println(topic);

  StaticJsonDocument<200> doc;
  deserializeJson(doc, payload);
  const char *drink = doc["drink"];
  Serial.println(drink);

   if (strcmp(drink, "greyhound") == 0)
  {
    greyhound();

  }else if(strcmp(drink, "tequila_sunrise") == 0){
    tequila_sunrise();
  }
  else if(strcmp(drink, "desarmador") == 0){
    desarmador();
  }
  else if(strcmp(drink, "cosmopolitan") == 0){
    cosmopolitan();
  }
}

void tequila_sunrise()
{
  Serial.println("tequla");

  digitalWrite(jugo, HIGH);
  sleep(4);
  digitalWrite(jugo, LOW);

  digitalWrite(tequila, HIGH);
  sleep(2);
  digitalWrite(tequila, LOW);

  digitalWrite(granadina, HIGH);
  sleep(1);
  digitalWrite(granadina, LOW);
}

void desarmador()
{
  Serial.println("desar");
  digitalWrite(jugo, HIGH);
  sleep(4);
  digitalWrite(jugo, LOW);

  digitalWrite(tequila, HIGH);
  sleep(2);
  digitalWrite(tequila, LOW);
}

void cosmopolitan()
{
    Serial.println("cosmo");
  digitalWrite(jugo, HIGH);
  sleep(4);
  digitalWrite(jugo, LOW);

  digitalWrite(vodka, HIGH);
  sleep(2);
  digitalWrite(vodka, LOW);

  digitalWrite(granadina, HIGH);
  sleep(1);
  digitalWrite(granadina, LOW);
}

void greyhound()
{
    Serial.println("greu");
  digitalWrite(jugo, HIGH);
  sleep(4);
  digitalWrite(jugo, LOW);

  digitalWrite(vodka, HIGH);
  sleep(2);
  digitalWrite(vodka, LOW);
}
void setup()
{

  pinMode(tequila, OUTPUT);
  digitalWrite(tequila, LOW);

  pinMode(granadina, OUTPUT);
  digitalWrite(granadina, LOW);

  pinMode(vodka, OUTPUT);
  digitalWrite(vodka, LOW);

  pinMode(jugo, OUTPUT);
  digitalWrite(jugo, LOW);

  Serial.begin(115200);
  connectAWS();
}

void loop()
{

  client.loop();
  delay(100);
}
