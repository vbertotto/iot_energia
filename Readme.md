# ESP32 MQTT Temperature and LED Control

Este projeto utiliza um ESP32 para ler a temperatura de um sensor DHT22, conectar-se a um servidor MQTT para publicar dados de temperatura, além de controlar um LED com base na temperatura e no horário obtido via NTP (Network Time Protocol).

## Funcionalidades

- Leitura de temperatura do sensor DHT22.
- Controle de LED baseado na temperatura.
- Controle de LED baseado no horário (entre 08h e 18h o LED acende).
- Envio da temperatura lida para o broker MQTT no tópico `FIT/PUB_Temperatura`.
- Recebe mensagens de controle no tópico `FIT/SUB` para ligar ou desligar o LED remotamente.
- Conexão automática ao WiFi e ao servidor MQTT.

## Componentes Necessários

- ESP32
- Sensor de temperatura DHT22
- LED
- Conexão WiFi
- Broker MQTT (utilizando o servidor público `test.mosquitto.org`)

## Pinagem

- **DHT22**: Conectado ao pino `32` do ESP32.
- **LED**: Conectado ao pino `4` do ESP32.

## Bibliotecas Necessárias

Para compilar o código, as seguintes bibliotecas são necessárias:

- `DHTesp.h`: Para o sensor DHT22.
- `NTPClient.h`: Para obter a hora via NTP.
- `WiFi.h`: Para conectar o ESP32 à rede WiFi.
- `WiFiClient.h`: Para criar o cliente WiFi.
- `PubSubClient.h`: Para comunicação com o broker MQTT.

Você pode instalar essas bibliotecas na IDE Arduino através do Gerenciador de Bibliotecas.

## Conexão WiFi

- **SSID**: Wokwi-GUEST
- **Senha**: (vazio)
- **Canal**: 6

## Funcionamento

1. O ESP32 se conecta à rede WiFi e ao servidor MQTT.
2. O sensor DHT22 é inicializado para realizar leituras de temperatura.
3. O LED será ligado se a temperatura for maior que 28°C ou se a hora estiver entre 08:00 e 18:00.
4. A cada 15 segundos, a temperatura será publicada no tópico MQTT `FIT/PUB_Temperatura`.
5. O ESP32 também escuta o tópico `FIT/SUB`. Se receber a mensagem `"1"`, o LED acenderá, e se receber `"0"`, o LED será apagado.
