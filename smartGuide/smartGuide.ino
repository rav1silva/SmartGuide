#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

/* ======== DEFINIÇÕES DE PINOS ======== */
#define LED_BUILTIN 13
#define PIN_LED 2
#define VIBRA_L 22
#define VIBRA_R 23

#define PIN_TRIGGER 2
#define PIN_ECHO 4

/* ======== DEFINIÇÕES DE MQTT ======== */
#define TOPICO_SUBSCRIBE_LED        "topico_liga_desliga_led"
#define TOPICO_PUBLISH_DISTANCIA    "topico_sensor_distancia"
#define ID_MQTT  "smartGuide"

const char* BROKER_MQTT = "test.mosquitto.org";
int BROKER_PORT = 1883;

/* ======== REDE WI-FI ======== */
const char* SSID = "2.4GHz NET VIRTUA 400";
const char* PASSWORD = "3523322300";

/* ======== OBJETOS GLOBAIS ======== */
WiFiClient espClient;
PubSubClient MQTT(espClient);

/* ======== PROTÓTIPOS ======== */
void initWiFi(void);
void reconnectWiFi(void);
void initMQTT(void);
void reconnectMQTT(void);
void mqtt_callback(char* topic, byte* payload, unsigned int length);
void VerificaConexoesWiFIEMQTT(void);
float lerDistancia();

/* ======== FUNÇÕES ======== */
void initWiFi(void) {
  delay(10);
  Serial.println("------Conexao WI-FI------");
  Serial.print("Conectando-se na rede: ");
  Serial.println(SSID);
  reconnectWiFi();
}

void reconnectWiFi(void) {
  if (WiFi.status() == WL_CONNECTED) return;
  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(200);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Conectado na rede ");
  Serial.println(SSID);
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

void initMQTT(void) {
  MQTT.setServer(BROKER_MQTT, BROKER_PORT);
  MQTT.setCallback(mqtt_callback);
}

void reconnectMQTT(void) {
  while (!MQTT.connected()) {
    Serial.print("Conectando ao broker MQTT: ");
    Serial.println(BROKER_MQTT);
    if (MQTT.connect(ID_MQTT)) {
      Serial.println("Conectado com sucesso!");
      MQTT.subscribe(TOPICO_SUBSCRIBE_LED);
    } else {
      Serial.println("Falha ao conectar. Tentando novamente em 2s...");
      delay(2000);
    }
  }
}

void mqtt_callback(char* topic, byte* payload, unsigned int length) {
  String msg = "";
  for (int i = 0; i < length; i++) msg += (char)payload[i];

  Serial.print("Mensagem MQTT recebida: ");
  Serial.println(msg);

  if (msg[0] == 'L') {
    digitalWrite(PIN_LED, HIGH);
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.println("LED LIGADO");
  } else if (msg[0] == 'D') {
    digitalWrite(PIN_LED, LOW);
    digitalWrite(LED_BUILTIN, LOW);
    Serial.println("LED DESLIGADO");
  } else {
    Serial.println("Comando desconhecido via MQTT");
  }
}

void VerificaConexoesWiFIEMQTT(void) {
  if (!MQTT.connected()) reconnectMQTT();
  reconnectWiFi();
}

/* ======== FUNÇÃO DE LEITURA ULTRASSÔNICO ======== */
float lerDistancia() {
  digitalWrite(PIN_TRIGGER, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_TRIGGER, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIGGER, LOW);

  long duracao = pulseIn(PIN_ECHO, HIGH);
  float distancia = duracao * 0.034 / 2; // em cm
  return distancia;
}

/* ======== SETUP ======== */
void setup() {
  Serial.begin(9600);
  Serial.println("\nSmartGuide - MQTT + Sensor Ultrassônico");

  pinMode(PIN_LED, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(VIBRA_L, OUTPUT);
  pinMode(VIBRA_R, OUTPUT);
  pinMode(PIN_TRIGGER, OUTPUT);
  pinMode(PIN_ECHO, INPUT);

  initWiFi();
  initMQTT();
}

/* ======== LOOP PRINCIPAL ======== */
void loop() {
  VerificaConexoesWiFIEMQTT();

  float distancia = lerDistancia();

  char distanciaStr[20];
  sprintf(distanciaStr, "%.2f cm", distancia);
  MQTT.publish(TOPICO_PUBLISH_DISTANCIA, distanciaStr);

  Serial.print("Distancia: ");
  Serial.println(distanciaStr);

  MQTT.loop();
  delay(2000);
  if (distancia <= 15.00){
    digitalWrite (VIBRA_R, HIGH);
    digitalWrite (VIBRA_L, HIGH);

  }else{
    digitalWrite (VIBRA_R, LOW);
    digitalWrite (VIBRA_L, LOW);
  }
}
