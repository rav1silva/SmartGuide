# Hardware e Montagem

Este arquivo descreve todos os componentes de hardware necessários para montar o protótipo do SmartGuide.

## 1. Lista de Materiais (BOM - Bill of Materials)

| Componente                              | Quantidade | Descrição                                                                  |
| --------------------------------------- | ---------- | -------------------------------------------------------------------------- |
| **ESP32 (DevKit V1)**                   | 1          | Microcontrolador principal (Wi-Fi + MQTT).                                 |
| **Motor Driver L298N**                  | 1          | Ponte H para controle dos dois motores DC.                                 |
| **Motores DC com Roda**                 | 2          | Para tração e direção do carrinho (ex: Motores Amarelos).                  |
| **Roda Boba**                           | 1          | Ponto de apoio frontal ou traseiro para equilíbrio.                        |
| **Sensor Ultrassônico HC-SR04**         | 1          | Para medição de distância e detecção de obstáculos.                        |
| **Motores de Vibração (Pancake/Moeda)** | 2          | Para feedback tátil (Lado Esquerdo e Direito).                             |
| **Fonte de Alimentação Externa**        | 1          | Pack de baterias (ex: 6x AA = 9V) para alimentar o L298N.                  |
| **Jumpers (Macho-Macho, Macho-Fêmea)**  | Vários     | Para conexões elétricas.                                                   |
| **Chassi do Carrinho**                  | 1          | Estrutura para fixar os componentes (pode ser impresso em 3D ou adaptado). |
| **LED (Opcional)**                      | 1          | Para debug visual (usar pino de saída válido, ex: GPIO 25).                |

### Alimentação

- **L298N (Pino +12V)**: Conectar à fonte de alimentação externa (Ex: 9V).
- **L298N (Pino GND)**: Conectar ao GND da fonte externa.
- **ESP32 (Pino VIN)**: Conectar ao pino +5V do L298N (isso alimenta o ESP32 a partir da fonte dos motores).
- **ESP32 (Pino GND)**: Conectar ao GND do L298N. (**GND Comum é essencial!**)

### Controle dos Motores (ESP32 -> L298N)

- **GPIO 5 (ESP32)** -> **IN1 (L298N)**
- **GPIO 18 (ESP32)** -> **IN2 (L298N)**
- **GPIO 19 (ESP32)** -> **IN3 (L298N)**
- **GPIO 21 (ESP32)** -> **IN4 (L298N)**

**Motor A (Esquerdo)**: Conectar aos bornes **OUT1** e **OUT2** do L298N.

**Motor B (Direito)**: Conectar aos bornes **OUT3** e **OUT4** do L298N.

### Sensor Ultrassônico (ESP32 -> HC-SR04)

- **GPIO 2 (ESP32)** -> **TRIGGER (HC-SR04)**
- **GPIO 4 (ESP32)** -> **ECHO (HC-SR04)**
- **5V (ESP32 ou L298N)** -> **VCC (HC-SR04)**
- **GND (ESP32 ou L298N)** -> **GND (HC-SR04)**

### Feedback Tátil (ESP32 -> Vibração)

- **GPIO 22 (ESP32)** -> **VIBRA_L** (Positivo do motor de vibração esquerdo)
- **GPIO 23 (ESP32)** -> **VIBRA_R** (Positivo do motor de vibração direito)
- **GND**: Conectar o terminal negativo de ambos os motores de vibração ao **GND do ESP32**.

**Nota**: Para um protótipo mais robusto, use um **transistor** (ex: **BC547**) para chavear os motores de vibração, protegendo os pinos do ESP32.

### LEDs de Teste (Atenção)

- **GPIO 13 (ESP32)** -> **LED_BUILTIN** (LED azul na placa)
- **GPIO 35 (ESP32)** -> **PIN_LED**

**Atenção**: O pino **GPIO 35** no ESP32 é **somente entrada** (**Input Only**). O código tenta usá-lo como **OUTPUT**. Para usar um LED externo, **mude este pino no código para um pino de saída válido** (ex: **GPIO 25**).

## 2. Processo de Construção (Montagem)

### **Chassi**

- Fixe os dois motores DC e a roda boba no chassi para criar a base de 3 rodas.

### **Eletrônica**

- Fixe o **ESP32**, o **L298N** e o **pack de baterias** no chassi.

### **Motores**

- Conecte os fios dos **Motores A** (Esquerdo) e **B** (Direito) nas saídas (**OUT1-OUT4**) do **L298N**.

### **Controle**

- Conecte os pinos de controle (**IN1-4**) do **L298N** aos GPIOs **5, 18, 19, 21** do **ESP32**.

### **Alimentação**

- Conecte a **fonte de 9V** ao **L298N** e alimente o **ESP32** (pelo pino **5V** do L298N).
- **Conecte todos os GNDs**: **Fonte dos motores**, **L298N**, **ESP32** devem ter GND em comum.

### **Sensor Ultrassônico**

- Monte o **sensor HC-SR04** na parte frontal do chassi, apontando para frente.
- Conecte-o aos pinos **2 (TRIGGER)** e **4 (ECHO)** do **ESP32**.

### **Feedback**

- Posicione os **motores de vibração** onde o usuário segurará o carrinho.
- Conecte-os aos pinos **22 (Esquerda)** e **23 (Direita)**.

---

Esta documentação descreve as etapas essenciais para montar o hardware do projeto **SmartGuide**. Ela abrange desde a **lista de materiais** até as instruções detalhadas sobre como fazer as **ligações elétricas** e montar os componentes no **chassi** do carrinho.

Com isso, você pode montar seu carrinho de compras inteligente para deficientes visuais e **ajustar conforme necessário**!
