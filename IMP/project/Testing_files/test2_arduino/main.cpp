#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>

// TFT Display Pins
#define TFT_CS      5   // Chip Select
#define TFT_RST     17  // Reset
#define TFT_DC      2  // Data/Command
#define TFT_SDA     23  // MOSI
#define TFT_SCK     18  // Clock
#define LED_PIN     27   // LED Backlight Pin


void setup()
{
  // Create a display object
  Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);

  SPI.begin(TFT_SCK, -1, TFT_SDA, TFT_CS);
  
  tft.initR(INITR_144GREENTAB);
  tft.fillScreen(ST7735_BLACK);

  for (int x = 40; x < 54; x++)
  {
      for (int y = 40; y < 54; y++)
      {
          tft.drawPixel(x, y, ST7735_RED);
      }
  }
  tft.drawPixel(10, 10, ST7735_RED);   // Draw a red pixel
  tft.drawPixel(20, 20, ST7735_GREEN);  // Draw a green pixel
  tft.drawPixel(30, 30, ST7735_BLUE);   // Draw a blue pixel
  tft.drawPixel(40, 40, ST7735_WHITE);  // Draw a white pixel
}

void loop()
{
  //
}

