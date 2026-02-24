#include <WiFi.h>
#include <HTTPClient.h>

const char *WIFI_SSID = "Rayhan_HomeNet";
const char *WIFI_PASS = "Ryh#47kLm92!";

const char *SERVER_URL = "http://192.168.1.120:8080/rfid";

HardwareSerial &cartUart = Serial2;
const int UART_RX_PIN = 16;
const int UART_TX_PIN = 17;
const int UART_BAUD  = 9600;

// Connect ESP32 to Wi-Fi network
void connectWiFi()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  unsigned long startAttempt = millis();

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);

    // simple timeout protection
    if (millis() - startAttempt > 20000)
    {
      startAttempt = millis();
      WiFi.disconnect();
      WiFi.begin(WIFI_SSID, WIFI_PASS);
    }
  }
}

// Send UID JSON to backend
bool sendUidToServer(const String &uid)
{
  if (WiFi.status() != WL_CONNECTED)
  {
    connectWiFi();
  }

  HTTPClient http;
  http.begin(SERVER_URL);
  http.addHeader("Content-Type", "application/json");

  String payload = "{\"uid\":\"" + uid + "\"}";
  int httpCode = http.POST(payload);
  http.end();

  return (httpCode > 0);
}

// Parse UART frame like <UID:XXXXXXXX>
bool parseUidFrame(const String &line, String &uidOut)
{
  int start = line.indexOf("<UID:");
  int end   = line.indexOf(">");

  if (start == -1 || end == -1)
  {
    return false;
  }

  int uidStart = start + 5;
  if (uidStart >= end)
  {
    return false;
  }

  uidOut = line.substring(uidStart, end);
  uidOut.trim();

  return (uidOut.length() > 0);
}

// ESP32 setup
void setup()
{
  Serial.begin(115200);
  cartUart.begin(UART_BAUD, SERIAL_8N1, UART_RX_PIN, UART_TX_PIN);

  connectWiFi();

  Serial.println("Rayhan Cart Gateway Ready");
}

// Main processing loop
void loop()
{
  if (cartUart.available())
  {
    String line = cartUart.readStringUntil('\n');
    line.trim();

    Serial.print("UART RX: ");
    Serial.println(line);

    String uid;

    if (parseUidFrame(line, uid))
    {
      Serial.print("UID Parsed: ");
      Serial.println(uid);

      bool ok = sendUidToServer(uid);

      if (ok)
      {
        Serial.println("POST success");
      }
      else
      {
        Serial.println("POST failed");
      }
    }
    else
    {
      Serial.println("Invalid frame");
    }
  }
}