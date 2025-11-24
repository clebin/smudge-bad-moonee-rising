void gameOver(void)
{
    zx_border(0);
    
    // clear tiles
    clearScreen();
    
    moveEverythingOffScreen();
    
    // update high score
    if( score > highScore ) {
        highScore = score;
    }

    // draw game over
    printBigString(0, 0, PAPER_MAGENTA|INK_MAGENTA|BRIGHT, "GAME", 13);
    printBigString(8, 0, PAPER_MAGENTA|INK_MAGENTA|BRIGHT, "OVER", 13);
    
    printString(18, 0, PAPER_BLACK|INK_WHITE|BRIGHT, "SCORE:");
    printNum(18, 8, PAPER_BLACK|INK_WHITE|BRIGHT, score, 5);
    
    sp1_UpdateNow();

    waitForFireButton();
}