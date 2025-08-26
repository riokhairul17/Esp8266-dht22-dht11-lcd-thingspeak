# 🌡️ ESP8266 DHT22 / DHT11 LCD ThingSpeak

Proyek IoT menggunakan **ESP8266 (NodeMCU)**, **Sensor DHT22**, dan **LCD I2C 16x4** untuk monitoring suhu & kelembapan, dengan integrasi ke **ThingSpeak** sebagai platform IoT cloud.

---

## 🚀 Fitur
- Membaca **suhu** (°C) dan **kelembapan** (%RH) menggunakan DHT22.
- Menampilkan data real-time di **LCD I2C 16x4**.
- Memberikan status:
  - ✅ **Normal** → LED biru menyala.
  - ⚠️ **Alert** (suhu/kelembapan di luar batas) → LED merah menyala + pesan peringatan berjalan (scroll).
- Mengirim data ke **ThingSpeak** setiap 5 detik.
- Pesan peringatan ditampilkan secara **marquee (scrolling text)** di baris ke-4 LCD.

---

## 🛠️ Hardware
- ESP8266 (NodeMCU V3)
- DHT22 (Temperature & Humidity Sensor)
- LCD I2C 16x4
- LED Merah + Resistor
- LED Biru + Resistor
- Breadboard & Kabel jumper

---

## 🔌 Wiring
| Komponen | ESP8266 |
|----------|----------|
| DHT22 Data | D4 |
| LED Merah  | D5 |
| LED Biru   | D6 |
| LCD SDA    | D2 |
| LCD SCL    | D1 |
| VCC & GND  | 3.3V / GND |

---

## 💻 Library Arduino
Pastikan sudah install library berikut:
- [ESP8266WiFi](https://github.com/esp8266/Arduino)
- [DHT sensor library](https://github.com/adafruit/DHT-sensor-library)
- [Adafruit Unified Sensor](https://github.com/adafruit/Adafruit_Sensor)
- [LiquidCrystal_I2C](https://github.com/johnrickman/LiquidCrystal_I2C)
- [ThingSpeak](https://github.com/mathworks/thingspeak-arduino)

---

## ⚙️ Konfigurasi
Edit bagian berikut di kode `main.ino` sesuai WiFi & akun ThingSpeak kamu:

```cpp
const char* ssid = "NAMA_WIFI";
const char* password = "PASSWORD_WIFI";
const unsigned long channelID = 123456;
const char* writeAPIKey = "APIKEY_THINGSPEAK";

