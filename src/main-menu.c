
void mainMenu(void)
{
    sp1_Invalidate(&full_screen);

    zx_border(INK_BLACK);
    clearScreen();

    showTitle();

    // start
    printString(17, 8, INK_GREEN|BRIGHT, "0. OFF TO WORK");
    
    // show high score
    printTile(SCREEN_ROWS-1, SCREEN_COLS-7, TILE_HI_LABEL);
    printNum(SCREEN_ROWS-1, SCREEN_COLS-6, INK_WHITE|PAPER_BLACK|BRIGHT, highScore, 5);
    
    showControls();

    // print info
    // set keyboard controls
    framecount = 0;
    while(1) {

        // keyboard
        if(in_key_pressed(IN_KEY_SCANCODE_1)) {
            selectControls(0);
        }
        // kempston
        else if(in_key_pressed(IN_KEY_SCANCODE_2)) {
            selectControls(1);
        }
        // sinclair
        else if(in_key_pressed(IN_KEY_SCANCODE_3)) {
            selectControls(2);
        } else if(in_key_pressed(IN_KEY_SCANCODE_0)) {
            // start
            break;
        }

        framecount++;
    }
    
    bit_beepfx_di(BEEPFX_ITEM_4);

    // an extra bit of randomisation
    srand(framecount);
}

void selectControls(uint8_t num)
{
    joy = (JOYFUNC)controlOptions[num];
    bit_beepfx_di(BEEPFX_ITEM_3);
    // la marseillaise on the beeper??
    // bit_play_di("FF1F2Bb9Bb9C9C9F9C9Bb9");
    showControls();
}

void showTitle(void)
{
    count = 0;
    x = TITLE_X;
    y = TITLE_Y;

    for(i=0;i<TITLE_LEN;i++) {
        if( i % 9 == 0 ) {
            y++;
            x = TITLE_X;
        }
        n = menuTitleTilemap[i];
        printTile(y, x, n);

        x++;
    }
}

void showControls(void)
{
    uint8_t colour;
    
    // keyboard
    for(i=0;i<3;i++) {
        if( joy == controlOptions[i] ) {
            colour = BRIGHT|PAPER_BLUE|menuInk[i];
        } else {
            colour = BRIGHT|PAPER_BLACK|menuInk[i];
        }
        printString(10+(i*2), 9, colour, menuItems[i]);
    }
    sp1_UpdateNow();
}
