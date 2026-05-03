# IoT Power Monitoring System (ESP32 + PZEM004T + MQTT + Telegram)

Sistem monitoring konsumsi listrik berbasis Internet of Things (IoT) menggunakan ESP32 dan sensor PZEM004T v3.0 untuk membaca parameter listrik secara real-time. Data dikirim ke platform MQTT dan notifikasi dikirim melalui Telegram.

---

## Deskripsi Sistem
Sistem ini digunakan untuk memonitor parameter listrik seperti tegangan, arus, daya, energi, frekuensi, dan power factor secara real-time. Data ditampilkan pada OLED display serta dikirim ke cloud menggunakan MQTT dan Telegram.

---

## Fitur Utama
- Monitoring tegangan, arus, daya, energi, frekuensi, dan power factor
- Pengiriman data real-time ke MQTT
- Notifikasi otomatis ke Telegram setiap interval tertentu
- Tampilan data langsung pada OLED display
- Indikator LED berdasarkan kondisi listrik
- Sistem berjalan berbasis WiFi

---

## Teknologi yang Digunakan

### Hardware
- ESP32  
- PZEM004T v3.0  
- OLED SSD1306 (I2C)  
- LED Indicator  
- Power Supply  

### Software
- Arduino IDE  
- Library EasyMQTT  
- Adafruit SSD1306  
- HTTPClient (Telegram API)  

### Protocol
- WiFi Communication  
- MQTT  
- HTTPS (Telegram API)  
- I2C (OLED)  
- UART (PZEM004T)  

---

## Cara Kerja Sistem
1. ESP32 terhubung ke jaringan WiFi  
2. Sensor PZEM membaca parameter listrik  
3. Data ditampilkan pada OLED display  
4. Data dikirim ke MQTT server  
5. Sistem mengirim notifikasi ke Telegram setiap 15 detik  
6. LED indikator menyala berdasarkan kondisi tegangan dan arus  

---
