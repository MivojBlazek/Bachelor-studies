#include "shape.hpp"

Shape::Shape(Adafruit_ST7735 &tft, std::vector<Block *> &fallenBlocks)
    : tft(tft),
    fallenBlocks(fallenBlocks)
{
    blocks.push_back(new Block(tft));
    blocks.push_back(new Block(tft));
    blocks.push_back(new Block(tft));
    blocks.push_back(new Block(tft));

    arrangeBlocks();
}

void Shape::arrangeBlocks()
{
    int shapeType = random(0, ShapeTypeCount);
    int x = 7 * BLOCK_SIZE;
    int y = 0;
    uint16_t color = ST7735_WHITE;

    switch(shapeType)
    {
        case ShapeType::I:
            color = ST7735_CYAN;
            blocks[0]->setPosition(x,                   y,  color);
            blocks[1]->setPosition(x - BLOCK_SIZE,      y,  color);
            blocks[2]->setPosition(x + BLOCK_SIZE,      y,  color);
            blocks[3]->setPosition(x + 2 * BLOCK_SIZE,  y,  color);
            break;

        case ShapeType::O:
            color = ST7735_YELLOW;
            blocks[0]->setPosition(x,               y,              color);
            blocks[1]->setPosition(x + BLOCK_SIZE,  y,              color);
            blocks[2]->setPosition(x,               y + BLOCK_SIZE, color);
            blocks[3]->setPosition(x + BLOCK_SIZE,  y + BLOCK_SIZE, color);
            break;

        case ShapeType::T:
            color = ST7735_MAGENTA;
            blocks[0]->setPosition(x,               y + BLOCK_SIZE, color);
            blocks[1]->setPosition(x - BLOCK_SIZE,  y + BLOCK_SIZE, color);
            blocks[2]->setPosition(x + BLOCK_SIZE,  y + BLOCK_SIZE, color);
            blocks[3]->setPosition(x,               y,              color);
            break;

        case ShapeType::S:
            color = ST7735_GREEN;
            blocks[0]->setPosition(x + BLOCK_SIZE,  y,              color);
            blocks[1]->setPosition(x,               y,              color);
            blocks[2]->setPosition(x,               y + BLOCK_SIZE, color);
            blocks[3]->setPosition(x - BLOCK_SIZE,  y + BLOCK_SIZE, color);
            break;

        case ShapeType::Z:
            color = ST7735_RED;
            blocks[0]->setPosition(x - BLOCK_SIZE,  y,              color);
            blocks[1]->setPosition(x,               y,              color);
            blocks[2]->setPosition(x,               y + BLOCK_SIZE, color);
            blocks[3]->setPosition(x + BLOCK_SIZE,  y + BLOCK_SIZE, color);
            break;

        case ShapeType::J:
            color = ST7735_BLUE;
            blocks[0]->setPosition(x,               y + BLOCK_SIZE, color);
            blocks[1]->setPosition(x + BLOCK_SIZE,  y + BLOCK_SIZE, color);
            blocks[2]->setPosition(x - BLOCK_SIZE,  y + BLOCK_SIZE, color);
            blocks[3]->setPosition(x - BLOCK_SIZE,  y,              color);
            break;

        case ShapeType::L:
            color = ST7735_ORANGE;
            blocks[0]->setPosition(x,               y + BLOCK_SIZE, color);
            blocks[1]->setPosition(x - BLOCK_SIZE,  y + BLOCK_SIZE, color);
            blocks[2]->setPosition(x + BLOCK_SIZE,  y + BLOCK_SIZE, color);
            blocks[3]->setPosition(x + BLOCK_SIZE,  y,              color);
            break;

        default:
            break;
    }
}

void Shape::moveLeft()
{
    bool collision = false;
    hide();
    for (Block *block : blocks)
    {
        if (block->moveLeft(fallenBlocks) == 1)
        {
            collision = true;
        }
    }
    if (collision)
    {
        for (Block *block : blocks)
        {
            block->moveRight(fallenBlocks);
        }
    }
    show();
}

void Shape::moveRight()
{
    bool collision = false;
    hide();
    for (Block *block : blocks)
    {
        if (block->moveRight(fallenBlocks) == 1)
        {
            collision = true;
        }
    }
    if (collision)
    {
        for (Block *block : blocks)
        {
            block->moveLeft(fallenBlocks);
        }
    }
    show();
}

int Shape::moveDown()
{
    bool collision = false;
    hide();
    for (Block *block : blocks)
    {
        if (block->moveDown(fallenBlocks) == 1)
        {
            collision = true;
        }
    }
    if (collision)
    {
        // Shape touched of the bottom or other shape
        for (Block *block : blocks)
        {
            block->moveUp();
        }
        show();
        return 1;
    }
    show();
    return 0;
}

std::vector<Block *> Shape::getNewBlocks()
{
    return blocks;
}

void Shape::show()
{
    for (Block *block : blocks)
    {
        block->show();
    }
}

void Shape::hide()
{
    for (Block *block : blocks)
    {
        block->hide();
    }
}
