
void printString(uint8_t y, uint8_t x, uint8_t attr, char *chars)
{

    uint8_t xx, i;
    xx = x;
    i = 0;
    while (chars[i])
    {
        sp1_PrintAtInv(y, xx++, attr, chars[i++]);
    }
}

void printNum(uint8_t row, uint8_t col, uint8_t colour, uint16_t num, uint8_t length)
{
    uint8_t chindex;

    for (chindex = 0; chindex < length; chindex++)
    {
        sp1_PrintAtInv(row, col + chindex, colour, (uint16_t)&tilesGraphics[0]);
    }

    sprintf(strNumber, "%d", num);
    printString(row, col, colour, strNumber);
}

void printBigString(uint8_t row, uint8_t col, uint8_t colour, char *str, uint8_t tileNum)
{
    uint8_t len = strlen(str);
    uint8_t index;

    for (index = 0; index < len; index++)
    {
        printBigLetter(row, col + (index * 8), colour, str[index], tileNum);
    }
}

void printBigLetter(uint8_t row, uint8_t col, uint8_t colour, char ch, uint8_t tileNum)
{
    ptr = &fontGraphics[(ch - 32) * 8];

    // loop through lines
    for (i = 0; i < 8; i++)
    {
        // loop through bits
        for (n = 0; n < 8; n++)
        {
            t = *ptr;
            if (((t >> n) & 1) == 1)
            {
                sp1_PrintAtInv(row + i, col + 7 - n, colour, (uint16_t)&tilesGraphics[tileNum * 8]);
            }
        }
        ptr++;
    }
}

struct sp1_ss *getNewUnmaskedSprite(uint8_t *sprite, uint8_t spCols, uint8_t spRows)
{
    struct sp1_ss *sp;
    sp = sp1_CreateSpr(SP1_DRAW_LOAD1LB, SP1_TYPE_1BYTE, spRows + 1, (int)sprite, 0);

    for (int i = 1; i < spCols; i++)
    {
        sp1_AddColSpr(sp, SP1_DRAW_LOAD1, 0, (int)&sprite[(i * spRows * 8) + (i * 8)], 0);
    }
    sp1_AddColSpr(sp, SP1_DRAW_LOAD1RB, SP1_TYPE_1BYTE, 0, 0);

    return sp;
}

struct sp1_ss *getNewMaskedSprite(uint8_t *sprite, uint8_t spCols, uint8_t spRows)
{
    struct sp1_ss *sp;

    sp = sp1_CreateSpr(SP1_DRAW_MASK2LB, SP1_TYPE_2BYTE, spRows + 1, (int)sprite, 0);

    for (int i = 1; i < spCols; i++)
    {
        sp1_AddColSpr(sp, SP1_DRAW_MASK2, SP1_TYPE_2BYTE, (int)&sprite[(i * spRows * 16) + (i * 16)], 0);
    }
    sp1_AddColSpr(sp, SP1_DRAW_MASK2RB, SP1_TYPE_2BYTE, 0, 0);

    return sp;
}

uint8_t spriteTestEdge(uint8_t testcol, uint8_t testrow, uint8_t length, uint8_t edge)
{
    for (uint8_t testi = 0; testi < length; testi++)
    {

        if (isTileSolid(testcol, testrow) == true)
        {
            return true;
        }

        if (edge == EDGE_HORIZONTAL)
        {
            testcol++;
        }
        else
        {
            testrow++;
        }
    }

    return false;
}

// TODO: find out the best way of doing this
void clearScreen(void)
{
    for (y = 0; y < 24; y++)
    {
        for (x = 0; x < 32; x++)
        {
            currentScreen[(y * SCREEN_COLS) + x] = 0;
            printTile(y, x, 0);
        }
    }
}

// void colourRectangle(uint8_t colour, uint8_t row, uint8_t col, uint8_t height, uint8_t width)
// {
//     uint8_t tileNum;
//     for (y = row; y < row + height; y++)
//     {
//         for (x = col; x < col + width; x++)
//         {
//             tileNum = currentScreen[(y * SCREEN_COLS) + x];
//             sp1_PrintAtInv(y, x, colour, (uint16_t)&tilesGraphics[tileNum * 8]);
//         }
//     }
// }

void changeSpriteColour(struct sp1_ss *sp, uint8_t colour)
{
    t = colour;
    sp1_IterateSprChar(sp, initialiseColour);
}

void initialiseColour(uint16_t count, struct sp1_cs *c)
{
    (void)count; // suppress warning
    c->attr_mask = SP1_AMASK_INK;
    c->attr = t;
}

// void plot(unsigned char y, unsigned char x)
// {
//     *zx_pxy2saddr(x,y) |= zx_px2bitmask(x);
// }

void printTile(uint8_t row, uint8_t col, uint8_t num)
{
    sp1_PrintAtInv(row, col, tilesColours[num] & 0x7F, (uint16_t)&tilesGraphics[num * 8]);
}

