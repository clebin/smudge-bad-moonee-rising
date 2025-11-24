void initialiseLevels(void)
{
    strcpy(levels[0].name, "CURRAHNATION STREET");
    strcpy(levels[1].name, "PI-CADILLY CIRCUS");
    strcpy(levels[2].name, "WEIRVERRIE CLOSE");
    strcpy(levels[3].name, "PENULTIMATE AVENUE");
    strcpy(levels[4].name, "MICRO DRIVE");
}

void startNewLevel(uint8_t num)
{
    // reset variables
    currentLevelNum = num;
    timeLeft = MAX_TIME_LEFT;// + (num * 50);
    
    levelNumEnemies = START_ENEMIES + num;
    if( levelNumEnemies > NUM_ENEMIES ) {
        levelNumEnemies = NUM_ENEMIES;
    }

    playerPower = PLAYER_POWER_MAX;
    player.x = PLAYER_START_X;
    player.y = PLAYER_START_Y;
    levelScreens = levelScreenLayouts[num];

    // set sprite to starting position
    resetEnemies();

    // reset chimneys swept
    levelScreensCompleted = 1;
    for(i=1;i<NUM_SCREENS;i++) {
        chimneySwept[i] = 0;
    }
    
    // go to laundrette
    changeScreen(0);
    
    bit_beepfx_di(BEEPFX_ITEM_5);
}

void levelComplete()
{
    moveSpriteOffScreen(player.sp);
    player.deltax = -1;

    sp1_UpdateNow();

    // display message
    if( currentLevelNum == NUM_LEVELS-1 ) {
        printString(8, 9,INK_WHITE|PAPER_BLACK|BRIGHT, "CONGRATULATIONS!" );
        printString(10, 0, INK_CYAN|PAPER_BLACK|BRIGHT, "IT WAS A CLEAN SWEEP. JOIN SMUDGE");
        printString(11, 0, INK_CYAN|PAPER_BLACK|BRIGHT, "AGAIN IN HIS NEXT ADVENTURE" );
        t = SCORE_GAME_COMPLETED;
    } else {
        printString(8, 9, INK_WHITE|PAPER_BLACK|BRIGHT, "LEVEL COMPLETE");
        t = SCORE_LEVEL_COMPLETED;
    }
    bit_beepfx_di(BEEPFX_SCORE);
    z80_delay_ms(300);
    sp1_UpdateNow();

    // add score for level complete
    score += t;
    printString(13, 11, INK_MAGENTA|PAPER_BLACK|BRIGHT, "BONUS: ");
    printNum(13, 18, INK_WHITE|PAPER_BLACK|BRIGHT, t, 5);

    z80_delay_ms(300);

    sp1_UpdateNow();

    // time bonus
    printString(15, 6, INK_GREEN|PAPER_BLACK|BRIGHT, "TIME BONUS: ");
    z80_delay_ms(200);

    count = 0;
    framecount = 0;
    while(timeLeft >= 0 ) {

        count += SCORE_TIME_BONUS_MULTIPLIER;
        printNum(15, 18, INK_WHITE|PAPER_BLACK|BRIGHT, count, 3);
        drawTimeLeft();

        // walk smudge across the screen
        if( count % 2 == 0 && player.x > -20 ) {
            player.x -= 2;
            changeFrameNext(player.sp, PLAYER_FRAME_LENGTH, 1, 4);
            drawPlayer();
        }

        sp1_UpdateNow();
        timeLeft--;
        framecount++;
    }
    score += count;

    drawScore();
    bit_beepfx_di(BEEPFX_SELECT_2);
    
    // add score for remaining time
    sp1_UpdateNow();

    if( currentLevelNum == NUM_LEVELS-1 ) {
        waitForFireButton();
    } else {
        z80_delay_ms(300);
    }
    
    // you've completed the game!
    if(currentLevelNum == NUM_LEVELS-1 ) {
        mode = MODE_MAIN_MENU;
    }
    // go to next level
    else {
        startNewLevel(currentLevelNum+1);
    }
}