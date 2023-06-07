#include "secrets.h"
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "WiFi.h"
#include <HTTPClient.h>

#define AWS_IOT_PUBLISH_TOPIC "IDRINK/pub"
#define AWS_IOT_SUBSCRIBE_TOPIC "IDRINK/sub"

int tequila = 16;
int granadina = 17;
int vodka = 18;
int jugo = 19;


int variable1=25;
int variable2=25;
int variable3=25;
int variable4=25;

WiFiClientSecure net = WiFiClientSecure();
PubSubClient client(net);

unsigned long previousMillis = 0;
const unsigned long interval = 15000; // Intervalo de 1 minuto (60000 ms)

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

  Serial.println("Connecting to AWS IoT");

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

  // Enviar los datos por POST a un servidor
  HTTPClient http;
  http.begin("https://idrink-api-prod-idrink-api-fqemyp.mo2.mogenius.io/send_leves"); // Reemplaza con la URL de tu servidor y la ruta correspondiente
  DynamicJsonDocument jsonDoc(256);
    jsonDoc["bottle1"] = variable1;
    jsonDoc["bottle2"] = variable2;
    jsonDoc["bottle3"] = variable3;
    jsonDoc["bottle4"] = variable4;

    // Convertir el objeto JSON a una cadena
    String jsonString;
    serializeJson(jsonDoc, jsonString);

   
    http.addHeader("Content-Type", "application/json");

    // Enviar la solicitud POST con los datos en formato JSON
    int httpResponseCode = http.POST(jsonString);

    if (httpResponseCode > 0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
    } else {
      Serial.print("Error in HTTP request: ");
      Serial.println(http.errorToString(httpResponseCode).c_str());
    }
    

    http.end();
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
  }
  else if (strcmp(drink, "tequila_sunrise") == 0)
  {
    tequila_sunrise();
  }
  else if (strcmp(drink, "desarmador") == 0)
  {
    desarmador();
  }
  else if (strcmp(drink, "cosmopolitan") == 0)
  {
    cosmopolitan();
  }
}

void tequila_sunrise()
{
  digitalWrite(jugo, HIGH);
  delay(4000);
  variable4=variable4-4;
  digitalWrite(jugo, LOW);

  digitalWrite(tequila, HIGH);
  delay(2000);
  variable1=variable1-2;
  digitalWrite(tequila, LOW);

  digitalWrite(granadina, HIGH);
  delay(1000);
  variable2=variable2-1;
  digitalWrite(granadina, LOW);
}

void desarmador()
{
  digitalWrite(jugo, HIGH);
  delay(8000);
  variable4=variable4-8;
  digitalWrite(jugo, LOW);

  digitalWrite(tequila, HIGH);
  delay(5000);
  variable1=variable1-5;
  digitalWrite(tequila, LOW);
}

void cosmopolitan()
{
  digitalWrite(jugo, HIGH);
  delay(4000);
  variable4=variable4-4;
  digitalWrite(jugo, LOW);

  digitalWrite(vodka, HIGH);
  delay(2000);
  variable3=variable3-2;
  digitalWrite(vodka, LOW);

  digitalWrite(granadina, HIGH);
  variable2=variable2-1;
  delay(1000);
  digitalWrite(granadina, LOW);
}

void greyhound()
{
  digitalWrite(jugo, HIGH);
  delay(4000);
  variable4=variable4-4;
  digitalWrite(jugo, LOW);

  digitalWrite(vodka, HIGH);
  delay(2000);
  variable3=variable3-2;
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

  previousMillis = millis(); // Inicializar el tiempo anterior
}

void loop()
{
  client.loop();

  unsigned long currentMillis = millis(); // Obtener el tiempo actual

  if (currentMillis - previousMillis >= interval)
  {
    // Actualizar las variables y enviar los datos
    //int variable1 = 2; // Actualiza el valor de variable1
    //int variable2 = 1; // Actualiza el valor de variable2
    //int variable3 = 3; // Actualiza el valor de variable3
    //int variable4 = 4; // Actualiza el valor de variable4

    publishMessage();

    previousMillis = currentMillis; // Guardar el tiempo actual como tiempo anterior
  }
}
