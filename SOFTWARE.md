# Software e Configuração

O coração do **SmartGuide** é o firmware embarcado no **ESP32**, escrito em **C++/Arduino**. Este arquivo detalha como configurá-lo e usá-lo.

## 1. Visão Geral do Firmware

O código em `smartGuide/smartGuide.ino` é responsável por:

- Conectar o **ESP32** à rede **Wi-Fi**.
- Conectar a um **broker MQTT público** para receber comandos e enviar dados.
- Gerenciar um "estado de movimento" (**MoveState**) para controlar o **Motor Driver L298N**.
- Ler o **sensor ultrassônico** (HC-SR04) em intervalos regulares (a cada 200ms).
- Publicar a distância medida no **MQTT** (a cada 1000ms).
- Ativar os **motores de vibração** se um obstáculo estiver muito próximo (**<= 15cm**).

## 2. Configuração (Obrigatória)

Antes de carregar o código, você **DEVE** editar as seguintes variáveis no início do arquivo `smartGuide/smartGuide.ino`:

### **Configuração Wi-Fi**

Altere as variáveis `SSID` e `PASSWORD` para corresponder às credenciais da sua rede **Wi-Fi**.

```cpp
// --- CONFIGURE SEU WI-FI AQUI ---
const char* SSID = "";
const char* PASSWORD = "";
// ---------------------------------
```
