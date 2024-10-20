/**
 * @file main.cpp
 * @author Patrik Sehnoutek <xsehno01@stud.fit.vutbr.cz>
 * @brief ESP32 with TFT display
 * @date 2022-12-11
 * 
 * @copyright Copyright (c) 2022
 */

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <SPI.h>
#include <WiFi.h>
#include <WebServer.h>
#include <stdio.h>

// Port mapping according to display connection
#define TFT_CS      5
#define TFT_RST     17 
#define TFT_DC      2

// WiFi configuration
const char* ssid = "Vodafone-DE84";
const char* password = "FctL6drcRHubC6Dh";

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
WebServer server(80);

/**
 * @brief Html code of index page
 * 
 * @return const char* 
 */
const char* htmlCode() {
  return "<!DOCTYPE html> \
          <html lang='en'> \
          <head> \
            <meta charset='UTF-8'> \
            <meta http-equiv='X-UA-Compatible' content='IE=edge'> \
            <meta name='viewport' content='width=device-width, initial-scale=1.0'> \
            <title>ESP32 Server</title> \
            <style> \
              * { \
                  box-sizing: border-box; \
              } \
              \
              .container { \
                  display: grid; \
                  grid-template-rows: repeat(16, 30px); \
                  grid-template-columns: repeat(16, 30px); \
                  row-gap: 0; \
              } \
              input[type='checkbox'] { \
                  appearance: none; \
                  display: grid; \
                  margin: 0; \
              } \
              input[type='checkbox']::before { \
                  content: ' '; \
                  position: relative; \
                  width: 28px; \
                  height: 28px; \
                  border: 1px solid black; \
                  cursor: pointer; \
              } \
              input[type='checkbox']:checked::before { \
                  background-color: aquamarine; \
              } \
            </style> \
          </head> \
          <body> \
            <form method='POST' action='/text'> \
              <label>Text: </label> \
              <input type='text' name='text'/> \
              <input type='submit' name='btn-send' value='Write'/> \
              </form> \
              <br> \
              <form action='/draw' method='POST'> \
                <div class='container'> \
                    <div class='row'> \
                      <input type='checkbox' name='0-0' id=''> \
                      <input type='checkbox' name='1-0' id=''> \
                      <input type='checkbox' name='2-0' id=''> \
                      <input type='checkbox' name='3-0' id=''> \
                      <input type='checkbox' name='4-0' id=''> \
                      <input type='checkbox' name='5-0' id=''> \
                      <input type='checkbox' name='6-0' id=''> \
                      <input type='checkbox' name='7-0' id=''> \
                      <input type='checkbox' name='8-0' id=''> \
                      <input type='checkbox' name='9-0' id=''> \
                      <input type='checkbox' name='10-0' id=''> \
                      <input type='checkbox' name='11-0' id=''> \
                      <input type='checkbox' name='12-0' id=''> \
                      <input type='checkbox' name='13-0' id=''> \
                      <input type='checkbox' name='14-0' id=''> \
                      <input type='checkbox' name='15-0' id=''> \
                    </div> \
                    <div class='row'> \
                      <input type='checkbox' name='0-1' id=''> \
                      <input type='checkbox' name='1-1' id=''> \
                      <input type='checkbox' name='2-1' id=''> \
                      <input type='checkbox' name='3-1' id=''> \
                      <input type='checkbox' name='4-1' id=''> \
                      <input type='checkbox' name='5-1' id=''> \
                      <input type='checkbox' name='6-1' id=''> \
                      <input type='checkbox' name='7-1' id=''> \
                      <input type='checkbox' name='8-1' id=''> \
                      <input type='checkbox' name='9-1' id=''> \
                      <input type='checkbox' name='10-1' id=''> \
                      <input type='checkbox' name='11-1' id=''> \
                      <input type='checkbox' name='12-1' id=''> \
                      <input type='checkbox' name='13-1' id=''> \
                      <input type='checkbox' name='14-1' id=''> \
                      <input type='checkbox' name='15-1' id=''> \
                    </div> \
                    <div class='row'> \
                      <input type='checkbox' name='0-2' id=''> \
                      <input type='checkbox' name='1-2' id=''> \
                      <input type='checkbox' name='2-2' id=''> \
                      <input type='checkbox' name='3-2' id=''> \
                      <input type='checkbox' name='4-2' id=''> \
                      <input type='checkbox' name='5-2' id=''> \
                      <input type='checkbox' name='6-2' id=''> \
                      <input type='checkbox' name='7-2' id=''> \
                      <input type='checkbox' name='8-2' id=''> \
                      <input type='checkbox' name='9-2' id=''> \
                      <input type='checkbox' name='10-2' id=''> \
                      <input type='checkbox' name='11-2' id=''> \
                      <input type='checkbox' name='12-2' id=''> \
                      <input type='checkbox' name='13-2' id=''> \
                      <input type='checkbox' name='14-2' id=''> \
                      <input type='checkbox' name='15-2' id=''> \
                    </div> \
                    <div class='row'> \
                      <input type='checkbox' name='0-3' id=''> \
                      <input type='checkbox' name='1-3' id=''> \
                      <input type='checkbox' name='2-3' id=''> \
                      <input type='checkbox' name='3-3' id=''> \
                      <input type='checkbox' name='4-3' id=''> \
                      <input type='checkbox' name='5-3' id=''> \
                      <input type='checkbox' name='6-3' id=''> \
                      <input type='checkbox' name='7-3' id=''> \
                      <input type='checkbox' name='8-3' id=''> \
                      <input type='checkbox' name='9-3' id=''> \
                      <input type='checkbox' name='10-3' id=''> \
                      <input type='checkbox' name='11-3' id=''> \
                      <input type='checkbox' name='12-3' id=''> \
                      <input type='checkbox' name='13-3' id=''> \
                      <input type='checkbox' name='14-3' id=''> \
                      <input type='checkbox' name='15-3' id=''> \
                    </div> \
                    <div class='row'> \
                      <input type='checkbox' name='0-4' id=''> \
                      <input type='checkbox' name='1-4' id=''> \
                      <input type='checkbox' name='2-4' id=''> \
                      <input type='checkbox' name='3-4' id=''> \
                      <input type='checkbox' name='4-4' id=''> \
                      <input type='checkbox' name='5-4' id=''> \
                      <input type='checkbox' name='6-4' id=''> \
                      <input type='checkbox' name='7-4' id=''> \
                      <input type='checkbox' name='8-4' id=''> \
                      <input type='checkbox' name='9-4' id=''> \
                      <input type='checkbox' name='10-4' id=''> \
                      <input type='checkbox' name='11-4' id=''> \
                    <input type='checkbox' name='12-4' id=''> \
                      <input type='checkbox' name='13-4' id=''> \
                      <input type='checkbox' name='14-4' id=''> \
                      <input type='checkbox' name='15-4' id=''> \
                    </div> \
                    <div class='row'> \
                      <input type='checkbox' name='0-5' id=''> \
                      <input type='checkbox' name='1-5' id=''> \
                      <input type='checkbox' name='2-5' id=''> \
                      <input type='checkbox' name='3-5' id=''> \
                      <input type='checkbox' name='4-5' id=''> \
                      <input type='checkbox' name='5-5' id=''> \
                      <input type='checkbox' name='6-5' id=''> \
                      <input type='checkbox' name='7-5' id=''> \
                      <input type='checkbox' name='8-5' id=''> \
                      <input type='checkbox' name='9-5' id=''> \
                      <input type='checkbox' name='10-5' id=''> \
                      <input type='checkbox' name='11-5' id=''> \
                      <input type='checkbox' name='12-5' id=''> \
                      <input type='checkbox' name='13-5' id=''> \
                      <input type='checkbox' name='14-5' id=''> \
                      <input type='checkbox' name='15-5' id=''> \
                    </div> \
                    <div class='row'> \
                      <input type='checkbox' name='0-6' id=''> \
                      <input type='checkbox' name='1-6' id=''> \
                      <input type='checkbox' name='2-6' id=''> \
                      <input type='checkbox' name='3-6' id=''> \
                      <input type='checkbox' name='4-6' id=''> \
                      <input type='checkbox' name='5-6' id=''> \
                      <input type='checkbox' name='6-6' id=''> \
                      <input type='checkbox' name='7-6' id=''> \
                      <input type='checkbox' name='8-6' id=''> \
                      <input type='checkbox' name='9-6' id=''> \
                      <input type='checkbox' name='10-6' id=''> \
                      <input type='checkbox' name='11-6' id=''> \
                      <input type='checkbox' name='12-6' id=''> \
                      <input type='checkbox' name='13-6' id=''> \
                      <input type='checkbox' name='14-6' id=''> \
                      <input type='checkbox' name='15-6' id=''> \
                    </div> \
                    <div class='row'> \
                      <input type='checkbox' name='0-7' id=''> \
                      <input type='checkbox' name='1-7' id=''> \
                      <input type='checkbox' name='2-7' id=''> \
                      <input type='checkbox' name='3-7' id=''> \
                      <input type='checkbox' name='4-7' id=''> \
                      <input type='checkbox' name='5-7' id=''> \
                      <input type='checkbox' name='6-7' id=''> \
                      <input type='checkbox' name='7-7' id=''> \
                      <input type='checkbox' name='8-7' id=''> \
                      <input type='checkbox' name='9-7' id=''> \
                      <input type='checkbox' name='10-7' id=''> \
                      <input type='checkbox' name='11-7' id=''> \
                      <input type='checkbox' name='12-7' id=''> \
                      <input type='checkbox' name='13-7' id=''> \
                      <input type='checkbox' name='14-7' id=''> \
                      <input type='checkbox' name='15-7' id=''> \
                    </div> \
                    <div class='row'> \
                      <input type='checkbox' name='0-8' id=''> \
                    <input type='checkbox' name='1-8' id=''> \
                      <input type='checkbox' name='2-8' id=''> \
                      <input type='checkbox' name='3-8' id=''> \
                      <input type='checkbox' name='4-8' id=''> \
                      <input type='checkbox' name='5-8' id=''> \
                      <input type='checkbox' name='6-8' id=''> \
                      <input type='checkbox' name='7-8' id=''> \
                      <input type='checkbox' name='8-8' id=''> \
                      <input type='checkbox' name='9-8' id=''> \
                      <input type='checkbox' name='10-8' id=''> \
                      <input type='checkbox' name='11-8' id=''> \
                      <input type='checkbox' name='12-8' id=''> \
                      <input type='checkbox' name='13-8' id=''> \
                      <input type='checkbox' name='14-8' id=''> \
                      <input type='checkbox' name='15-8' id=''> \
                    </div> \
                    <div class='row'> \
                    <input type='checkbox' name='0-9' id=''> \
                      <input type='checkbox' name='1-9' id=''> \
                      <input type='checkbox' name='2-9' id=''> \
                      <input type='checkbox' name='3-9' id=''> \
                      <input type='checkbox' name='4-9' id=''> \
                      <input type='checkbox' name='5-9' id=''> \
                      <input type='checkbox' name='6-9' id=''> \
                      <input type='checkbox' name='7-9' id=''> \
                      <input type='checkbox' name='8-9' id=''> \
                      <input type='checkbox' name='9-9' id=''> \
                      <input type='checkbox' name='10-9' id=''> \
                      <input type='checkbox' name='11-9' id=''> \
                      <input type='checkbox' name='12-9' id=''> \
                      <input type='checkbox' name='13-9' id=''> \
                      <input type='checkbox' name='14-9' id=''> \
                      <input type='checkbox' name='15-9' id=''> \
                    </div> \
                    <div class='row'> \
                      <input type='checkbox' name='0-10' id=''> \
                      <input type='checkbox' name='1-10' id=''> \
                      <input type='checkbox' name='2-10' id=''> \
                      <input type='checkbox' name='3-10' id=''> \
                    <input type='checkbox' name='4-10' id=''> \
                      <input type='checkbox' name='5-10' id=''> \
                      <input type='checkbox' name='6-10' id=''> \
                      <input type='checkbox' name='7-10' id=''> \
                      <input type='checkbox' name='8-10' id=''> \
                      <input type='checkbox' name='9-10' id=''> \
                      <input type='checkbox' name='10-10' id=''> \
                      <input type='checkbox' name='11-10' id=''> \
                      <input type='checkbox' name='12-10' id=''> \
                      <input type='checkbox' name='13-10' id=''> \
                      <input type='checkbox' name='14-10' id=''> \
                      <input type='checkbox' name='15-10' id=''> \
                    </div> \
                    <div class='row'> \
                      <input type='checkbox' name='0-11' id=''> \
                      <input type='checkbox' name='1-11' id=''> \
                      <input type='checkbox' name='2-11' id=''> \
                      <input type='checkbox' name='3-11' id=''> \
                      <input type='checkbox' name='4-11' id=''> \
                      <input type='checkbox' name='5-11' id=''> \
                      <input type='checkbox' name='6-11' id=''> \
                      <input type='checkbox' name='7-11' id=''> \
                      <input type='checkbox' name='8-11' id=''> \
                    <input type='checkbox' name='9-11' id=''> \
                      <input type='checkbox' name='10-11' id=''> \
                      <input type='checkbox' name='11-11' id=''> \
                      <input type='checkbox' name='12-11' id=''> \
                      <input type='checkbox' name='13-11' id=''> \
                      <input type='checkbox' name='14-11' id=''> \
                      <input type='checkbox' name='15-11' id=''> \
                    </div> \
                    <div class='row'> \
                      <input type='checkbox' name='0-12' id=''> \
                      <input type='checkbox' name='1-12' id=''> \
                      <input type='checkbox' name='2-12' id=''> \
                      <input type='checkbox' name='3-12' id=''> \
                      <input type='checkbox' name='4-12' id=''> \
                      <input type='checkbox' name='5-12' id=''> \
                      <input type='checkbox' name='6-12' id=''> \
                    <input type='checkbox' name='7-12' id=''> \
                      <input type='checkbox' name='8-12' id=''> \
                      <input type='checkbox' name='9-12' id=''> \
                      <input type='checkbox' name='10-12' id=''> \
                      <input type='checkbox' name='11-12' id=''> \
                      <input type='checkbox' name='12-12' id=''> \
                      <input type='checkbox' name='13-12' id=''> \
                      <input type='checkbox' name='14-12' id=''> \
                      <input type='checkbox' name='15-12' id=''> \
                    </div> \
                    <div class='row'> \
                      <input type='checkbox' name='0-13' id=''> \
                      <input type='checkbox' name='1-13' id=''> \
                      <input type='checkbox' name='2-13' id=''> \
                      <input type='checkbox' name='3-13' id=''> \
                      <input type='checkbox' name='4-13' id=''> \
                    <input type='checkbox' name='5-13' id=''> \
                      <input type='checkbox' name='6-13' id=''> \
                      <input type='checkbox' name='7-13' id=''> \
                      <input type='checkbox' name='8-13' id=''> \
                      <input type='checkbox' name='9-13' id=''> \
                      <input type='checkbox' name='10-13' id=''> \
                      <input type='checkbox' name='11-13' id=''> \
                      <input type='checkbox' name='12-13' id=''> \
                      <input type='checkbox' name='13-13' id=''> \
                      <input type='checkbox' name='14-13' id=''> \
                      <input type='checkbox' name='15-13' id=''> \
                    </div> \
                    <div class='row'> \
                      <input type='checkbox' name='0-14' id=''> \
                      <input type='checkbox' name='1-14' id=''> \
                      <input type='checkbox' name='2-14' id=''> \
                    <input type='checkbox' name='3-14' id=''> \
                      <input type='checkbox' name='4-14' id=''> \
                      <input type='checkbox' name='5-14' id=''> \
                      <input type='checkbox' name='6-14' id=''> \
                      <input type='checkbox' name='7-14' id=''> \
                      <input type='checkbox' name='8-14' id=''> \
                      <input type='checkbox' name='9-14' id=''> \
                      <input type='checkbox' name='10-14' id=''> \
                      <input type='checkbox' name='11-14' id=''> \
                      <input type='checkbox' name='12-14' id=''> \
                      <input type='checkbox' name='13-14' id=''> \
                      <input type='checkbox' name='14-14' id=''> \
                      <input type='checkbox' name='15-14' id=''> \
                    </div> \
                    <div class='row'> \
                      <input type='checkbox' name='0-15' id=''> \
                      <input type='checkbox' name='1-15' id=''> \
                      <input type='checkbox' name='2-15' id=''> \
                    <input type='checkbox' name='3-15' id=''> \
                      <input type='checkbox' name='4-15' id=''> \
                      <input type='checkbox' name='5-15' id=''> \
                      <input type='checkbox' name='6-15' id=''> \
                      <input type='checkbox' name='7-15' id=''> \
                      <input type='checkbox' name='8-15' id=''> \
                      <input type='checkbox' name='9-15' id=''> \
                      <input type='checkbox' name='10-15' id=''> \
                      <input type='checkbox' name='11-15' id=''> \
                      <input type='checkbox' name='12-15' id=''> \
                      <input type='checkbox' name='13-15' id=''> \
                      <input type='checkbox' name='14-15' id=''> \
                      <input type='checkbox' name='15-15' id=''> \
                    </div> \
                  </div> \
                  <br><br> \
                  <label for='chooseColor'>Choose a color:</label> \
                  <select name='textColor' id='chooseColor'> \
                    <option value='0'>Red</option> \
                    <option value='1'>Green</option> \
                    <option value='2'>Blue</option> \
                    <option value='3'>White</option> \
                  </select> \
                  <input type='submit' value='Draw'> \
              </form> \
          </body> \
          </html>";
}

