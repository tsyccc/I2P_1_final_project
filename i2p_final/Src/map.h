#ifndef map_h
#define map_h

#include <stdint.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>

#include "utility.h"

// IF YOU WANT TO INCREASE YOUR MAP SIZE OR ENEMY SIZE MAKE SURE TO INCREASE DEFINE VARIABLE BELOW IF IT EXCEED THE MAXIMUM
#define MAX_MAP_ROW 100
#define MAX_MAP_COL 100
#define MAX_ENEMY_SPAWN 100

/*
    Map Tiles type
    Hint : Some type might need a background, to configure it try to modify "get_map_offset(Map * map)"
*/
typedef enum _BLOCK_TYPE{
    FLOOR,
    WALL,
    DOOR_CLOSE,
    DOOR_OPENING,
    DOOR_OPEN,
    BUTTON,
    HOLE,
    COIN,
    ITEM,
    NOTHING
} BLOCK_TYPE;

typedef enum _COIN_STATUS {
    APPEAR,
    DISAPPEARING,
    DISAPPEAR
} COIN_STATUS;

typedef enum _DOOR_TYPE {
	DOOR_LEFT,
	DOOR_RIGHT,
}DOOR_TYPE;

// Map Struct
typedef struct Map_{
    uint8_t ** map; // Map array
    Point ** offset_assets; // Assets
    
    int row, col;
    
    // Map assets
    ALLEGRO_BITMAP* assets;
    ALLEGRO_BITMAP* coin_assets;
	ALLEGRO_BITMAP* item_assets;
    ALLEGRO_BITMAP* door_assets;
    ALLEGRO_BITMAP* button_assets;

    // Coin Properties
    ALLEGRO_SAMPLE* coin_audio;
    
    // Spawn Coordinate
    Point Spawn;
    Point EnemySpawn[MAX_ENEMY_SPAWN];
    char EnemyCode[MAX_ENEMY_SPAWN];
    uint8_t EnemySpawnSize;
	int enemy_counter;

    //coin status
    COIN_STATUS** coin_status;
    uint8_t coin_animation_tick;
    uint8_t coin_disappear_animation_tick;
    int coin_counter;

    //door status
	bool button_pressed;
	DOOR_TYPE** door_type;
    uint8_t door_animation_tick;

} Map;

/*
    MAP FUNCTION
    Feel free to add more if you have some idea or interaction with the map
 */
Map create_map(char * path, uint8_t type); // Create a map based on given file path
void draw_map(Map * map, Point cam); // Draw the map
void update_map(Map * map, Point player_coord, int * total_coins, int * player_speed); // Update map : you might want add some parameter here
void destroy_map(Map * map); // Destroy map

bool isWalkable(BLOCK_TYPE block);

#endif /* map_h */
