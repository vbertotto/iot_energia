#include <DHTesp.h>
#include <NTPClient.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <PubSubClient.h>

#define WIFI_SSID "Wokwi-GUEST"
#define WIFI_PASSWORD ""
#define WIFI_CHANNEL 6

const char* mqtt_server = "test.mosquitto.org";
char* my_topic_SUB = "FIT/SUB";
char* my_topic_PUB1 = "FIT/PUB_Temperatura";

WiFiClient espClient;
PubSubClient client(espClient);

const int DHT_PIN = 32;
int led1 = 4;
int hora = 0;

DHTesp dhtSensor;
float temperatura;
float lastTemp = -999.0;

WiFiUDP ntpUDP;
NTPClient ntp(ntpUDP, "pool.ntp.org");

unsigned long lastMillis = 0;

void setup() {
  Serial.begin(115200);
  Serial.println("Hello, ESP32!");
  dhtSensor.setup(DHT_PIN, DHTesp::DHT22);
  pinMode(led1, OUTPUT);
  conectawifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  delay(100);

  delay(2000);
  TempAndHumidity data = dhtSensor.getTempAndHumidity();
  temperatura = data.temperature;

  if (temperatura > 28) {
    digitalWrite(led1, HIGH);
  } else {
    digitalWrite(led1, LOW);

  if (ntp.update()) {
    Serial.print("HORARIO: ");
    Serial.println(ntp.getFormattedTime());
    hora = ntp.getHours();

    if (hora > 8 && hora < 18) {
      digitalWrite(led1, HIGH);
    } else {
      digitalWrite(led1, LOW);
    }
  } else {
    Serial.println("!Erro ao atualizar NTP!\n");
  } 

  if (millis() - lastMillis > 15000) {
    lastMillis = millis();
    int temp = temperatura;
    char mensagem[10];
    sprintf(mensagem, "%d", temp);
    client.publish(my_topic_PUB1, mensagem);
    Serial.print("Temperatura enviada: ");
    Serial.println(mensagem);
  }
}
}

void conectawifi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD, WIFI_CHANNEL);
  Serial.print("Conectando ao WiFi ");
  Serial.print(WIFI_SSID);

  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
  Serial.println(" Conectado!");

  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  WiFi.mode(WIFI_STA);
}

void callback(char* topic, byte* payload, unsigned int length) {
  String string;
  Serial.print("Chegou a mensagem [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    string += ((char)payload[i]);
  }
  Serial.print(string);

  if (string == "1") {
    digitalWrite(led1, HIGH);
  } else {
    digitalWrite(led1, LOW);
  }
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Tentando conectar ao MQTT ...");
    if (client.connect("ESPClient")) {
      Serial.println("Conectado");
      client.subscribe(my_topic_SUB);
    } else {
      Serial.print("falhou, rc=");
      Serial.print(client.state());
      Serial.println(" Tente novamente em 5 segundos");
      delay(5000);
    }
  }
}
