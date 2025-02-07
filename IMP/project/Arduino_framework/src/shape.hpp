#ifndef SHAPE_HPP
#define SHAPE_HPP

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <vector>

#include "block.hpp"

class Shape
{
public:
    Shape(Adafruit_ST7735 &tft, std::vector<Block *> &fallenBlocks);
    enum ShapeType { I, O, T, S, Z, J, L, ShapeTypeCount};

    void arrangeBlocks();
    void moveLeft();
    void moveRight();
    int moveDown();
    std::vector<Block *> getNewBlocks();

private:
    Adafruit_ST7735 &tft;
    std::vector<Block *> blocks;
    std::vector<Block *> &fallenBlocks;

    void show();
    void hide();
};

#endif // SHAPE_HPP
