#include <ESP8266WiFi.h>
#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ThingSpeak.h>

// === Sensor DHT22 ===
#define DHTPIN D4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// === LCD I2C 16x4 ===
LiquidCrystal_I2C lcd(0x27, 16, 4);

// === LED Output ===
#define LED_MERAH D5
#define LED_BIRU  D6

// === Batas Suhu & Kelembapan ===
const float SUHU_MIN = 16.0;
const float SUHU_MAX = 25.0;
const float HUMI_MIN = 40.0;
const float HUMI_MAX = 60.0;

// === WiFi & ThingSpeak ===
const char* ssid = "NAMA WIFI";
const char* password = "PASSWORD WIFI";
const unsigned long channelID = CHANNEL_ID_THINKSPEAK;
const char* writeAPIKey = "API_THINKSPEAK";
WiFiClient client;

// === Scroll (Marquee) Settings ===
String scrollText = "";
int scrollIndex = 0;
unsigned long lastScrollTime = 0;
const unsigned long scrollDelay = 300;

void setup() {
  Serial.begin(115200);
  dht.begin();
  lcd.init();
  lcd.backlight();

  pinMode(LED_MERAH, OUTPUT);
  pinMode(LED_BIRU, OUTPUT);

  lcd.setCursor(0, 0);
  lcd.print("Connect WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("WiFi OK: ");
  lcd.setCursor(0, 1);
  lcd.print(WiFi.localIP());

  ThingSpeak.begin(client);
  delay(2000);
}

void loop() {
  float suhu = dht.readTemperature();
  float humi = dht.readHumidity();

  lcd.clear();

  if (isnan(suhu) || isnan(humi)) {
    Serial.println("Sensor Error");
    lcd.setCursor(0, 0);
    lcd.print("Sensor Error!");
    digitalWrite(LED_MERAH, HIGH);
    digitalWrite(LED_BIRU, LOW);
    delay(5000);
    return;
  }

  // Serial Monitor
  Serial.print("Suhu: ");
  Serial.print(suhu);
  Serial.print(" C | Humi: ");
  Serial.print(humi);
  Serial.println(" %");

  // LCD
  lcd.setCursor(0, 0);
  lcd.print("Suhu: ");
  lcd.print(suhu, 1);
  lcd.print((char)223);
  lcd.print("C");

  lcd.setCursor(0, 1);
  lcd.print("Humi: ");
  lcd.print(humi, 1);
  lcd.print("%");

  // Status check
  bool suhuNormal = (suhu >= SUHU_MIN && suhu <= SUHU_MAX);
  bool humiNormal = (humi >= HUMI_MIN && humi <= HUMI_MAX);
  bool isNormal = suhuNormal && humiNormal;

  String alertMsg = "";

  if (isNormal) {
    digitalWrite(LED_BIRU, HIGH);
    digitalWrite(LED_MERAH, LOW);
    lcd.setCursor(0, 2);
    lcd.print("Status: NORMAL");
    alertMsg = "Suhu & Kelembapan Normal";
  } else {
    digitalWrite(LED_MERAH, HIGH);
    digitalWrite(LED_BIRU, LOW);
    lcd.setCursor(0, 2);
    lcd.print("Status: ALERT");

    if (suhu < SUHU_MIN) alertMsg += "Suhu Rendah! ";
    if (suhu > SUHU_MAX) alertMsg += "Suhu Tinggi! ";
    if (humi < HUMI_MIN) alertMsg += "Kelembapan Rendah! ";
    if (humi > HUMI_MAX) alertMsg += "Kelembapan Tinggi! ";
  }

  // Scroll Marquee Baris ke-4
  scrollText = alertMsg + "   ";
  if (millis() - lastScrollTime > scrollDelay) {
    lastScrollTime = millis();

    String displayPart = scrollText.substring(scrollIndex, scrollIndex + 16);
    lcd.setCursor(0, 3);
    lcd.print(displayPart);

    scrollIndex++;
    if (scrollIndex + 16 > scrollText.length()) {
      scrollIndex = 0;
    }
  }

  // Kirim ke ThingSpeak
  ThingSpeak.setField(1, suhu);
  ThingSpeak.setField(2, humi);
  int response = ThingSpeak.writeFields(channelID, writeAPIKey);
  Serial.println(response == 200 ? "ThingSpeak OK" : "TS Error");

  delay(5000); // jeda antar pembacaan
}