/**
 * @brief Clear screen, set default position and size of cursor.
 * 
 */
void clearScreen() {
  tft.setCursor(0, 0);
  tft.fillScreen(ST7735_BLACK);
  tft.setTextColor(ST7735_WHITE);
  tft.setTextSize(1);
}

/**
 * @brief Print formated text
 * 
 * @param label 
 * @param text 
 */
void printText(const char* label, const char *text) {
  clearScreen();
  printf("%s\n%s\n", label, text);
  tft.println(label);
  tft.setTextColor(ST7735_GREEN);
  tft.setTextSize(2);
  tft.println(text);
}


/**
 * @brief Connect to wifi
 * 
 */
void connectToWifi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    tft.print('.');
    delay(500);
  }
}

/**
 * @brief Load index page
 * 
 */
void indexPageAction() {
  server.send(200, "text/html", htmlCode());
}

/**
 * @brief Display text and redirect to index page
 * 
 */
void displayTextAction() {
  clearScreen();

  tft.setTextSize(2);
  tft.println(server.arg("text"));

  server.sendHeader("Location", "/", true);  
  server.send(302, "text/plain", "");
}

/**
 * @brief Draw pixel 8x8
 * 
 * @param x 
 * @param y 
 * @param color 
 */
void drawPixel(int x, int y, int color) {
  x *= 8;
  y *= 8;
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      tft.drawPixel(x + i, y + j, color);
    }
  }
}

