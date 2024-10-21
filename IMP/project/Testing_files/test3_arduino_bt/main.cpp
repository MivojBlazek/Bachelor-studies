#include <Arduino.h>
#include <BluetoothSerial.h>

BluetoothSerial SerialBT;

void setup()
{
    Serial.begin(115200);
    SerialBT.begin("ESP32_xblaze38");
    SerialBT.enableSSP();
    Serial.println("ESP32 is ready to pair");

    uint8_t address[6];
    esp_read_mac(address, ESP_MAC_BT);
    Serial.printf("Bluetooth Address: %02X:%02X:%02X:%02X:%02X:%02X\n",
                  address[0], address[1], address[2],
                  address[3], address[4], address[5]);
}

void loop()
{
    if (SerialBT.available())
    {
        String receivedData = SerialBT.readStringUntil('\n');
        Serial.print("Received: ");
        Serial.println(receivedData);
    }
    delay(100);
}
