#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <math.h>

/* ======== DEFINIÇÕES DE PINOS ======== */
#define LED_BUILTIN 13
#define PIN_LED 35 
#define VIBRA_L 22
#define VIBRA_R 23

// Motor A
const int IN1 = 5;
const int IN2 = 18;
// Motor B
const int IN3 = 19;
const int IN4 = 21;

// Ultrassônico
#define PIN_TRIGGER 2
#define PIN_ECHO    4

/* ======== MQTT / WI-FI ======== */
#define TOPICO_SUBSCRIBE_LED         "topico_liga_desliga_led"
#define TOPICO_SUBSCRIBE_MOVER       "smartguide/controle/mover"
#define TOPICO_PUBLISH_DISTANCIA     "topico_sensor_distancia"
#define ID_MQTT  "smartGuide"

const char* BROKER_MQTT = "test.mosquitto.org";
const int   BROKER_PORT = 1883;

// const char* SSID = "2.4GHz NET VIRTUA 400";
// const char* PASSWORD = "3523322300";
const char* SSID = "Ravi's Galaxy S21 FE 5G";
const char* PASSWORD = "vovozona2";

WiFiClient espClient;
PubSubClient MQTT(espClient);

/* ======== ESTADOS DE MOVIMENTO ======== */
enum MoveState { FRENTE=0, TRAS=1, DIREITA=2, ESQUERDA=3, PARAR=4 };
volatile MoveState estadoAtual = PARAR;   // começa parado

/* ======== PROTÓTIPOS ======== */
void initWiFi(void);
void reconnectWiFi(void);
void initMQTT(void);
void reconnectMQTT(void);
void mqtt_callback(char* topic, byte* payload, unsigned int length);
void VerificaConexoesWiFIEMQTT(void);
float lerDistancia_cm(void);

void motorStop(void);
void motorForward(void);
void motorBackward(void);
void motorLeft(void);
void motorRight(void);
void aplicaEstado(MoveState s);

/* ======== CONTROLE DE MOTORES ======== */
void motorStop() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
}
void motorForward() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
}
void motorBackward() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
}
void motorLeft() {   // A pra trás, B pra frente
  digitalWrite(IN1, LOW);  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
}
void motorRight() {  // A pra frente, B pra trás
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);  digitalWrite(IN4, HIGH);
}

void aplicaEstado(MoveState s) {
  switch (s) {
    case FRENTE:   motorForward();  break;
    case TRAS:     motorBackward(); break;
    case DIREITA:  motorRight();    break;
    case ESQUERDA: motorLeft();     break;
    case PARAR:    motorStop();     break;
  }
}

/* ======== WI-FI/MQTT ======== */
void initWiFi(void) {
  delay(10);
  Serial.println("------ Conexao WI-FI ------");
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
      MQTT.subscribe(TOPICO_SUBSCRIBE_MOVER);
    } else {
      Serial.println("Falha ao conectar. Tentando novamente em 2s...");
      delay(2000);
    }
  }
}

void mqtt_callback(char* topic, byte* payload, unsigned int length) {
  String msg;
  for (unsigned int i = 0; i < length; i++) msg += (char)payload[i];

  Serial.print("Mensagem em [");
  Serial.print(topic);
  Serial.print("] = ");
  Serial.println(msg);

  // Comandos de LED (opcional)
  if (String(topic) == TOPICO_SUBSCRIBE_LED) {
    if (!msg.isEmpty()) {
      if (msg[0] == 'L') {
        digitalWrite(PIN_LED, HIGH);
        digitalWrite(LED_BUILTIN, HIGH);
        Serial.println("LED LIGADO (obs: GPIO35 é apenas entrada; troque o pino se quiser LED real)");
      } else if (msg[0] == 'D') {
        digitalWrite(PIN_LED, LOW);
        digitalWrite(LED_BUILTIN, LOW);
        Serial.println("LED DESLIGADO");
      }
    }
    return;
  }

  // Comandos de MOVIMENTO via MQTT
  if (String(topic) == TOPICO_SUBSCRIBE_MOVER) {
    // aceita minúsculas/maiúsculas, corta espaços
    msg.trim();
    msg.toUpperCase();

    if (msg == "F")      estadoAtual = FRENTE;
    else if (msg == "B") estadoAtual = TRAS;
    else if (msg == "L") estadoAtual = ESQUERDA;
    else if (msg == "R") estadoAtual = DIREITA;
    else if (msg == "S") estadoAtual = PARAR;
    else {
      Serial.println("Comando desconhecido. Use F/B/L/R/S");
      return;
    }

    aplicaEstado(estadoAtual);
    Serial.print("Estado aplicado: ");
    Serial.println(msg);
  }
}

void VerificaConexoesWiFIEMQTT(void) {
  if (!MQTT.connected()) reconnectMQTT();
  reconnectWiFi();
}

/* ======== ULTRASSÔNICO ======== */
float lerDistancia_cm(void) {
  // pulso de 10us
  digitalWrite(PIN_TRIGGER, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_TRIGGER, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIGGER, LOW);

  // timeout para não travar (máx ~30ms)
  unsigned long duracao = pulseIn(PIN_ECHO, HIGH, 30000UL);
  if (duracao == 0) return NAN; // sem eco dentro do timeout
  float distancia = (duracao * 0.034f) / 2.0f; // cm
  return distancia;
}

/* ======== TIMERS DO LOOP ======== */
unsigned long tUltLeituraUS = 0;
unsigned long tUltPublish   = 0;
const unsigned long intervaloUS_ms   = 200;  // lê ultrassom a cada 200ms
const unsigned long intervaloPub_ms  = 1000; // publica a cada 1s
float distanciaAtual_cm = NAN;

/* ======== SETUP ======== */
void setup() {
  Serial.begin(9600);
  Serial.println("\nSmartGuide - Controle por MQTT (F/B/L/R/S) + Ultrassom");

  pinMode(PIN_LED, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(VIBRA_L, OUTPUT);
  pinMode(VIBRA_R, OUTPUT);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(PIN_TRIGGER, OUTPUT);
  pinMode(PIN_ECHO, INPUT);

  initWiFi();
  initMQTT();

  aplicaEstado(estadoAtual); // começa parado
}

/* ======== LOOP PRINCIPAL ======== */
void loop() {
  VerificaConexoesWiFIEMQTT();
  MQTT.loop();

  // Leitura ultrassônica periódica
  if (millis() - tUltLeituraUS >= intervaloUS_ms) {
    tUltLeituraUS = millis();
    distanciaAtual_cm = lerDistancia_cm();

    // vibra alerta quando perto (<= 15 cm)
    bool perto = (!isnan(distanciaAtual_cm) && distanciaAtual_cm <= 15.0f);
    digitalWrite(VIBRA_R, perto ? HIGH : LOW);
    digitalWrite(VIBRA_L, perto ? HIGH : LOW);
  }

  // Publica distância via MQTT a cada 1s
  if (millis() - tUltPublish >= intervaloPub_ms) {
    tUltPublish = millis();
    char payload[24];
    if (isnan(distanciaAtual_cm)) {
      snprintf(payload, sizeof(payload), "nan");
    } else {
      snprintf(payload, sizeof(payload), "%.2f cm", distanciaAtual_cm);
    }
    MQTT.publish(TOPICO_PUBLISH_DISTANCIA, payload);
    Serial.print("Distancia publicada: ");
    Serial.println(payload);
  }
}
