#include <allegro5/allegro.h>
#include <string.h>
#include "menu_scene.h"
#include "game_scene.h"
#include "setting_scene.h"
#include "loading_scene.h"
#include "utility.h"
#include "UI.h"
#include "game.h"

static Button settingButton;
ALLEGRO_BITMAP* background;

static void init(void) {
    settingButton = button_create(SCREEN_W / 2 - 200, 550, 400, 100, "Assets/UI_Button.png", "Assets/UI_Button_hovered.png");
	background = al_load_bitmap("Assets/background.png");
    change_bgm("Assets/audio/menu_bgm.mp3");
}

static void update(void) {
    update_button(&settingButton);
    if (keyState[ALLEGRO_KEY_ENTER]) {
        change_scene(create_loading_scene());
    }

    /*
        [TODO HACKATHON 4-3] 
        
        Change scene to setting scene when the button is pressed
    */
    if (mouseState.buttons && settingButton.hovered == true) {
        change_scene(create_setting_scene());
    }
}

static void draw(void) {
	// background
	al_draw_bitmap(background, 0, 0, 0);
    // Title Text
    al_draw_text(
        TITLE_FONT,
        al_map_rgb(146, 161, 185),
        SCREEN_W / 2,
        225,
        ALLEGRO_ALIGN_CENTER,
        "NTHU-RPG ADVENTURE"
    );
    al_draw_text(
        TITLE_FONT,
        al_map_rgb(199, 207, 221),
        SCREEN_W / 2,
        220,
        ALLEGRO_ALIGN_CENTER,
        "NTHU-RPG ADVENTURE"
    );

    al_draw_text(
        P2_FONT,
        al_map_rgb(255, 255, 255),
        SCREEN_W / 2,
        450,
        ALLEGRO_ALIGN_CENTER,
        "PRESS [ENTER] TO PLAY"
    );

    // button
    draw_button(settingButton);
    // button text
    al_draw_text(
        P2_FONT,
        al_map_rgb(66, 76, 110),
        SCREEN_W / 2,
        550 + 28 + settingButton.hovered * 11,
        ALLEGRO_ALIGN_CENTER,
        "SETTING"
    );
    al_draw_text(
        P2_FONT,
        al_map_rgb(225, 225, 225),
        SCREEN_W / 2,
        550 + 31 + settingButton.hovered * 11,
        ALLEGRO_ALIGN_CENTER,
        "SETTING"
    );
}

static void destroy(void) {
	al_destroy_bitmap(background);
    destroy_button(&settingButton);
}


Scene create_menu_scene(void) {
    Scene scene;
    memset(&scene, 0, sizeof(Scene));

    scene.name = "menu";
    scene.init = &init;
    scene.draw = &draw;
    scene.update = &update;
    scene.destroy = &destroy;

    return scene;
}
