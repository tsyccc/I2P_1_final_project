#include <string.h>
#include "game_scene.h"
#include "menu_scene.h"
#include "loading_scene.h"
#include "end_scene.h"
#include "game.h"
#include "player.h"
#include "map.h"
#include "enemy.h"
#include "bullet.h"
#include "weapon.h"
#include "UI.h"
#include "utility.h"

#include <math.h>

Player player; // Player
Map map; // Map
enemyNode * enemyList; // Enemy List
BulletNode * bulletList; // Bullet List

// Weapon
Weapon weapon;
Weapon weapon1;
Weapon weapon2;
int now_weapon;

int coins_obtained;

//coin and heart icon
ALLEGRO_BITMAP* coin_icon;
ALLEGRO_BITMAP* heart_icon;

//game time
double game_time;
double last_update_time;

//game over, draw pop box
bool popup_box;
char user_name[10];
void on_key_down(int key_code);

static void init(void) {

    initEnemy();

    map = create_map("Assets/map0.txt", 0);

    player = create_player("Assets/panda2.png", map.Spawn.x, map.Spawn.y);

    enemyList = createEnemyList();
    bulletList = createBulletList();

    weapon1 = create_weapon("Assets/guns.png", "Assets/yellow_bullet.png", 16, 8, 10);
    weapon2 = create_weapon("Assets/sniper.png", "Assets/orange_bullet.png", 26, 16, 20);
    weapon = weapon1;
	now_weapon = 1;

    coins_obtained = 0;

    game_time = 0;
    last_update_time = al_get_time();

    popup_box = false;
    memset(user_name, 0, sizeof(user_name));

    for (int i = 0; i < map.EnemySpawnSize; i++) {
        Enemy enemy = createEnemy(map.EnemySpawn[i].x, map.EnemySpawn[i].y, map.EnemyCode[i]);
        insertEnemyList(enemyList, enemy);
    }

    coin_icon = al_load_bitmap("Assets/coin_icon.png");
    if (!coin_icon) {
        game_abort("Can't load coin assets");
    }
    heart_icon = al_load_bitmap("Assets/heart.png");
    if (!heart_icon) {
        game_abort("Can't load heart assets");
    }

    game_log("coord x:%d \n coords y:%d \n", map.Spawn.x, map.Spawn.y);
    change_bgm("Assets/audio/game_bgm.mp3");
}

//change weapon
static bool spaceKeyPressed = false;

static void update(void) {

    if (popup_box) {
        return;
    }

    /*
        [TODO Homework]

        Change the scene if winning/losing to win/lose scene

    */
    
    if (player.status == PLAYER_DEAD) {
        change_scene(create_end_scene(0));
        return;
    }
    else if (map.coin_counter == 0 && map.enemy_counter == 0) {
        static int gameover_counter = 0;
		gameover_counter++;
        if (gameover_counter >= 30) {
            popup_box = true;
        }
        return;
    }

    //game time
    if (player.status != PLAYER_DYING) {
        double current_time = al_get_time();
        game_time += (current_time - last_update_time);
        last_update_time = current_time;
    }

	//weapon switch
    if (keyState[ALLEGRO_KEY_SPACE]) {
        if (!spaceKeyPressed) {
            spaceKeyPressed = true;
            if (now_weapon == 1) {
                weapon = weapon2;
                now_weapon = 2;
            }
            else {
                weapon = weapon1;
                now_weapon = 1;
            }
        }
    }
    else {
        spaceKeyPressed = false;
    }

    update_player(&player, &map);

    Point Camera;
    /*
        [TODO HACKATHON 1-3]

        Calcualte the formula for the Camera
    */
    Camera.x = player.coord.x - (SCREEN_W / 2);
    Camera.y = player.coord.y - (SCREEN_H / 2);

    //Hint: Adjust it based on player position variable, then subtract it with half of the gameplay screen

//Camera = (Point){ 0, 0 };

	if (player.status != PLAYER_DYING) {
        updateEnemyList(enemyList, &map, &player, bulletList);
        update_weapon(&weapon, bulletList, player.coord, Camera);
	}
    
    updateBulletList(bulletList, enemyList, &map, &player);
    update_map(&map, player.coord, &coins_obtained, &player.speed);

}

