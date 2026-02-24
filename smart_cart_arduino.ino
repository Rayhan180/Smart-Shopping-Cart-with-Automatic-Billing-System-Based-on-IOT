#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>

#define RFID_SS_PIN 10
#define RFID_RST_PIN 9

#define ESP32_RX_PIN 2
#define ESP32_TX_PIN 3

MFRC522 rfid(RFID_SS_PIN, RFID_RST_PIN);
LiquidCrystal_I2C lcd(0x27, 16, 2);
SoftwareSerial espUart(ESP32_RX_PIN, ESP32_TX_PIN);

// Convert UID bytes into a readable HEX string
String uidToHexString(MFRC522::Uid *uid)
{
  String result = "";

  for (byte i = 0; i < uid->size; i++)
  {
    if (uid->uidByte[i] < 0x10)
    {
      result += "0";
    }
    result += String(uid->uidByte[i], HEX);
  }

  result.toUpperCase();
  return result;
}

// Arduino initialization
void setup()
{
  Serial.begin(115200);
  espUart.begin(9600);

  SPI.begin();
  rfid.PCD_Init();

  lcd.init();
  lcd.backlight();

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Smart Cart");
  lcd.setCursor(0, 1);
  lcd.print("Scan RFID...");
}

// Main execution loop
void loop()
{
  if (!rfid.PICC_IsNewCardPresent())
  {
    return;
  }

  if (!rfid.PICC_ReadCardSerial())
  {
    return;
  }

  String uidStr = uidToHexString(&rfid.uid);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("UID:");
  lcd.setCursor(0, 1);
  lcd.print(uidStr);

  String msg = "<UID:" + uidStr + ">";
  espUart.println(msg);

  Serial.print("Sent to ESP32: ");
  Serial.println(msg);

  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();

  delay(1200);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Scan RFID...");
}
