#ifndef TETRIS_HPP
#define TETRIS_HPP

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <vector>

#include "shape.hpp"
#include "block.hpp"

#define TFT_WIDTH   128
#define TFT_HEIGHT  128

class Tetris
{
public:
    Tetris(Adafruit_ST7735 &tft);
    void startTetris();
    void moveLeft();
    void moveRight();
    int moveDown();

private:
    Adafruit_ST7735 &tft;
    Shape *currentShape;
    std::vector<Block *> fallenBlocks;
    
    void clearFullLines();
    void clearRow(int row);
    void clearScene();
};

#endif // TETRIS_HPP
