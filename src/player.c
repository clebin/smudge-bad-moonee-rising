
void initialisePlayer(void)
{
    player.sp = getNewMaskedSprite((int *) playerSprite, 2, 3);
    // player.spBike = getNewMaskedSprite((int *) playerBikeSprite, 4, 3);
}

void resetPlayer(void)
{
    numLives = START_NUM_LIVES;
    player.sp->frame = 0;
    playerPower = PLAYER_POWER_MAX;

    resetPlayerPosition();
}

void resetPlayerPosition(void)
{
    player.y = PLAYER_START_Y;
    player.x = PLAYER_START_X;
    player.deltax = -1;
    player.facing = -1;
    player.deltay = 0;
    player.state = PLAYER_STATE_WALKING;
    changeFrame(player.sp, PLAYER_FRAME_LENGTH, PLAYER_FRAME_STANDING_L);

    // set to starting position
    sp1_MoveSprAbs(player.sp, &full_screen, (void*) player.sp->frame, player.y, player.x, 0, 0);
}

void updatePlayer(void)
{
    player.col = getColForX(player.x);
    player.row = getRowForY(player.y);

    // left/right
    if( player.state == PLAYER_STATE_WALKING ) {
        updatePlayerWalking();
    } else {
        updatePlayerJetpack();
    }

    // up/down
    if( player.deltay == -1 ) {
        playerMoveUp();
    } else if( player.deltay == 1 ) {
        playerMoveDown();
    } else {
        playerNoMoveY();
    }

    // check collisions
    for(t=0;t<levelNumEnemies;t++) {
        if( enemies[t].spawnTime == -1 ) {
            checkCollision(t);
        }
    }

    updatePowerBar();

    if( playerPower <= 0 ) {
        loseALife();
    }
}

void crashedJetpack(void)
{
    playerPower -= PENALTY_CRASH_JETPACK;
    zx_border(INK_BLUE);
    bit_beepfx_di(BEEPFX_HIT_3);   
}

void updatePlayerWalking(void)
{
    // moving left
    if( player.deltax == -1 ) {

        // already facing left - next frame
        if( player.facing == -1 ) {
            changeFrameNext(player.sp, PLAYER_FRAME_LENGTH, 1, 4);
        }
        // first facing frame
        else {
            changeFrame(player.sp, PLAYER_FRAME_LENGTH, PLAYER_FRAME_STANDING_L);
        }
        playerMoveHorizontal(-1);
    }
    // moving right
    else if( player.deltax == 1) {

        // already facing right - next frame
        if( player.facing == 1 ) {
            changeFrameNext(player.sp, PLAYER_FRAME_LENGTH, 6, 9);
        }
        // first facing frame
        else {
            changeFrame(player.sp, PLAYER_FRAME_LENGTH, PLAYER_FRAME_STANDING_R);
        }
        playerMoveHorizontal(1);
    }
    // not moving - facing left
    else if( player.facing == -1 ) {
        changeFrame(player.sp, PLAYER_FRAME_LENGTH, PLAYER_FRAME_STANDING_L);
        
    }
    // not moving - facing right
    else {
        changeFrame(player.sp, PLAYER_FRAME_LENGTH, PLAYER_FRAME_STANDING_R);
    }

    // draw
    drawPlayer();
}

void drawPlayer(void)
{
    sp1_MoveSprPix(player.sp, &full_screen, (void*) 0, player.x, player.y);
}

void updatePlayerJetpack(void)
{
    if( player.deltax == -1 ) {
        changeFrame(player.sp, PLAYER_FRAME_LENGTH, PLAYER_FRAME_JETPACK_L);
        playerMoveHorizontal(-1);
    } else if( player.deltax == 1) {
        changeFrame(player.sp, PLAYER_FRAME_LENGTH, PLAYER_FRAME_JETPACK_R);
        playerMoveHorizontal(1);   
    }
    
    // draw
    drawPlayer();
}
void playerChangeStateJetpack(void)
{
    player.state = PLAYER_STATE_JETPACK;
    player.y = PLAYER_STATE_SWITCH_Y-24;
    
    if( player.facing < 0 ) {
        changeFrame(player.sp, PLAYER_FRAME_LENGTH, PLAYER_FRAME_JETPACK_L);
    } else {
        changeFrame(player.sp, PLAYER_FRAME_LENGTH, PLAYER_FRAME_JETPACK_R);
    }
}

