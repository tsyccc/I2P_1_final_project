#include <allegro5/allegro.h>
#include "scoreboard_scene.h"
#include "menu_scene.h"
#include "utility.h"
#include "UI.h"
#include "game.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void on_key_down(int key_code);
int scoreboard_count = 0;
Score scoreboard[100];
ALLEGRO_BITMAP* background;

int cmp(const void* a, const void* b) {
    const Score* scoreA = (const Score*)a;
    const Score* scoreB = (const Score*)b;
    return scoreA->score - scoreB->score; //¤É§Ç
}

void sort_score(Score* scoreboard, int size) {
    qsort(scoreboard, size, sizeof(Score), cmp);
}

static void init(void) {
    background = al_load_bitmap("Assets/background.png");
    FILE* file = fopen("./Assets/scoreboard.txt", "r");
	if (!file) {
		game_abort("Can't open scoreboard file");
	}

    while (fscanf_s(file, "%s %d",
            scoreboard[scoreboard_count].name,
            (unsigned)_countof(scoreboard[scoreboard_count].name),
            &scoreboard[scoreboard_count].score) != EOF) {
        scoreboard_count++;
    }
    fclose(file);
	sort_score(scoreboard, scoreboard_count);    
}

static void update(void) {
    
}

static void draw(void) {
    // background
    al_draw_bitmap(background, 0, 0, 0);

    al_draw_text(
        TITLE_FONT,
        al_map_rgb(146, 161, 185),
        SCREEN_W / 2,
        105,
        ALLEGRO_ALIGN_CENTER,
        "LEADERBOARD"
    );
    al_draw_text(
        TITLE_FONT,
        al_map_rgb(199, 207, 221),
        SCREEN_W / 2,
        100,
        ALLEGRO_ALIGN_CENTER,
        "LEADERBOARD"
    );

    al_draw_text(
        P2_FONT,
        al_map_rgb(255, 255, 255),
        SCREEN_W / 2,
        SCREEN_H - 100,
        ALLEGRO_ALIGN_CENTER,
        "[ESC] BACK TO MENU"
    );

    for (int i=0 ; i<10 ; i++) {
        al_draw_textf(
            P3_FONT,
            al_map_rgb(255, 255, 255),
            200,
            190 + 50 * i,
            ALLEGRO_ALIGN_LEFT,
            "%-2d.  %s",
            i + 1,
            scoreboard[i].name
        );

        if (scoreboard[i].score != 0) {
            al_draw_textf(
                P3_FONT,
                al_map_rgb(255, 255, 255),
                560,
                190 + 50 * i,
                ALLEGRO_ALIGN_CENTER,
                "%02d:%02d",
                scoreboard[i].score / 60,
                scoreboard[i].score % 60
            );
        }
    }  
}

void on_key_down(int keycode) {
    if (keycode == ALLEGRO_KEY_ESCAPE) {
        change_scene(create_menu_scene());
    }
}

static void destroy(void) {
    al_destroy_bitmap(background);
}

Scene create_scoreboard_scene(void) {
    Scene scene;
    memset(&scene, 0, sizeof(Scene));

    scene.name = "scoreboard";
    scene.init = &init;
    scene.draw = &draw;
    scene.update = &update;
    scene.destroy = &destroy;
	scene.on_key_down = &on_key_down;

    return scene;
}