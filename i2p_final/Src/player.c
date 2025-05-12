#include "player.h"
#include "game.h"
#include "utility.h"
#include "map.h"

#include <math.h>

static bool isCollision(Player* player, Map* map);

Player create_player(char * path, int row, int col){
    Player player;
    memset(&player, 0, sizeof(player));
    
    player.coord = (Point){
        col * TILE_SIZE,
        row * TILE_SIZE
    };
    
    player.speed = 4;
    player.health = 50;
    player.right_left = LEFT;
    player.status = PLAYER_IDLE;

    player.image = al_load_bitmap(path);
    if(!player.image){
        game_abort("Error Load Bitmap with path : %s", path);
    }
    
    return player;
}

void update_player(Player * player, Map* map){

    //death animation tick
    if (player->status == PLAYER_DYING) {
        player->death_animation_tick++;
        if (player->death_animation_tick >= 64) {
            player->status = PLAYER_DEAD;
        }
        return;
    }

    Point original = player->coord;
    
    // Knockback effect
    if(player->knockback_CD > 0){

        player->knockback_CD--;
        int next_x = player->coord.x + player->speed * cos(player->knockback_angle);
        int next_y = player->coord.y + player->speed * sin(player->knockback_angle);
        Point next;
        next = (Point){next_x, player->coord.y};
        
        if(!isCollision(player, map)){
            player->coord = next;
        }
        
        next = (Point){player->coord.x, next_y};
        if(!isCollision(player, map)){
            player->coord = next;
        }
    }
    
    /*
        [TODO HACKATHON 1-1]
        
        Player Movement
        Adjust the movement by player->speed
    */
    if (keyState[ALLEGRO_KEY_W]) {
        player->coord.y = player->coord.y - player->speed;
        player->direction = UP;
        player->status = PLAYER_WALKING;
    }
    if (keyState[ALLEGRO_KEY_S]) {
        player->coord.y = player->coord.y + player->speed;
        player->direction = DOWN;
        player->status = PLAYER_WALKING;
    }
    
        
    // if Collide, snap to the grid to make it pixel perfect
    if(isCollision(player, map)){
        player->coord.y = round((float)original.y / (float)TILE_SIZE) * TILE_SIZE;
    }
    
    /*
        [TODO HACKATHON 1-2] 
   
        Player Movement
        Add/Subtract the movement by player->speed
    */
    if (keyState[ALLEGRO_KEY_A]) {
        player->coord.x = player->coord.x - player->speed;
        player->direction = LEFT;
        player->right_left = LEFT;
        player->status = PLAYER_WALKING;
    }
    if (keyState[ALLEGRO_KEY_D]) {
        player->coord.x = player->coord.x + player->speed;
        player->direction = RIGHT;
        player->right_left = RIGHT;
        player->status = PLAYER_WALKING;
    }
    
    
    if(isCollision(player, map)){
        player->coord.x = round((float)original.x / (float)TILE_SIZE) * TILE_SIZE;
    }
    
    /*
        [TODO Homework] 
        
        Calculate the animation tick to draw animation later
    */
    player->animation_tick = (player->animation_tick + 1) % 32;

    //change walking->idle
    if (player->status == PLAYER_WALKING) {
        if (!keyState[ALLEGRO_KEY_W] && !keyState[ALLEGRO_KEY_S] && !keyState[ALLEGRO_KEY_A] && !keyState[ALLEGRO_KEY_D]) {
            player->status = PLAYER_IDLE;
            player->animation_tick = 0;
        }
    }

   
}

void draw_player(Player * player, Point cam){
    int dy = player->coord.y - cam.y; // destiny y axis
    int dx = player->coord.x - cam.x; // destiny x axis
    
    int flag = 0; // Change the flag to flip character
    
    /*
        [TODO Homework] 
        
        Draw Animation of Dying, Walking, and Idle
    */
    if (player->status == PLAYER_IDLE) {
        int offset = 32 * (int)(player->animation_tick / 16);
        int flag = player->right_left== RIGHT ? 1 : 0;

        ALLEGRO_COLOR tint = player->knockback_CD > 0
                            ? al_map_rgba(255, 0, 0, 128)
                            : al_map_rgb(255, 255, 255);

        al_draw_tinted_scaled_bitmap(player->image, tint,
            offset, 0, 32, 32, // source image x, y, width, height
            dx, dy, TILE_SIZE, TILE_SIZE, // destiny x, y, width, height
            flag // Flip or not
        );
    }
    else if (player->status == PLAYER_WALKING) {
        int offset = 32 * (int)(player->animation_tick / 8);
        int flag = player->right_left == RIGHT ? 1 : 0;

        ALLEGRO_COLOR tint = player->knockback_CD > 0
            ? al_map_rgba(255, 0, 0, 128)
            : al_map_rgb(255, 255, 255);

        al_draw_tinted_scaled_bitmap(player->image, tint,
            offset, 32, 32, 32, // source image x, y, width, height
            dx, dy, TILE_SIZE, TILE_SIZE, // destiny x, y, width, height
            flag // Flip or not
        );
    }
    else if(player->status == PLAYER_DYING){
        int offset = 32 * (int)(player->death_animation_tick / 16);
        int flag = player->right_left == RIGHT ? 1 : 0;

        al_draw_scaled_bitmap(player->image,
            offset, 64, 32, 32, // source x, source y, width, height
            dx, dy, TILE_SIZE, TILE_SIZE, // destiny x, y, width, height
            flag
        );
    }
       
#ifdef DRAW_HITBOX
    al_draw_rectangle(
        dx, dy, dx + TILE_SIZE, dy + TILE_SIZE,
        al_map_rgb(255, 0, 0), 1
    );
#endif
    
}

void delete_player(Player * player){
    al_destroy_bitmap(player->image);
}

static bool isCollision(Player* player, Map* map){

    if (player->coord.x < 0 ||
        player->coord.y < 0 ||
        (player->coord.x + TILE_SIZE - 1) / TILE_SIZE >= map->col ||
        (player->coord.y + TILE_SIZE - 1) / TILE_SIZE >= map->row)
        return true;

    /*

        [TODO HACKATHON 2-1] 
    
        Check every corner of enemy if it's collide or not

        We check every corner if it's collide with the wall/hole or not
        You can activate DRAW_HITBOX in utility.h to see how the collision work
    */
        if(!isWalkable(map->map[player->coord.y / TILE_SIZE][player->coord.x / TILE_SIZE])) return true;
        if(!isWalkable(map->map[(player->coord.y + TILE_SIZE - 1) / TILE_SIZE][player->coord.x / TILE_SIZE])) return true;
        if(!isWalkable(map->map[player->coord.y / TILE_SIZE][(player->coord.x + TILE_SIZE - 1) / TILE_SIZE])) return true;
        if(!isWalkable(map->map[(player->coord.y + TILE_SIZE - 1) / TILE_SIZE][(player->coord.x + TILE_SIZE - 1) / TILE_SIZE])) return true;

    
    
    return false;
}

void hitPlayer(Player * player, Point enemy_coord, int damage){
    if(player->knockback_CD == 0){
        float dY = player->coord.y - enemy_coord.y;
        float dX = player->coord.x - enemy_coord.x;
        float angle = atan2(dY, dX);

        /*
            [TODO Homework]
            
            Game Logic when the player get hit or die
        */
        player->knockback_angle = angle;
        player->knockback_CD = 32;

        player->health -= damage;

        if (player->health <= 0) {

            player->health = 0;
            player->status = PLAYER_DYING;
        }
    }
}
