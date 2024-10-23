#include "tetris.hpp"

Tetris::Tetris(Adafruit_ST7735 &tft) : tft(tft)
{
    //
}

void Tetris::startTetris()
{
    clearScene();
    currentShape = new Shape(tft, fallenBlocks);
}

void Tetris::moveLeft()
{
    currentShape->moveLeft();
}

void Tetris::moveRight()
{
    currentShape->moveRight();
}

int Tetris::moveDown()
{
    if (currentShape->moveDown() == 1)
    {
        // Shape dropped
        for (Block *block : currentShape->getNewBlocks())
        {
            fallenBlocks.push_back(block);
            if (block->getPositionY() <= 0)
            {
                return 1;
            }
        }
        currentShape = new Shape(tft, fallenBlocks);
    }
    return 0;
}

void Tetris::clearScene()
{
    tft.fillScreen(ST7735_BLACK);
    fallenBlocks.clear();
}
