# SmartGuide: Carrinho de Compras Inteligente

$$Imagem de um ícone de acessibilidade para deficiência visual$$

**"Tecnologia é ferramenta; inclusão é atitude."**

O SmartGuide é um projeto de tecnologia assistiva focado em aumentar a autonomia e segurança de deficientes visuais durante as compras em supermercados. Este repositório contém o hardware e software para o protótipo de um carrinho de compras inteligente, baseado nos desafios enfrentados por pessoas como a Karen (nossa persona de 32 anos), que busca independência em suas tarefas cotidianas.

## 1. O Problema

Para milhões de brasileiros com deficiência visual (3,4% da população), uma tarefa cotidiana como ir ao supermercado é repleta de desafios que limitam a independência:

- **Orientação Espacial**: Dificuldade em navegar por corredores, identificar filas e desviar de obstáculos inesperados.
- **Ambiente Desorganizado**: Produtos ou carrinhos deixados fora do lugar criam barreiras e riscos.
- **Identificação de Produtos**: A falta de Braille e embalagens semelhantes dificultam a escolha correta dos itens.
- **Falta de Suporte**: A dependência de auxílio humano, que nem sempre está disponível ou preparado, pode ser constrangedora.

Nosso objetivo é usar a tecnologia para mitigar esses desafios, transformando o simples ato de comprar em uma experiência livre, digna e acessível.

## 2. A Solução: SmartGuide

O SmartGuide é um upgrade para carrinhos de supermercado existentes, transformando-os em assistentes de navegação inteligentes. O protótipo atual foca em duas funcionalidades centrais: controle de direção assistido e alerta de obstáculos.

Através de um aplicativo (simulado via MQTT neste protótipo), o usuário pode guiar o carrinho, enquanto sensores embarcados fornecem feedback tátil para alertar sobre barreiras no caminho.

## 3. Funcionalidades (Protótipo Atual)

Este protótipo, baseado no firmware `smartGuide/smartGuide.ino`, implementa as seguintes funções:

- **Controle Remoto via MQTT**: O carrinho pode ser controlado remotamente (Frente, Trás, Esquerda, Direita, Parar) através de mensagens MQTT, simulando a entrada de um aplicativo de usuário.
- **Detecção de Obstáculos**: Um sensor ultrassônico (HC-SR04) montado na frente do carrinho mede continuamente a distância de objetos.
- **Feedback Tátil (Háptico)**: Motores de vibração (localizados no cabo do carrinho) são acionados quando um obstáculo é detectado a 15 cm ou menos, alertando o usuário sobre uma colisão iminente.
- **Monitoramento de Status**: O carrinho publica sua distância de obstáculos em um tópico MQTT, permitindo o monitoramento.

## 4. Visão Futura do Projeto

Este protótipo é a base para um sistema muito mais completo, que incluirá:

- **Aplicativo Móvel Dedicado**: Comandos de voz e interface acessível.
- **Navegação Interna**: Mapeamento do supermercado e rotas guiadas por voz ("Leve-me ao corredor de massas").
- **Identificação de Produtos**: Uso de câmeras e IA (Inteligência Artificial) para ler rótulos e preços.
- **Modo "Siga-me"**: O carrinho segue o usuário de forma autônoma.

## 5. Estrutura do Repositório

- `/smartGuide/smartGuide.ino`: O código-fonte principal do firmware para o ESP32.
- `README.md`: Esta página (visão geral do projeto).
- `HARDWARE.md`: Lista de materiais (BOM), diagrama elétrico e guia de montagem.
- `SOFTWARE.md`: Instruções detalhadas sobre o firmware, configuração de rede e tópicos MQTT.

## 6. Como Contribuir

Ficamos felizes com o seu interesse! Sinta-se à vontade para abrir uma **Issue** para relatar bugs ou sugerir melhorias, ou um **Pull Request** com suas contribuições.

---
