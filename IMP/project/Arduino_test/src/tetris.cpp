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
    int rows[16]{0};
    for (Block *block : fallenBlocks)
    {
        int y = block->getPositionY() / BLOCK_SIZE;
        rows[y]++;
        if (rows[y] == 16)
        {
            clearRow(y);
        }
    }
}

void Tetris::clearRow(int row)
{
    std::vector<Block *> blocksToRemove;
    for (Block *block : fallenBlocks)
    {
        int y = block->getPositionY() / BLOCK_SIZE;
        if (y == row)
        {
            blocksToRemove.push_back(block);
        }
    }

    for (Block *block : blocksToRemove)
    {
        block->hide();
        fallenBlocks.erase(std::remove(fallenBlocks.begin(), fallenBlocks.end(), block), fallenBlocks.end());
        delete block;
    }

    for (Block *block : fallenBlocks)
    {
        int y = block->getPositionY() / BLOCK_SIZE;
        if (y < row)
        {
            block->hide();
            block->setPosition(block->getPositionX(), block->getPositionY() + BLOCK_SIZE);
            block->show();
        }
    }
}

void Tetris::clearScene()
{
    tft.fillScreen(ST7735_BLACK);
    fallenBlocks.clear();
}
