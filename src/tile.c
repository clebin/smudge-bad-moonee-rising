
uint8_t getTileNumForRowCol(uint8_t row, uint8_t col)
{
    return currentScreen[(row * SCREEN_COLS) + col];
}

uint8_t getColForX(uint16_t x)
{
    return (uint8_t) x / 8;
}

uint8_t getRowForY(uint16_t y)
{
    return (uint8_t) y / 8;
}

// properties
// unsigned char getTileProperties(uint8_t col, uint8_t row)
// {
//     return currentScreen[(row * PLAYAREA_COLS) + col];
// }

uint8_t isTileSolid(int testcol, int testrow)
{
    if( testcol < 0 || testrow < 0 || testcol >= PLAYAREA_COLS || testrow >= PLAYAREA_ROWS ) {
        return false;
    }

    uint16_t tileNum = currentScreen[(testrow * PLAYAREA_COLS) + testcol];
    return (uint8_t) ((tilesColours[tileNum] & 0x80) >> 7);
}

// uint8_t isTileLethal(uint8_t testcol, uint8_t testrow)
// {
//     uint16_t tileNum = currentScreen[(testrow * PLAYAREA_COLS) + testcol];
//     return (tilesetProperties[tileNum] | 0x40) >> 6;
// }

// uint8_t isTilePlatform(uint8_t testcol, uint8_t testrow)
// {
//     uint16_t tileNum = currentScreen[(testrow * PLAYAREA_COLS) + testcol];
//     return (tilesetProperties[tileNum] | 0x20) >> 5;
// }

// uint8_t isTileCustom1(uint8_t testcol, uint8_t testrow)
// {
//     uint16_t tileNum = currentScreen[(testrow * PLAYAREA_COLS) + testcol];
//     return (tilesetProperties[tileNum] | 0x10) >> 4;
// }
