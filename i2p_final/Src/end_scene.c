#include <allegro5/allegro.h>
#include "end_scene.h"
#include "menu_scene.h"
#include "game_scene.h"
#include "scoreboard_scene.h"
#include "utility.h"
#include "UI.h"
#include "game.h"

static Button backButton;
ALLEGRO_BITMAP* background;
static ALLEGRO_BITMAP* result;
static bool gameWin;
ALLEGRO_SAMPLE* win_audio;

static void init(void) {
    backButton = button_create(SCREEN_W / 2 - 200, 650, 400, 100, "Assets/UI_Button.png", "Assets/UI_Button_hovered.png");
    win_audio = al_load_sample("Assets/audio/win.mp3");
    background = al_load_bitmap("Assets/background.png");
    if (!win_audio) {
        game_abort("Can't load win audio");
    }
    if (gameWin) {
        result = al_load_bitmap("Assets/panda_win.png");
        al_play_sample(win_audio, SFX_VOLUME, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
    }
    else {
        result = al_load_bitmap("Assets/panda_lose.png");
    }
}

static void update(void) {
    update_button(&backButton);

    if (!gameWin) {
        if (mouseState.buttons && backButton.hovered == true) {
            change_scene(create_menu_scene());
        }
    }
    else {
        if (mouseState.buttons && backButton.hovered == true) {
            change_scene(create_scoreboard_scene());
        }
    }
    
}

static void draw(void) {
    // background
    al_draw_bitmap(background, 0, 0, 0);
    // draw result
    al_draw_scaled_bitmap(result, // image
        0, 0, 64, 64, // source x, source y, width, height
        SCREEN_W / 2 - 200, SCREEN_H / 2 - 200, 400, 400, // destiny x, destiny y, destiny width, destiny height
        0 // flag : set 0
    );

    // draw button
    draw_button(backButton);

    // draw button text

    if (!gameWin) {
        al_draw_text(
            P2_FONT,
            al_map_rgb(66, 76, 110),
            SCREEN_W / 2,
            650 + 28 + backButton.hovered * 11,
            ALLEGRO_ALIGN_CENTER,
            "BACK"
        );
        al_draw_text(
            P2_FONT,
            al_map_rgb(225, 225, 225),
            SCREEN_W / 2,
            650 + 31 + backButton.hovered * 11,
            ALLEGRO_ALIGN_CENTER,
            "BACK"
        );
    }
    
    else {
        al_draw_text(
            P2_FONT,
            al_map_rgb(66, 76, 110),
            SCREEN_W / 2,
            650 + 28 + backButton.hovered * 11,
            ALLEGRO_ALIGN_CENTER,
            "SCOREBOARD"
        );
        al_draw_text(
            P2_FONT,
            al_map_rgb(225, 225, 225),
            SCREEN_W / 2,
            650 + 31 + backButton.hovered * 11,
            ALLEGRO_ALIGN_CENTER,
            "SCOREBOARD"
        );
    }
}

static void destroy(void) {
    destroy_button(&backButton);
    al_destroy_bitmap(result);
    al_destroy_bitmap(background);
}

Scene create_end_scene(bool win) {
    Scene scene;
    memset(&scene, 0, sizeof(Scene));

    gameWin = win;
    scene.name = "end";
    scene.init = &init;
    scene.draw = &draw;
    scene.update = &update;
    scene.destroy = &destroy;

    return scene;
}