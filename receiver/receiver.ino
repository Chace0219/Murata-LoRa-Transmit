#include <SPI.h>
#include <LoRa.h>

void setup()
{
  Serial.begin(115200);
  // while (!Serial);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(2000);
  Serial.println("LoRa Receiver");

  if (!LoRa.begin(915E6))
  {
    Serial.println("Starting LoRa failed!");
    while (1)
      ;
  }

  /** Setup LoRa Parameters */
  LoRa.setPreambleLength(6);      // Preamble 6 bytes Length
  LoRa.enableCrc();               // CRC on
  LoRa.setTxPower(20);            // Tx power 20
  LoRa.setCodingRate4(5);         // Coding Rate 4/5
  LoRa.setSyncWord(0x89);         // SyncWord 0x89
  LoRa.setSpreadingFactor(7);     // Spreading Factor 7 
  LoRa.setSignalBandwidth(250E3); // Bandwidth 250kHz
  /** Setup LoRa Parameters */
}

void loop()
{
  int packetSize = LoRa.parsePacket();
  if (packetSize)
  {
    // Received a packet
    Serial.print("\r\nReceived packet ");
    // Read packet payload
    while (LoRa.available())
    {
      char temp = (char)LoRa.read();
      Serial.print(temp);
    }
    Serial.println();

    // Print SNR/RSSI etc
    Serial.print("SNR: ");
    Serial.print(LoRa.packetSnr());
    Serial.print(", RSSI ");
    Serial.print(LoRa.packetRssi());
    Serial.println();
  }
}
