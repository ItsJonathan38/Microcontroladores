#include <WiFi.h>
#include <AsyncMqttClient.h>
#include <DHT.h>
#include <EEPROM.h>
#include <LiquidCrystal_I2C.h>


#define EEPROM_SIZE 100
#define DHTPIN 4
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2); 

AsyncMqttClient mqttClient;

// Broker MQTT
const char* mqttServer = "broker.emqx.io";
const int mqttPort = 1883;
const char* topic_temp = "temperatura/tor";
const char* topic_hum = "humedad/tor";

// Variables globales compartidas entre tareas
float temperatura = 0.0;
float humedad = 0.0;
SemaphoreHandle_t mutexData;

void guardarWiFiEEPROM(const char* ssid, const char* password) {
  EEPROM.begin(EEPROM_SIZE);
  for (int i = 0; i < 32; i++) EEPROM.write(i, ssid[i]);
  for (int i = 0; i < 64; i++) EEPROM.write(32 + i, password[i]);
  EEPROM.commit();
}

void leerWiFiEEPROM(char* ssid, char* password) {
  EEPROM.begin(EEPROM_SIZE);
  for (int i = 0; i < 32; i++) ssid[i] = EEPROM.read(i);
  for (int i = 0; i < 64; i++) password[i] = EEPROM.read(32 + i);
}

void conectarWiFi() {
  char ssid[33];
  char password[65];
  leerWiFiEEPROM(ssid, password);

  Serial.print("Conectando a WiFi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi conectado");
}

void onMqttConnect(bool sessionPresent) {
  Serial.println("Conectado al broker MQTT");
}

void onMqttDisconnect(AsyncMqttClientDisconnectReason reason) {
  Serial.println("Desconectado del broker MQTT. Reintentando...");
  delay(5000);
  mqttClient.connect();
}


//TAREA 1 - PRIORIDAD ALTA: Lectura del DHT11
void taskReadSensor(void * parameter) {
  for (;;) {
    float t = dht.readTemperature();
    float h = dht.readHumidity();

    if (!isnan(t) && !isnan(h)) {
      xSemaphoreTake(mutexData, portMAX_DELAY);
      temperatura = t;
      humedad = h;
      xSemaphoreGive(mutexData);
      Serial.printf("Sensor leído: T=%.2f°C H=%.2f%%\n", t, h);
    } else {
      Serial.println("Error leyendo DHT11");
    }
    vTaskDelay(pdMS_TO_TICKS(1000)); 
  }
}

// TAREA 2 - PRIORIDAD MEDIA: Enviar datos al broker MQTT
void taskSendMQTT(void * parameter) {
  for (;;) {
    if (mqttClient.connected()) {
      xSemaphoreTake(mutexData, portMAX_DELAY);
      float t = temperatura;
      float h = humedad;
      xSemaphoreGive(mutexData);

      char tempStr[8], humStr[8];
      dtostrf(t, 6, 2, tempStr);
      dtostrf(h, 6, 2, humStr);

      mqttClient.publish(topic_temp, 1, true, tempStr);
      mqttClient.publish(topic_hum, 1, true, humStr);

      Serial.printf("Datos enviados a MQTT -> T: %.2f°C | H: %.2f%%\n", t, h);
    }
    vTaskDelay(pdMS_TO_TICKS(2000)); 
  }
}

// TAREA 3 - PRIORIDAD BAJA: Mostrar datos en LCD
void taskDisplayLCD(void * parameter) {
  for (;;) {
    xSemaphoreTake(mutexData, portMAX_DELAY);
    float t = temperatura;
    float h = humedad;
    xSemaphoreGive(mutexData);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Temp: ");
    lcd.print(t, 1);
    lcd.print(" C");

    lcd.setCursor(0, 1);
    lcd.print("Hum: ");
    lcd.print(h, 1);
    lcd.print(" %");

    vTaskDelay(pdMS_TO_TICKS(3000)); 
  }
}


void setup() {
  Serial.begin(115200);
  dht.begin();
  lcd.init();
  lcd.backlight();

  
  conectarWiFi();

  mqttClient.onConnect(onMqttConnect);
  mqttClient.onDisconnect(onMqttDisconnect);
  mqttClient.setServer(mqttServer, mqttPort);
  mqttClient.connect();


  mutexData = xSemaphoreCreateMutex();


  xTaskCreatePinnedToCore(taskReadSensor, "LeerSensor", 4096, NULL, 3, NULL, 1);
  xTaskCreatePinnedToCore(taskSendMQTT, "EnviarMQTT", 4096, NULL, 2, NULL, 1);
  xTaskCreatePinnedToCore(taskDisplayLCD, "MostrarLCD", 4096, NULL, 1, NULL, 1);
}

void loop() {
}


