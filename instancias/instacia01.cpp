//https://wokwi.com/projects/398349002805871617
#include <DHTesp.h>
#include <EspMQTTClient.h>


char SSIDName[] = "Wokwi-GUEST";
char SSIDPass[] = "";

const int DHT_PIN = 15;
const int PIN_TRIG = 13;
const int PIN_ECHO = 12;

char BrokerURL[] = "broker.hivemq.com";
char BrokerUserName[] = "";
char BrokerPassword[] = "";
char MQTTClientName[] = "clientId-Lzk7fLfMpazzz";
int BrokerPort = 1883;

String TopicoPrefixo = "PROJETO-IOT_05H-01";
String Topico_01 = TopicoPrefixo+"/Temperatura";
String Topico_02 = TopicoPrefixo+"/Distancia";

DHTesp dhtSensor;

EspMQTTClient clienteMQTT(SSIDName, SSIDPass, BrokerURL, BrokerUserName, BrokerPassword, MQTTClientName, BrokerPort); 

void onConnectionEstablished() {
}

float calcularDistancia() {
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);

  long duration = pulseIn(PIN_ECHO, HIGH);
  float distance = duration / 58.5;
  return distance;
}

void enviarDados() {
  TempAndHumidity temp_umid = dhtSensor.getTempAndHumidity(); 
    
  clienteMQTT.publish(Topico_01, String(temp_umid.temperature, 2)); 
  float distancia = calcularDistancia();
  clienteMQTT.publish(Topico_02, String(distancia, 2));
}

void setup() {
  Serial.begin(9600);
  
  dhtSensor.setup(DHT_PIN, DHTesp::DHT22);

  clienteMQTT.enableDebuggingMessages();

  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
}

void loop() {
  
  clienteMQTT.loop(); 
  enviarDados(); 

  if (clienteMQTT.isWifiConnected() == 1) {
    Serial.println("Conectado ao WiFi!");
  } else {
    Serial.println("Nao conectado ao WiFi!");
  }

  if (clienteMQTT.isMqttConnected() == 1) {
    Serial.println("Conectado ao broker MQTT!");
  } else {
    Serial.println("Nao conectado ao broker MQTT!");
  }

  Serial.println("Nome do cliente: " + String(clienteMQTT.getMqttClientName())
    + " / Broker MQTT: " + String(clienteMQTT.getMqttServerIp())
    + " / Porta: " + String(clienteMQTT.getMqttServerPort())
  );

  delay(5000);
}