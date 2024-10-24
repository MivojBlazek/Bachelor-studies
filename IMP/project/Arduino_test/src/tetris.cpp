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
        clearFullLines();
        currentShape = new Shape(tft, fallenBlocks);
    }
    return 0;
}

void Tetris::clearFullLines()
{
    // Save block positions
    bool blockPositions[16][16] = {false};
    for (Block *block : fallenBlocks)
    {
        int x = block->getPositionX() / BLOCK_SIZE;
        int y = block->getPositionY() / BLOCK_SIZE;
        blockPositions[x][y] = true;
    }

    // Check full rows
    for (int y = 0; y < 16; y++)
    {
        bool isFull = true;

        for (int x = 0; x < 16; x++)
        {
            if (!blockPositions[x][y])
            {
                isFull = false;
                break;
            }
        }

        if (isFull)
        {
            clearRow(y);
        }
    }
}

void Tetris::clearRow(int row)
{
    //TODO
}

void Tetris::clearScene()
{
    tft.fillScreen(ST7735_BLACK);
    fallenBlocks.clear();
}