void playerChangeStateWalking(void)
{    
    player.state = PLAYER_STATE_WALKING;
    player.y = PLAYER_STATE_SWITCH_Y;

    if( player.facing < 0 ) {
        changeFrame(player.sp, PLAYER_FRAME_LENGTH, PLAYER_FRAME_STANDING_L);
    } else {
        changeFrame(player.sp, PLAYER_FRAME_LENGTH, PLAYER_FRAME_STANDING_R);
    }

    sp1_MoveSprAbs(player.sp, &full_screen, 0, player.y, SCREEN_WIDTH+1, 0, 0);
}

void playerMoveUp(void)
{
    y = player.y - PLAYER_SPEEDY;
    row = getRowForY(y);

    if( player.state == PLAYER_STATE_JETPACK ) {

        bit_click();

        if( framecount % PENALTY_JETPACK_POWER_USAGE == 0 ) {
            playerPower--;
        }
        
        if( y <= 0 ) {
            player.y = 0;
            return;
        }

    } else if( player.y < PLAYER_STATE_SWITCH_Y ) {
        playerChangeStateJetpack();
    }

    // hit an edge
    if( spriteTestEdge(player.col, row, 2, EDGE_HORIZONTAL) == true ) {
        if( player.state == PLAYER_STATE_JETPACK ) {
            crashedJetpack();
        }
        player.y = player.row * 8;
    } else {
        player.y = y;
    }
}

void playerMoveDown(void)
{
    y = player.y + PLAYER_SPEEDY;
    row = getRowForY(y);

    if( y > (PLAYAREA_ROWS*8)-24) {
        y = (PLAYAREA_ROWS*8)-24;
    }

    // touched solid block
    if( spriteTestEdge(player.col, row+3, 3, EDGE_HORIZONTAL) == true ) {

        // using jetpack
        if( player.state == PLAYER_STATE_JETPACK ) {

            if( checkChimney(row+3, player.col) == false && 
                checkChimney(row+3, player.col+1) == false ) {
                crashedJetpack();
            }
        }
        player.y = (player.row * 8)+7;
        
    } else {
        player.y = y;
    }

    // change to walking
    if( player.state == PLAYER_STATE_JETPACK && 
        player.y > PLAYER_STATE_SWITCH_Y-24 ) {
        playerChangeStateWalking();
    }
}

void playerMoveHorizontal(int8_t delta)
{
    player.facing = delta;
    x = player.x + (delta * PLAYER_SPEEDX);

    // left edge of screen
    if( x < 0 ) {
        // screen num +1 - screens go right to left
        if ( levelScreenIndex < NUM_SCREENS-1 && player.state == PLAYER_STATE_WALKING ) {
            player.x = 240;
            changeScreen(levelScreenIndex+1);
        } else {
            player.x = 0;
        }
        return;     
    }
    // right edge of screen
    else if( x > 240 ) {

        if( levelScreenIndex > 0 && player.state == PLAYER_STATE_WALKING ) {
            player.x = 0;
            // screen num-1 - screens go right to left
            changeScreen(levelScreenIndex-1);
        } else {
            player.x = 240;
        }
        return;
    }
    
    col = getColForX(x);
    
    // can't move, restrict to left of this block
    if( spriteTestEdge(col+(delta == 1 ? 2 : 0), player.row, 3, EDGE_VERTICAL) == true ) {
        if( player.state == PLAYER_STATE_JETPACK ) {
            crashedJetpack();
        }
        player.x = (col*8)+(delta == -1 ? 7 : 0);
    }
    // can move
    else {
        player.x = x;
    }
}

void playerNoMoveY(void)
{
    player.deltay = 0;

    // jetpack
    if( player.state == PLAYER_STATE_JETPACK && framecount % 2 == 0 ) {
        playerMoveDown();
    }
}
