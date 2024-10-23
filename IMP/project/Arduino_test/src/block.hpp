#ifndef BLOCK_HPP
#define BLOCK_HPP

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <vector>

#define BLOCK_SIZE 8

class Block
{
public:
    Block(Adafruit_ST7735 &tft);
    int moveLeft(std::vector<Block *> &fallenBlocks);
    int moveRight(std::vector<Block *> &fallenBlocks);
    int moveDown(std::vector<Block *> &fallenBlocks);
    void moveUp();
    void setPosition(int x, int y, uint16_t newColor);
    void show();
    void hide();
    int getPositionX();
    int getPositionY();

private:
    Adafruit_ST7735 &tft;
    int positionX;
    int positionY;
    uint16_t color;

    int isCollision(std::vector<Block *> &fallenBlocks);
};

#endif // BLOCK_HPP
