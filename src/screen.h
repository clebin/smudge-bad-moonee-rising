#define NUM_OBJECT_TYPES 29
#define NUM_SCREENS 14

#define SCREEN_WIDTH 256
#define SCREEN_HEIGHT 192
#define SCREEN_COLS 32
#define SCREEN_ROWS 24

#define BARRIER_X 13
#define BARRIER_Y 15
#define BARRIER_CASTLE_X 27
#define BARRIER_CASTLE_Y 16

#define PLAYAREA_WIDTH 256
#define PLAYAREA_HEIGHT 184
#define PLAYAREA_COLS 32
#define PLAYAREA_ROWS 23

#define ROAD_ROW 14
#define OBJECT_NUM_ROAD1 11
#define OBJECT_NUM_ROAD2 26
#define OBJECT_NUM_ROAD3 27

#define SCREEN_NUM_LAUNDRETTE 0
#define SCREEN_NUM_BRICK 1
#define SCREEN_NUM_VICTORIAN 2
#define SCREEN_NUM_RURAL 3
#define SCREEN_NUM_FACTORY 4
#define SCREEN_NUM_CASTLE 5
#define SCREEN_NUM_POSH 6
#define SCREEN_NUM_DEAD_END 7
#define SCREEN_NUM_HIGH_STREET 8
#define SCREEN_NUM_CASTLE_CORNER 9
#define SCREEN_NUM_CORNER_SHOP 10
#define SCREEN_NUM_RURAL_2 11
#define SCREEN_NUM_TOWN_SQUARE 12
#define SCREEN_NUM_BUCK_HOUSE 13

// screen object maps
extern unsigned char laundretteObjects[];
extern unsigned char brickStreetObjects[];
extern unsigned char victorianStreetObjects[];
extern unsigned char ruralStreetObjects[];
extern unsigned char factoryObjects[];
extern unsigned char castleObjects[];
extern unsigned char poshStreetObjects[];
extern unsigned char deadEndStreetObjects[];
extern unsigned char highStreetObjects[];
extern unsigned char castleCornerObjects[];
extern unsigned char cornerShopObjects[];
extern unsigned char ruralStreet2Objects[];
extern unsigned char townSquareObjects[];
extern unsigned char buckHouseObjects[];

const unsigned char *ScreenMaps[NUM_SCREENS] = {
    laundretteObjects,
    brickStreetObjects,
    victorianStreetObjects, 
    ruralStreetObjects,
    factoryObjects,
    castleObjects,
    poshStreetObjects,
    deadEndStreetObjects,
    highStreetObjects,
    castleCornerObjects,
    cornerShopObjects,
    ruralStreet2Objects,
    townSquareObjects,
    buckHouseObjects,
};

// object tilemaps
// extern unsigned char barrierTilemap[];
extern unsigned char brickHouseTilemap[];
extern unsigned char buckHouseTilemap[];
extern unsigned char bush1Tilemap[];
extern unsigned char bush2Tilemap[];
extern unsigned char bush3Tilemap[];
extern unsigned char castleTilemap[];
extern unsigned char factoryTilemap[];
extern unsigned char laundretteTilemap[];
extern unsigned char phoneBoxTilemap[];
extern unsigned char poshHouseTilemap[];
extern unsigned char road1Tilemap[];
extern unsigned char road2Tilemap[];
extern unsigned char road3Tilemap[];
extern unsigned char rundownHouse1Tilemap[];
extern unsigned char rundownHouse2Tilemap[];
extern unsigned char ruralHouse1Tilemap[];
extern unsigned char streetlampTilemap[];
extern unsigned char tree1Tilemap[];
extern unsigned char victorianHouseTilemap[];
extern unsigned char shop1Tilemap[];
extern unsigned char shop2Tilemap[];
extern unsigned char titleBarTilemap[];
extern unsigned char pubTilemap[];
extern unsigned char pubSign1Tilemap[];
extern unsigned char pubSign2Tilemap[];
extern unsigned char townCrossTilemap[];
extern unsigned char signTilemap[];
extern unsigned char pavementTilemap[];

const unsigned char *GameObjects[NUM_OBJECT_TYPES] = {
    0,//barrierTilemap,
    brickHouseTilemap,
    buckHouseTilemap,
    bush1Tilemap,
    bush2Tilemap,
    bush3Tilemap,
    castleTilemap,
    factoryTilemap,
    laundretteTilemap,
    phoneBoxTilemap,
    poshHouseTilemap,
    road1Tilemap,
    rundownHouse1Tilemap,
    rundownHouse2Tilemap,
    ruralHouse1Tilemap,
    streetlampTilemap,
    tree1Tilemap,
    victorianHouseTilemap,
    shop1Tilemap,
    shop2Tilemap,
    titleBarTilemap,
    pubTilemap,
    pubSign1Tilemap,
    pubSign2Tilemap,
    townCrossTilemap,
    signTilemap, 
    road2Tilemap, 
    road3Tilemap,
    pavementTilemap
};

// chimneys
int8_t chimneySwept[NUM_SCREENS];

// get tileset
extern unsigned char tilesGraphics[];
extern unsigned char tilesColours[];

unsigned char tiles[256];

// this is defining that screen will go from 0 - 32 in width and 0-24 in height
struct sp1_Rect full_screen = {0, 0, SCREEN_COLS, SCREEN_ROWS};

// define screen
typedef struct Screen {
    uint8_t numEnemies;
    uint8_t exitLeft;
    uint8_t exitRight;
    uint8_t exitTop;
    uint8_t exitBottom;
};

// current screen number
uint8_t currentScreen[768];
uint8_t screenNum = 0;
uint8_t levelScreenIndex = 0;

// frame counter
uint16_t framecount = 0;

void changeScreen(int num);
void loadTiles(void);
void drawBackground(void);
void processObjectMap(uint8_t mapIndex);
void drawBarrier(void);
void drawCones(void);
void drawRoad(void);
