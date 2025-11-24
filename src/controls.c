
void readControls(void)
{
    if( in_key_pressed(IN_KEY_SCANCODE_g)) {
        if( mode == MODE_PAUSED ) {
            mode = MODE_PLAYING;
        } else {
            mode = MODE_PAUSED;
        }
        z80_delay_ms(100);
    } else if( in_key_pressed(IN_KEY_SCANCODE_t) ) {
        mode = MODE_GAME_OVER;
    }
    
    in = (joy)(&joy_keys);

    // up
    if (in & IN_STICK_UP) {
        player.deltay = -1;
    }
    // down
    else if (in & IN_STICK_DOWN) {
        player.deltay = 1;
    }
    // not moving
    else {
        player.deltay = 0;
    }
    
    // left
    if (in & IN_STICK_LEFT) {
        player.deltax = -1;
    }
    // right
    else if (in & IN_STICK_RIGHT) {
        player.deltax = 1;
    }
    // not moving
    else {
        player.deltax = 0;
    }

    // fire button
    if (in & IN_STICK_FIRE) {
    }
    
}
