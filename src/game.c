#include "game.h"

#include "utilities.c"
#include "screen.c"
#include "tile.c"
#include "player.c"
#include "controls.c"
#include "main-menu.c"
#include "enemy.c"
#include "level.c"
#include "game-over.c"

int main(void)
{
    // initialise sp1_engine
    sp1_Initialize(
        SP1_IFLAG_MAKE_ROTTBL |
            SP1_IFLAG_OVERWRITE_TILES |
            SP1_IFLAG_OVERWRITE_DFILE,
        INK_WHITE |
            PAPER_BLACK |
            BRIGHT,
        ' ');

    // create title sprite
    // spTitle = getNewUnmaskedSprite((int *) titleSprite, 8, 4);

    joy = (JOYFUNC)in_stick_keyboard;

    // create brush sprite
    brushSp = getNewUnmaskedSprite((int *)brushSprite, 2, 1);

    initialisePlayer();
    initialiseEnemies();
    initialiseLevels();

    // font
    loadFont();

    zx_border(INK_BLACK);

    // clear
    sp1_Invalidate(&full_screen);

    while (1)
    {

        mainMenu();
        startNewGame();

        // main loop
        framecount = 0;
        while (1)
        {

            // reset border
            zx_border(INK_BLACK);

            // slow down the game if necessary
            // if( screenNum == SCREEN_NUM_LAUNDRETTE || screenNum == SCREEN_NUM_BUCK_HOUSE ) {
            //     z80_delay_ms(levelNumEnemies);
            // }
            update();

            if (mode == MODE_GAME_OVER)
            {
                gameOver();
                break;
            }

            if (mode == MODE_MAIN_MENU)
            {
                moveEverythingOffScreen();
                break;
            }
        }
    }
}

void startNewGame(void)
{
    mode = MODE_PLAYING;

    // reset score
    score = 0;
    resetPlayer();

    // go to first level
    startNewLevel(START_LEVEL);
}

void drawInterface(void)
{
    for (i = 0; i < 32; i++)
    {
        n = statusBarTilemap[i];
        printTile(SCREEN_ROWS - 1, i, n);
    }

    drawScore();
    drawTimeLeft();
    drawNumLives();
    updatePowerBar();
}

void drawScore(void)
{
    printNum(SCREEN_ROWS - 1, 18, INK_WHITE | PAPER_BLACK | BRIGHT, score, 4);
}

void drawTimeLeft(void)
{
    printNum(SCREEN_ROWS - 1, 25, INK_WHITE | PAPER_BLACK | BRIGHT, timeLeft, 3);
}

void drawNumLives(void)
{
    sprintf(strNumber, "%d", numLives);
    printString(SCREEN_ROWS - 1, 31, 71, strNumber);
}

void updatePowerBar(void)
{
    unsigned char colour;

    // playerPower = rand() % PLAYER_POWER_MAX;
    count = playerPower / 10;

    if (count <= 4)
    {
        colour = PAPER_RED | BRIGHT;
    }
    else if (count <= 8)
    {
        colour = PAPER_RED;
    }
    else if (count <= 10)
    {
        colour = PAPER_YELLOW | BRIGHT;
    }
    else
    {
        colour = PAPER_GREEN | BRIGHT;
    }

    for (i = 1; i < POWERBAR_LENGTH + 1; i++)
    {
        if (i <= count)
        {
            sp1_PrintAtInv(SCREEN_ROWS - 1, i, colour, (uint16_t)&tilesGraphics[0]);
        }
        else
        {
            printTile(SCREEN_ROWS - 1, i, 0);
        }
    }
}

uint8_t checkChimney(uint8_t testrow, uint8_t testcol)
{
    uint8_t tileNum = getTileNumForRowCol(testrow, testcol);

    // printNum(0, 0, INK_BLACK|PAPER_WHITE|BRIGHT, testrow, 3);
    // printNum(0, 4, INK_BLUE|PAPER_WHITE|BRIGHT, testcol, 3);
    // printNum(0, 8, INK_GREEN|PAPER_WHITE|BRIGHT, tileNum, 3);

    if (tileNum == TILE_CHIMNEY_DIRTY)
    {
        sweepChimney(testrow, testcol);
        return true;
    }
    else if (tileNum == TILE_CHIMNEY_CLEAN)
    {
        return true;
    }
    return false;
}

uint8_t sweepChimney(uint8_t testrow, uint8_t testcol)
{
    uint8_t bitmask;

    if (testcol <= 16)
    {
        bitmask = 0x10;
    }
    else
    {
        bitmask = 0x1;
    }

    // now sweep!
    chimneySwept[screenNum] |= bitmask;

    // do the animation
    uint8_t brushx = (testcol * 8) - 4;
    uint8_t brushy = y + 16;

    x = player.x;
    y = player.y;

    // zx_border(INK_GREEN);

    // hide player
    moveSpriteOffScreen(player.sp);

    // show animation
    sp1_MoveSprPix(brushSp, &full_screen, 0, brushx, brushy);

    framecount = 0;
    for (i = 0; i < 60; i++)
    {

        changeFrameNext(brushSp, BRUSH_FRAME_LENGTH, 0, 2);
        sp1_MoveSprPix(brushSp, &full_screen, 0, brushx, brushy);
        sp1_UpdateNow();

        // go to next frame
        framecount++;
        z80_delay_ms(10);
    }

    // highlight clean chimney
    currentScreen[(testrow * SCREEN_COLS) + testcol] = TILE_CHIMNEY_CLEAN;
    printTile(testrow, testcol, TILE_CHIMNEY_CLEAN);
    bit_beepfx_di(BEEPFX_SELECT_1);

    score += SCORE_CHIMNEY_SWEPT;

    // restore player
    player.x = x;
    player.y = y;

    // get rid of the brush
    moveSpriteOffScreen(brushSp);

    return true;
}

void smudgeUsesLaundrette()
{
    // get ready for next screen
    levelScreensCompleted++;

    resetPlayerPosition();

    // need to animate this
    playerPower = PLAYER_POWER_MAX;

    // add to score
    score += SCORE_SCREEN_COMPLETED;

    bit_beepfx_di(BEEPFX_ITEM_5);
}

void update(void)
{
    readControls();

    if (mode != MODE_PLAYING)
    {
        return;
    }

    // enemies
    if (framecount % 3 == 0)
    {
        updateEnemies();
    }
    // player
    updatePlayer();
    sp1_UpdateNow();

    // handle frame count
    framecount++;
    if (framecount > 50)
    {
        framecount = 0;

        // count down
        if (timeLeft > 1)
        {
            timeLeft--;
        }
        else
        {
            // lose a life
            loseALife();
        }

        drawTimeLeft();
    }
}

void moveEverythingOffScreen(void)
{
    for (i = 0; i < levelNumEnemies; i++)
    {
        moveSpriteOffScreen(enemies[i].sp);
    }
    moveSpriteOffScreen(player.sp);
}

void loseALife(void)
{

    zx_border(INK_YELLOW);

    numLives--;

    bit_beepfx_di(BEEPFX_BOOM_6);

    if (numLives < 0)
    {
        mode = MODE_GAME_OVER;
        return;
    }

    playerPower = PLAYER_POWER_MAX;
    resetPlayerPosition();
    changeScreen(0);
}

void loadFont(void)
{
    // Initialise Tiles
    for (i = 0; i < 64; i++)
    {
        sp1_TileEntry(32 + i, (int)&fontGraphics[i * 8]);
    }
}
