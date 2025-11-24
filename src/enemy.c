
// initialise enemies for new level
void initialiseEnemies(void)
{
    for(i=0;i<NUM_ENEMIES;i++) {
        initialiseEnemy(i, levelEnemies[i]);
    }
}

// initialise an individual enemy
void initialiseEnemy(uint8_t num, uint8_t enemyType)
{
    enemies[num].speed = 3;
    enemies[num].flying = true;
    enemies[num].deltax = 0;
    enemies[num].deltay = 0;
    enemies[num].type = enemyType;

    switch(enemyType) {
        case ENEMY_VAN:
            enemies[num].sp = getNewUnmaskedSprite((int *) vanSprite, 4, 3);
            enemies[num].movement = MOVEMENT_LEFT_RIGHT;
            enemies[num].flying = false;
            break;

        case ENEMY_SAUCER:
            enemies[num].sp = getNewUnmaskedSprite((int *) saucerSprite, 3, 2);
            enemies[num].movement = MOVEMENT_LEFT_RIGHT;
            enemies[num].speed = 4;
            break;

        case ENEMY_CLOUD:
            enemies[num].sp = getNewUnmaskedSprite((int *) cloudSprite, 4, 2);
            enemies[num].movement = MOVEMENT_LEFT_RIGHT;
            enemies[num].speed = 2;
            break;

        // case ENEMY_WASP:
        //     enemies[num].sp = getNewUnmaskedSprite((int *) waspSprite, 2, 2);
        //     enemies[num].movement = MOVEMENT_HOMING;
        //     break;

        case ENEMY_BUBBLE:
            enemies[num].sp = getNewMaskedSprite((int *) bubbleSprite, 2, 2);
            enemies[num].movement = MOVEMENT_REBOUND;
            enemies[num].flying = false;
            break;

        case ENEMY_FACE1:
            enemies[num].sp = getNewUnmaskedSprite((int *) faceSprite, 2, 2);
            enemies[num].movement = MOVEMENT_STATIC;
            enemies[num].flying = true;
            break;

        case ENEMY_FACE2:
            enemies[num].sp = getNewUnmaskedSprite((int *) face2Sprite, 2, 2);
            enemies[num].movement = MOVEMENT_STATIC;
            enemies[num].flying = true;
            break;

        default:
            enemies[num].sp = getNewMaskedSprite((int *) ballSprite, 1, 1);
            enemies[num].movement = MOVEMENT_REBOUND;
            enemies[num].flying = false;
            enemies[num].speed = 4;
            break;
    }
}

void resetEnemies(void)
{
    for(i=0;i<levelNumEnemies;i++) {
        moveSpriteOffScreen(enemies[i].sp);
        // spawn straight away
        enemies[i].spawnTime = 0;
    }
}

void setEnemyColour(uint8_t num)
{
    changeSpriteColour(enemies[num].sp, 65+(rand()%6));
}

void updateEnemies(void) {

    if( screenNum == SCREEN_NUM_LAUNDRETTE || 
        screenNum == SCREEN_NUM_LAUNDRETTE ) {
        return;
    }

    for(i=0;i<levelNumEnemies;i++) {
    
        // enemy is active
        if( enemies[i].spawnTime == -1 ) {

            switch( enemies[i].movement ) {
                case MOVEMENT_LEFT_RIGHT:
                    doMovementLeftRight(i);
                break;
                
                case MOVEMENT_REBOUND:
                    doMovementRebound(i);
                break;
                
                // case MOVEMENT_HOMING:
                //     doMovementHoming(i);
                // break;

                case MOVEMENT_STATIC:
                    doMovementStatic(i);
                break;
            }

            // draw the sprite
            sp1_MoveSprPix(enemies[i].sp, &full_screen, (void *) 0, enemies[i].x, enemies[i].y);
        }
        // ready to spawn
        else {
            if (enemies[i].spawnTime == 0 ) {
                spawnEnemy(i);
            }
            enemies[i].spawnTime--;
        }
    }
}

void doMovementStatic(uint8_t num)
{
    // count down lifetime
    enemies[num].lifeTime--;

    // remove enemy when time is up
    if( enemies[num].lifeTime <= 0) {
        removeEnemy(num);
    }
}

// void doMovementHoming(uint8_t num)
// {
//     if( enemies[num].x > player.x ) {
//         enemies[num].x--;
//         enemies[i].sp->frame = WASP_FRAME_LENGTH;
//     } else if( enemies[num].x < player.x ) {
//         enemies[num].x++;
//         enemies[i].sp->frame = 0;
//     }

//     if( enemies[num].y > player.y ) {
//         enemies[num].y--;
//     } else if( enemies[num].y < player.y ) {
//         enemies[num].y++;
//     }
// }

