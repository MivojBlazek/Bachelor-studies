#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <BluetoothSerial.h>

#include <stdio.h>

// TFT Display Pins
#define TFT_CS      5   // Chip Select
#define TFT_RST     17  // Reset
#define TFT_DC      2   // Data/Command
#define TFT_SDA     23  // MOSI
#define TFT_SCK     18  // Clock

#define BT_NAME     "ESP32_xblaze38"    // Name of bluetooth device

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
BluetoothSerial SerialBT;

void setupBluetoothDevice()
{
    SerialBT.begin(BT_NAME);
    SerialBT.enableSSP();
    Serial.println("ESP32 is ready to pair");

    uint8_t address[6];
    esp_read_mac(address, ESP_MAC_BT);
    Serial.printf("Bluetooth Address: %02X:%02X:%02X:%02X:%02X:%02X\n",
                  address[0], address[1], address[2],
                  address[3], address[4], address[5]);
}

void setup(void)
{

    Serial.begin(115200);
    SPI.begin(TFT_SCK, -1, TFT_SDA, TFT_CS);
  
    tft.initR(INITR_144GREENTAB);

    setupBluetoothDevice();

    // Draws a square //! DEBUG
    for (int x = 40; x < 54; x++)
    {
        for (int y = 40; y < 54; y++)
        {
            tft.drawPixel(x, y, ST7735_RED);
        }
    }
}

void loop()
{
    if (SerialBT.available())
    {
        //! DEBUG
        String receivedData = SerialBT.readStringUntil('\n');
        Serial.print("Received: ");
        Serial.println(receivedData);
    }
    delay(100);
}