void moveSpriteOffScreen(struct sp1_ss *sp)
{
    sp1_MoveSprAbs(sp, &full_screen, 0, -100, -100, 0, 0);
}

void changeFrame(struct sp1_ss *sp, uint8_t framelength, uint8_t frame)
{
    sp->frame = (void *)(framelength * frame);
}

void changeFrameNext(struct sp1_ss *sp, uint8_t framelength, uint8_t framefirst, uint8_t framelast)
{
    if (framecount % 3 == 0)
    {
        sp->frame += framelength;

        if (sp->frame > framelength * framelast)
        {
            sp->frame = framelength * framefirst;
        }
    }
}

void waitForFireButton(void)
{
    // wait for fire button
    while (1)
    {
        in = (joy)(&joy_keys);
        if (in & IN_STICK_FIRE)
        {
            break;
        }
    }
}

void addTilemap(uint8_t index, uint8_t row, uint8_t col)
{
    unsigned char *obj = GameObjects[index];
    uint8_t height = (uint8_t)obj[0];
    uint8_t width = (uint8_t)obj[1];
    uint8_t tileNum;
    uint16_t source;
    uint16_t dest;

    // skip to third element in array
    source = 2;
    dest = 0;

    for (y = row; y < row + height; y++)
    {
        dest = (y * SCREEN_COLS) + col;

        for (x = col; x<col+width; x++)
        {
            tileNum = obj[source];

            // add the tile
            if (
                x >= 0 && x < SCREEN_COLS &&
                y >= 0 && y < SCREEN_ROWS &&
                tileNum > 0)
            {
                currentScreen[dest] = tileNum;
                // printTile(y+row, x+col, tileNum);
            }
            source++;
            dest++;
        }
    }
}

void addCompressedTilemap(uint8_t index, uint8_t row, uint8_t col)
{
    uint8_t tileNum;

    unsigned char *tilemap = GameObjects[index];

    // get width & height
    unsigned char height = (unsigned char) tilemap[0];
    unsigned char width = (unsigned char) tilemap[1];  

    // get array length
    unsigned char hiByte = (unsigned char) tilemap[2];
    unsigned char loByte = (unsigned char) tilemap[3];

    uint16_t length = (hiByte << 8) | loByte;

    t = (row*SCREEN_COLS)+col;

    for(i=4;i<length+4;i+=2) {
        tileNum = tilemap[i];
        // loop through tiles
        for(n=0;n<tilemap[i+1];n++) {
            currentScreen[t] = tileNum;
            t++;
        }
    }

    // row = 0;
    // get array data
    // x = 0;
    // for(i=4;i<length+4;i+=2) {

    //     tileNum = tilemap[i];
    //     // loop through tiles
    //     for(n=0;n<tilemap[i+1];n++) {
    //         if(x>width) {
    //             x = 0;
    //             row++;
    //         }
    //         // write to current screen
    //         t = (row*SCREEN_COLS)+col+x;
    //         if( t < 768 ) {
    //             currentScreen[(row*SCREEN_COLS)+col+x] = tileNum;
    //         }
    //         x++;
    //     }
    // }
}

// void drawTilemap(uint8_t index, uint8_t row, uint8_t col)
// {
//     unsigned char *obj = GameObjects[index];
//     uint8_t height = (uint8_t) obj[0];
//     uint8_t width = (uint8_t) obj[1];
//     uint8_t tileNum;

//     // skip to third element in array
//     count = 2;
//     for(y=0;y<height;y++) {
//         for(x=0;x<width;x++) {
//             tileNum = obj[count];
//             if( tileNum > 0 ) {
//                 printTile(y+row, x+col, tileNum);
//             }
//             count++;
//         }
//     }
// }

// void showSpriteArea(struct sp1_ss *sp, sp, int8_t x, int8_t y)
// {
//     i = getColForX(x);
//     n = getRowForY(y);

//     for(int a=0;a<sp->width;a++) {
//         for(int b=0;b<sp->height;b++) {
//             sp1_PrintAtInv(
//                 n+b,
//                 i+a,
//                 tilesColours[13],
//                 (uint16_t) &tilesGraphics[5]
//             );
//         }
//     }
//     // pos
//     sp1_PrintAtInv(
//         n,
//         i,
//         tilesColours[12],
//         (uint16_t) &tilesGraphics[5]
//     );
// }

// void line(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1)
// {
//     unsigned char dx  = abs(x1-x0);
//     unsigned char dy  = abs(y1-y0);
//     signed char sx  = x0<x1 ? 1 : -1;
//     signed char sy  = y0<y1 ? 1 : -1;
//     int err = (dx>dy ? dx : -dy)/2;
//     int e2;

//     while (1)
//     {
//         plot(x0,y0);
//         if (x0==x1 && y0==y1) break;

//         e2 = err;
//         if (e2 >-dx) { err -= dy; x0 += sx; }
//         if (e2 < dy) { err += dx; y0 += sy; }
//     }
// }
