#include "block.hpp"

Block::Block(Adafruit_ST7735 &tft)
    : tft(tft)
{
    positionX = 0;
    positionY = 0;
    color = ST7735_WHITE;
}

int Block::moveLeft(std::vector<Block *> &fallenBlocks)
{
    positionX -= BLOCK_SIZE;
    if (positionX < 0 || isCollision(fallenBlocks))
    {
        return 1;
    }
    return 0;
}

int Block::moveRight(std::vector<Block *> &fallenBlocks)
{
    positionX += BLOCK_SIZE;
    if (positionX >= 16 * BLOCK_SIZE || isCollision(fallenBlocks))
    {
        return 1;
    }
    return 0;
}

int Block::moveDown(std::vector<Block *> &fallenBlocks)
{
    positionY += BLOCK_SIZE;
    if (positionY >= 16 * BLOCK_SIZE || isCollision(fallenBlocks))
    {
        return 1;
    }
    return 0;
}

void Block::moveUp()
{
    positionY -= BLOCK_SIZE;
}

void Block::setPosition(int x, int y, uint16_t newColor)
{
    positionX = x;
    positionY = y;
    color = newColor;
}

void Block::show()
{
    tft.drawRect(positionX, positionY, BLOCK_SIZE, BLOCK_SIZE, color);
}

void Block::hide()
{
    tft.drawRect(positionX, positionY, BLOCK_SIZE, BLOCK_SIZE, ST7735_BLACK);
}

int Block::getPositionX()
{
    return positionX;
}

int Block::getPositionY()
{
    return positionY;
}

int Block::isCollision(std::vector<Block *> &fallenBlocks)
{
    for (Block *fallenBlock : fallenBlocks)
    {
        if (fallenBlock->getPositionX() == positionX &&
            fallenBlock->getPositionY() == positionY)
        {
            return 1;
        }
    }
    return 0;
}