void doMovementLeftRight(uint8_t num)
{
    enemies[num].x += enemies[num].deltax * enemies[num].speed;

    if( enemies[num].x > SCREEN_WIDTH || 
        enemies[num].x < (0-enemies[num].sp->width*8) ) {  
        // remove enemy
        removeEnemy(num);
    }
}

void removeEnemy(uint8_t num)
{
    enemies[num].x = -100;
    enemies[num].y = -100;
    moveSpriteOffScreen(enemies[num].sp);
    enemies[num].spawnTime = rand() % 100;
}

void doMovementRebound(uint8_t num)
{
    // get new x & y position after movement
    int16_t newy = enemies[num].y + (enemies[num].deltay * enemies[num].speed);
    int8_t newrow = getRowForY(newy);

    // moving up
    if( enemies[num].deltay != 0 ) {

        if( enemies[num].deltay > 0 ) {
            n = newrow + enemies[num].sp->height;
        }
        else {
            n = newrow;
        }

        // test top edge of screen
        if( newy < 0 || n > PLAYAREA_ROWS ||
            spriteTestEdge(enemies[num].sp->col, n, (uint8_t) enemies[num].sp->width, EDGE_HORIZONTAL) == true ) {
            enemies[num].deltay = -enemies[num].deltay;
            newy = enemies[num].y + enemies[num].speed;
            newrow = getRowForY(newy);
        }
    }

    int16_t newx = enemies[num].x + (enemies[num].deltax * enemies[num].speed);
    int8_t newcol = getColForX(newx); 

    // moving left
    if( enemies[num].deltax != 0 ) {
        
        if( enemies[num].deltax > 0 ) {
            n = newcol + enemies[num].sp->width;
        }
        else {
            n = newcol;
        }

        // left edge of screen
        if( newx < 0 || n > PLAYAREA_COLS || 
            spriteTestEdge(newcol, newrow, (uint8_t) enemies[num].sp->height, EDGE_VERTICAL) == true ) {
            enemies[num].deltax = -enemies[num].deltax;
            newx = enemies[num].x + enemies[num].speed;
        }
    }
    enemies[num].x = newx;
    enemies[num].y = newy;
}

void spawnEnemy(uint8_t num)
{
    // sky
    if( enemies[i].flying == true ) {
        row = (rand() % 5);
        setEnemyColour(num);
    }
    // ground
    else {
        row = PLAYAREA_ROWS - 3 - (rand() % 3);
    }
    enemies[i].y = row*8;
    
    // spawn
    switch(enemies[num].movement) {
        case MOVEMENT_LEFT_RIGHT:
            spawnEnemyLeftRight(i);
        break;

        case MOVEMENT_STATIC:
            spawnEnemyStatic(i);
            break;
        
        default:
            spawnEnemyRebound(i);
        break;
    }
}

void spawnEnemyStatic(uint8_t num)
{
    spawnEnemyRandomX(num);
    enemies[num].lifeTime = rand() % 100;
}

void spawnEnemyLeftRight(uint8_t num)
{
    if( player.state != enemies[num].flying ) {
        t = rand() % 2;
    }
    else if( player.x > 160 ) {
        t = 0;
    } else {
        t = 1;
    }
    // left-to-right
    if( t == 0 ) {
        enemies[num].deltax = 1;
        enemies[num].x = -32;        
        enemies[num].sp->frame = 0;
    }
    // right-to-left
    else {
        enemies[num].deltax = -1;
        enemies[num].x = SCREEN_WIDTH;
        if( enemies[num].type == ENEMY_VAN) {
            enemies[num].sp->frame = 128;
        }
    }
}

void spawnEnemyRandomX(uint8_t num)
{
    col = rand() % 24;
    if( player.x > 160 ) {
        enemies[num].x = col*8;
    } else {
        enemies[num].x = 64+(col*8);
    }
}

void spawnEnemyRebound(uint8_t num)
{
    spawnEnemyRandomX(num);

    switch(rand() % 3) {
        case 0:
            enemies[num].deltax =-1;
            enemies[num].deltay =-1;
            break;
        case 1:
            enemies[num].deltax = 0;
            enemies[num].deltay = -1;
            break;
        case 2:
            enemies[num].deltax = 1;
            enemies[num].deltay = 1;
            break;
    }
}

void checkCollision(uint8_t num)
{
    if( !(
        player.x+4 >= enemies[num].x+(enemies[num].sp->width*8)-8 ||
        player.x+12 <= enemies[num].x || 
        player.y >= enemies[num].y+(enemies[num].sp->height*8)-8 ||
        player.y+24 <= enemies[num].y
    )) {
        // collision occurred - take power away
        playerPower -= PENALTY_TOUCH_ENEMY;

        // play sound
        bit_beepfx_di(BEEPFX_HIT_4);

        // flash border
        zx_border(INK_RED);
    }
}
