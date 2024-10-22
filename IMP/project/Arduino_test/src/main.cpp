#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <BluetoothSerial.h>

// TFT Display Pins
#define TFT_CS      5   // Chip Select
#define TFT_RST     27  // Reset
#define TFT_DC      2   // Data/Command
#define TFT_SDA     23  // MOSI
#define TFT_SCK     18  // Clock

#define TFT_WIDTH   128
#define TFT_HEIGHT  128

#define BT_NAME     "ESP32_xblaze38"    // Name of bluetooth device

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
BluetoothSerial SerialBT;
int currentRow = 0;
int currentColumn = 0;

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

void printText(String text)
{
    tft.fillScreen(ST7735_BLACK);
    tft.setTextSize(2);
    tft.setCursor(0, 0);
    tft.setTextColor(ST7735_WHITE);
    tft.print(text);
}

void printRowOfImage()
{
    while (currentRow < TFT_HEIGHT && SerialBT.available())
    {
        if (currentColumn == TFT_WIDTH)
        {
            currentColumn = 0;
            currentRow++;
            if (currentRow == TFT_HEIGHT)
            {
                return;
            }
        }

        if (SerialBT.available() >= 2)
        {
            uint8_t highByte = SerialBT.read();
            uint8_t lowByte = SerialBT.read();

            uint16_t color = (highByte << 8) | lowByte;

            tft.drawPixel(currentColumn, currentRow, color);
            currentColumn++;
        }
    }
}

void emptySerial()
{
    while (SerialBT.available())
    {
        SerialBT.read();
    }
}

void setup(void)
{
    Serial.begin(115200);
    SPI.begin(TFT_SCK, -1, TFT_SDA, TFT_CS);
  
    tft.initR(INITR_144GREENTAB);
    tft.setRotation(2);
    tft.fillScreen(ST7735_BLACK);

    setupBluetoothDevice();
}

void loop()
{
    if (SerialBT.available())
    {
        uint8_t type = SerialBT.read();

        if (type == 0x00)
        {
            // First image chunk
            currentRow = 0;
            currentColumn = 0;
            tft.fillScreen(ST7735_BLACK);
            tft.setCursor(0, 0);
            printRowOfImage();
            emptySerial();
        }
        else if (type == 0x01)
        {
            // Next image chunks
            printRowOfImage();
            emptySerial();
        }
        else if (type == 0x02)
        {
            // Text
            String receivedData = SerialBT.readStringUntil('\n');
            printText(receivedData);
            emptySerial();
        }
    }
    delay(10);
}