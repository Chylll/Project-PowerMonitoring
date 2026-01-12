#include <EasyMQTT.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <PZEM004Tv30.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>

/* ================= WIFI ================= */
#define WIFI_SSID "Monika"
#define WIFI_PASS "12345789"

/* ================= TELEGRAM ================= */
#define BOT_TOKEN "8291576423:AAFNk5-E4frK7rMHQVX9A5l5UKcjmNRg1_M"
#define CHAT_ID   "8351133820"

/* ================= EASY MQTT ================= */
#define EASY_MQTT_TOKEN "9Gw1O3mxaq0NhURFwEnm"

/* ================= PIN ================= */
#define LED_BIRU   2
#define LED_HIJAU  4

/* ================= OBJECT ================= */
EasyMQTT mqtt(EASY_MQTT_TOKEN);
WiFiClientSecure client;

#define PZEM_RX 25
#define PZEM_TX 26
HardwareSerial PZEMSerial(2);
PZEM004Tv30 pzem(PZEMSerial, PZEM_RX, PZEM_TX);

Adafruit_SSD1306 display(128, 32, &Wire, -1);

/* ================= TIMER ================= */
unsigned long lastSend = 0;
const unsigned long interval = 15000;

/* ================= TELEGRAM FUNCTION ================= */
void sendTelegram(String message) {
  if (WiFi.status() != WL_CONNECTED) return;

  HTTPClient https;
  client.setInsecure();

  message.replace(" ", "%20");
  message.replace("\n", "%0A");

  String url = "https://api.telegram.org/bot" + String(BOT_TOKEN) +
               "/sendMessage?chat_id=" + String(CHAT_ID) +
               "&text=" + message;

  https.begin(client, url);
  https.GET();
  https.end();
}

/* ================= SETUP ================= */
void setup() {
  Serial.begin(115200);

  pinMode(LED_BIRU, OUTPUT);
  pinMode(LED_HIJAU, OUTPUT);

  PZEMSerial.begin(9600, SERIAL_8N1, PZEM_RX, PZEM_TX);

  Wire.begin(21, 22);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Connecting WiFi...");
  display.display();

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("WiFi Connected");
  display.display();

  mqtt.begin(WIFI_SSID, WIFI_PASS);
}

/* ================= LOOP ================= */
void loop() {
  mqtt.loop();

  if (millis() - lastSend >= interval) {
    lastSend = millis();

    float voltage   = pzem.voltage();
    float current   = pzem.current();
    float power     = pzem.power();
    float energy    = pzem.energy();
    float frequency = pzem.frequency();
    float pf        = pzem.pf();

    if (!isnan(voltage)) {

      digitalWrite(LED_BIRU, current > 0.06);
      digitalWrite(LED_HIJAU, voltage > 220.0);

      display.clearDisplay();
      display.setCursor(0, 0);
      display.printf("V:%.1f I:%.2f", voltage, current);
      display.setCursor(0, 11);
      display.printf("P:%.1f PF:%.2f", power, pf);
      display.setCursor(0, 22);
      display.printf("E:%.2f F:%.1f", energy, frequency);
      display.display();

      mqtt.publish("V0", String(current, 2));
      mqtt.publish("V1", String(voltage, 1));
      mqtt.publish("V2", String(power, 1));
      mqtt.publish("V3", String(energy, 3));
      mqtt.publish("V4", String(pf, 2));
      mqtt.publish("V5", String(frequency, 1));

      String pesan =
        "MONITORING LISTRIK\n"
        "------------------\n"
        "Tegangan  : " + String(voltage,1) + " V\n"
        "Arus      : " + String(current,2) + " A\n"
        "Daya      : " + String(power,1) + " W\n"
        "PowerFact : " + String(pf,2) + "\n"
        "Frekuensi : " + String(frequency,1) + " Hz\n"
        "Energi    : " + String(energy,3) + " kWh\n" ;

      sendTelegram(pesan);

      Serial.println("Data terkirim MQTT & Telegram");

    } else {
      display.clearDisplay();
      display.setCursor(0, 10);
      display.println("PZEM ERROR!");
      display.display();
      Serial.println("Gagal baca PZEM");
    }
  }
}