/**
 * @brief Draw image and redirect to index page
 * 
 */
void drawAction() {
  clearScreen();
  
  char name[10];
  int color = ST7735_WHITE;

  if (server.hasArg("textColor")) {
    switch (server.arg("textColor").toInt())
    {
    case 0:
      color = ST7735_RED;
      break;
    case 1:
      color = ST7735_GREEN;
      break;
    case 2:
      color = ST7735_BLUE;
      break;
    case 3:
      color = ST7735_WHITE;
      break;
    default:
      break;
    }
  }

  for (int y = 0; y < 16; y++) {
    for (int x = 0; x < 16; x++) {
      sprintf(name, "%d-%d", y, x);      
      
      if (server.hasArg(name)) {
          drawPixel(x, y, color);
      }
    }
  }

  server.sendHeader("Location", "/", true);  
  server.send(302, "text/plain", "");
}

/**
 * @brief Set up routings
 * 
 */
void setUpRoutings() {
  server.on("/", indexPageAction);
  server.on("/text", HTTP_POST, displayTextAction);
  server.on("/draw", HTTP_POST, drawAction);
}

void setup(void) {
  Serial.begin(115200);
  tft.initR(INITR_144GREENTAB); // Init ST7735R chip, green tab
  
  printText("Connecting to", ssid);
  connectToWifi();

  char buf[16];
  WiFi.localIP().toString().toCharArray(buf, sizeof(buf));
  printText("Connected!", (const char *)buf);
  
  setUpRoutings();
  server.begin();
}

void loop() {
  server.handleClient();
}