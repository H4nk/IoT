#define BLYNK_TEMPLATE_ID "TMPL6SjS-w3Fy"
#define BLYNK_TEMPLATE_NAME "Sensor Kelembapan"
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = "xxxxxxxxxxxxxxxxx";  // Token Blynk
char ssid[] = "H4nk";                               // Nama WiFi
char pass[] = "xxxxxxxxxxxx";                         // Password WiFi

const int sensorPin = A0; // Pin sensor kelembapan tanah
BlynkTimer timer;         // Timer untuk pengecekan koneksi

bool lastWiFiStatus = false;   // Status koneksi WiFi terakhir
bool lastBlynkStatus = false;  // Status koneksi Blynk terakhir

void setup() {
  Serial.begin(115200);
  connectToWiFiAndBlynk(); // Fungsi koneksi awal
  
  pinMode(sensorPin, INPUT);

  // Atur timer untuk menjalankan fungsi pembacaan sensor secara berkala
  timer.setInterval(1000L, sendSensorData); // Setiap 1 detik
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    connectToWiFiAndBlynk(); // Rekoneksi jika WiFi terputus
  } else if (!Blynk.connected()) {
    connectToWiFiAndBlynk(); // Rekoneksi jika Blynk terputus
  }
  
  Blynk.run();
  timer.run(); // Jalankan timer untuk fungsi pembacaan sensor

  checkConnectionStatus(); // Cek dan tampilkan status koneksi
}

// Fungsi untuk menghubungkan ke WiFi dan Blynk
void connectToWiFiAndBlynk() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Menghubungkan ke WiFi...");
    WiFi.begin(ssid, pass);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("\nWiFi Terhubung");
  }

  if (!Blynk.connected()) {
    Serial.println("Menghubungkan ke Blynk...");
    Blynk.config(auth);
    Blynk.connect(); // Menyambungkan ke server Blynk
    if (Blynk.connected()) {
      Blynk.syncAll(); // Sinkronisasi data Blynk jika koneksi berhasil
      Serial.println("Blynk Terhubung dan Data Tersinkronisasi");
    }
  }
}

// Fungsi untuk membaca data sensor dan mengirim ke Blynk
void sendSensorData() {
  if (Blynk.connected()) { // Hanya kirim data jika terhubung ke Blynk
    int kelembapan = analogRead(sensorPin);
    int persentaseKelembapan = map(kelembapan, 1023, 0, 0, 100); // Kalibrasi sesuai sensor
    Blynk.virtualWrite(V1, kelembapan); // Kirim data ke Virtual Pin V1
    Serial.print("Kelembapan: ");
    Serial.print(kelembapan);
    Serial.print(" %");
        Serial.print(" |  Persentase Kelembapan : ");
    Serial.print(persentaseKelembapan);
    Serial.println(" %");

  } else {
    Serial.println("Tidak terhubung ke Blynk, data tidak terkirim.");
  }
}

// Fungsi untuk memeriksa dan menampilkan status koneksi WiFi dan Blynk
void checkConnectionStatus() {
  bool currentWiFiStatus = (WiFi.status() == WL_CONNECTED);
  bool currentBlynkStatus = Blynk.connected();

  // Cek perubahan status koneksi WiFi
  if (currentWiFiStatus != lastWiFiStatus) {
    if (currentWiFiStatus) {
      Serial.println("Status WiFi: Terhubung");
    } else {
      Serial.println("Status WiFi: Terputus");
    }
    lastWiFiStatus = currentWiFiStatus;
  }

  // Cek perubahan status koneksi Blynk
  if (currentBlynkStatus != lastBlynkStatus) {
    if (currentBlynkStatus) {
      Serial.println("Status Blynk: Terhubung");
    } else {
      Serial.println("Status Blynk: Terputus");
    }
    lastBlynkStatus = currentBlynkStatus;
  }
}
