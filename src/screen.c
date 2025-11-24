
void changeScreen(int index)
{
    // zx_border(INK_BLACK);
    levelScreenIndex = index;
    screenNum = levelScreens[index];

    processObjectMap(screenNum);
    drawBackground();
    resetEnemies();

    switch (screenNum)
    {

        case SCREEN_NUM_LAUNDRETTE:

            // display street name
            printString(0, 6, 71, levels[currentLevelNum].name);

            // check whether smudge needs to clean himself up
            if (chimneySwept[levelScreens[levelScreensCompleted]] == 0x11)
            {
                smudgeUsesLaundrette();
            }
            break;

        // case SCREEN_NUM_BRICK:

        //     // windows
        //     colourRectangle(PAPER_BLACK | INK_YELLOW | BRIGHT, 11, 27, 2, 2);
        //     colourRectangle(PAPER_BLACK | INK_YELLOW | BRIGHT, 14, 3, 2, 3);
        //     break;

        case SCREEN_NUM_HIGH_STREET:

            printString(12, 2, PAPER_GREEN | INK_WHITE | BRIGHT, "FLORIST");
            printString(12, 23, PAPER_BLUE | INK_WHITE | BRIGHT, "GROCERY");
            break;

        case SCREEN_NUM_RURAL_2:

            printString(13, 19, PAPER_YELLOW | INK_BLACK, "BAN");
            printString(14, 17, PAPER_YELLOW | INK_BLACK, "REMAKES");
            break;

        case SCREEN_NUM_CORNER_SHOP:
            printString(12, 25, PAPER_GREEN | INK_WHITE | BRIGHT, "NEWS");
            break;

        case SCREEN_NUM_BUCK_HOUSE:

            drawInterface();
            printString(0, 11, 71, "BUCK HOUSE");

            // level is complete
            levelComplete();
            break;
    }

    drawInterface();
}

void processObjectMap(uint8_t mapNum)
{
    // get array length - only one byte needed for this
    unsigned char *objectMap = ScreenMaps[mapNum];
    uint8_t length = (unsigned char)objectMap[0];
    uint8_t index;

    // clear the screen
    clearScreen();

    // draw stars
    currentScreen[(2 * SCREEN_COLS + 4)] = TILE_STAR_YELLOW;
    currentScreen[(4 * SCREEN_COLS + 20)] = TILE_STAR_CYAN;
    currentScreen[(2 * SCREEN_COLS + 30)] = TILE_STAR_WHITE;
    currentScreen[(10 * SCREEN_COLS + 13)] = TILE_STAR_WHITE;

    // draw the road
    if( screenNum == SCREEN_NUM_CASTLE ) {
        addCompressedTilemap(OBJECT_NUM_ROAD3, ROAD_ROW, 0);
    } else if(
        screenNum == SCREEN_NUM_RURAL || 
        screenNum == SCREEN_NUM_RURAL_2 || 
        screenNum == SCREEN_NUM_CASTLE_CORNER ) {
        addCompressedTilemap(OBJECT_NUM_ROAD2, ROAD_ROW, 0);
    } else {
        addCompressedTilemap(OBJECT_NUM_ROAD1, ROAD_ROW, 0);
    }

    // draw screen objects
    for (i = 1; i < length + 1; i += 3)
    {

        // get dimensions
        index = (unsigned char)objectMap[i];
        row = (unsigned char)objectMap[i + 1];
        col = (unsigned char)objectMap[i + 2];

        // draw object
        addTilemap(index, row, col);
    }

    // draw barrier
    drawBarrier();

    // draw cones
    drawCones();

    sp1_Invalidate(&full_screen);
}

// place barrier if necessary
void drawBarrier(void)
{
    if (screenNum != SCREEN_NUM_BUCK_HOUSE && screenNum != SCREEN_NUM_LAUNDRETTE &&
        levelScreenIndex > levelScreensCompleted)
    {

        if (screenNum == SCREEN_NUM_CASTLE)
        {
            n = (BARRIER_CASTLE_Y * SCREEN_COLS) + BARRIER_CASTLE_X;
        }
        else
        {
            n = (BARRIER_Y * SCREEN_COLS) + BARRIER_X;
        }

        currentScreen[n] = TILE_BARRIER;
        currentScreen[n + 1] = TILE_BARRIER + 1;
        currentScreen[n + 2] = TILE_BARRIER + 1;
        currentScreen[n + 3] = TILE_BARRIER + 2;
    }
}

void drawCones(void)
{
    // place cones if necessary
    if (screenNum != SCREEN_NUM_BUCK_HOUSE &&
        levelScreens[levelScreenIndex + 1] == SCREEN_NUM_BUCK_HOUSE &&
        levelScreenIndex > levelScreensCompleted - 1)
    {
        currentScreen[18 * SCREEN_COLS] = TILE_CONE;
        currentScreen[20 * SCREEN_COLS] = TILE_CONE;
        currentScreen[22 * SCREEN_COLS] = TILE_CONE;
    }
}

void drawBackground(void)
{
    uint16_t tileNum;

    count = 0;
    for (y = 0; y < 24; y++)
    {

        for (x = 0; x < 32; x++)
        {
            tileNum = currentScreen[count];

            if (tileNum == TILE_CHIMNEY_DIRTY)
            {
                if ((x <= 16 && (chimneySwept[screenNum] & 0x10) == 0x10) ||
                    (x > 16 && (chimneySwept[screenNum] & 0x1) == 0x1))
                {
                    tileNum++;
                    currentScreen[count] = tileNum;
                }
            }

            printTile(y, x, tileNum);
            count++;
        }
    }
    sp1_Invalidate(&full_screen);
}