void on_key_down(int keycode) {
	if (keycode == ALLEGRO_KEY_ESCAPE) {
		change_scene(create_menu_scene());
	}
    if (popup_box) {
        if (keycode == ALLEGRO_KEY_ENTER) {
            FILE* fp = fopen("./Assets/scoreboard.txt", "a");
            fprintf(fp, "%s %d\n", user_name, (int)game_time);
            fclose(fp);
            popup_box = false;
            change_scene(create_end_scene(1));
        }
        else if (keycode == ALLEGRO_KEY_SPACE) {
            memset(user_name, 0, sizeof(user_name));
        }
        else if (keycode == ALLEGRO_KEY_BACKSPACE) {
            if (strlen(user_name) > 0)
                user_name[strlen(user_name) - 1] = '\0';
        }
        else if (keycode >= ALLEGRO_KEY_A && keycode <= ALLEGRO_KEY_Z) { 
            if (strlen(user_name) < 9){
                user_name[strlen(user_name)] = 'A' + keycode - 1;
                user_name[strlen(user_name)] = '\0';
            }
        }
		else if (keycode >= ALLEGRO_KEY_0 && keycode <= ALLEGRO_KEY_9) {
			if (strlen(user_name) < 9){
				user_name[strlen(user_name)] = '0' + keycode - ALLEGRO_KEY_0;
				user_name[strlen(user_name)] = '\0';
			}
		}
    }
}

static void draw(void) {
    Point Camera;
    /*
        [TODO HACKATHON 1-4]

        Calcualte the formula for the Camera
    */
    Camera.x = player.coord.x - (SCREEN_W / 2);
    Camera.y = player.coord.y - (SCREEN_H / 2);

    //Hint: Just copy from the [TODO HACKATHON 1-3]

//Camera = (Point){ 0, 0 };

// Draw
    draw_map(&map, Camera);
    drawEnemyList(enemyList, Camera);
    drawBulletList(bulletList, Camera);
    draw_player(&player, Camera);
    if (player.status != PLAYER_DYING){
        draw_weapon(&weapon, player.coord, Camera);
    }
    

    /*
        [TODO Homework]
        
        Draw the UI of Health and Total Coins
    */

    int ui_x = 20;
    int ui_y = 20;                   
    int spacing_y = 50;             
    int icon_size = TILE_SIZE;       
    int text_offset_y = (icon_size - 36) / 2; //垂直對齊文字 文字36px

    al_draw_scaled_bitmap(
        heart_icon,
        0, 0, 32, 32,               
        ui_x, ui_y,                 
        icon_size, icon_size,       
        0
    );
    al_draw_textf(
        P2_FONT,
        al_map_rgb(255, 255, 255),
        ui_x + icon_size + 3,
        ui_y + text_offset_y,
        ALLEGRO_ALIGN_LEFT,
        "%02d", player.health
    );

    al_draw_scaled_bitmap(
        coin_icon,
        0, 0, 16, 16,
        ui_x, ui_y + spacing_y,     
        icon_size, icon_size,       
        0
    );

    al_draw_textf(
        P2_FONT,
        al_map_rgb(255, 255, 255),
        ui_x + icon_size + 3,      // 圖示右側的文字位置
        ui_y + spacing_y + text_offset_y,
        ALLEGRO_ALIGN_LEFT,
        "%02d", coins_obtained
    );

    //game time
	int minute = (int)(game_time / 60);
	int second = (int)(game_time) % 60;
	al_draw_textf(
		P2_FONT,
		al_map_rgb(255, 255, 255),
		SCREEN_W - 240, ui_y + text_offset_y,
		ALLEGRO_ALIGN_LEFT,
		"TIME : %02d:%02d", minute, second
	);

    //draw popup box
    if (popup_box == true) {
        al_draw_filled_rectangle(SCREEN_W / 2 - 200, SCREEN_H / 2 - 125, SCREEN_W / 2 + 200, SCREEN_H / 2 + 125, al_map_rgb(0, 0, 0));
		al_draw_rectangle(SCREEN_W / 2 - 200, SCREEN_H / 2 - 125, SCREEN_W / 2 + 200, SCREEN_H / 2 + 125, al_map_rgb(255, 255, 255), 5);
        al_draw_text(
            P2_FONT,
            al_map_rgb(255, 255, 255),
            SCREEN_W / 2, SCREEN_H / 2 - 80,
            ALLEGRO_ALIGN_CENTER,
            "Enter your name:"
        );
        al_draw_filled_rectangle(SCREEN_W / 2 - 130, SCREEN_H / 2 - 10, SCREEN_W / 2 + 130, SCREEN_H / 2 + 70, al_map_rgb(208, 208, 208));
        al_draw_text(
            P2_FONT,
            al_map_rgb(0, 0, 0),
            SCREEN_W / 2, SCREEN_H / 2 + 20,
            ALLEGRO_ALIGN_CENTER,
            user_name
        );
    }
   
}

static void destroy(void){
    delete_player(&player);
    delete_weapon(&weapon1);
    delete_weapon(&weapon2);
    destroy_map(&map);
    destroyBulletList(bulletList);
    destroyEnemyList(enemyList);
    terminateEnemy();
    al_destroy_bitmap(heart_icon);
    al_destroy_bitmap(coin_icon);
}


Scene create_game_scene(void){
    Scene scene;
    memset(&scene, 0, sizeof(Scene));
    
    scene.name = "game";
    scene.init = &init;
    scene.draw = &draw;
    scene.update = &update;
	scene.on_key_down = &on_key_down;
    scene.destroy = &destroy;

    
    return scene;
}
