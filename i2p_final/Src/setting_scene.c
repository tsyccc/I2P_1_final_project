#include <allegro5/allegro.h>
#include "setting_scene.h"
#include "menu_scene.h"
#include "utility.h"
#include "UI.h"
#include "game.h"

static Button backButton;
static Slider bgmSlider;
static Slider sfxSlider;
ALLEGRO_BITMAP* background;

static Slider create_slider(int type, float x, float y, float width, float height, float value, const char* image_path, const char* hovered_image_path) {
	Slider slider;
	slider.type = type;
	slider.x = x;
	slider.y = y;
	slider.width = width;
	slider.height = height;
	slider.value = value;
    slider.image = al_load_bitmap(image_path);
    slider.hovered_image = al_load_bitmap(hovered_image_path);
    slider.hovered = false;
	slider.pressed = false;
	return slider;
}


static void init(void) {
    //button
    backButton = button_create(SCREEN_W / 2 - 200, 650, 400, 100, "Assets/UI_Button.png", "Assets/UI_Button_hovered.png");

	//background
    background = al_load_bitmap("Assets/background.png");
    // slider, bgm
    //change_bgm("Assets/audio/menu_bgm.mp3");
    bgmSlider = create_slider(BGM, SCREEN_W / 2 - 200, 300, 400, 20, BGM_VOLUME,
                "Assets/slider.png", "Assets/slider_hovered.png");
    sfxSlider = create_slider(SFX, SCREEN_W / 2 - 200, 450, 400, 20, SFX_VOLUME,
                "Assets/slider.png", "Assets/slider_hovered.png");
}


static void update_slider(Slider* slider) {
    Point mouse = { mouseState.x, mouseState.y };
    RecArea rect = { slider->x, slider->y, slider->width, slider->height };


    if (mouse_in_rect(mouse, rect)) {
        slider->hovered = true;
        if (mouseState.buttons) {
			slider->pressed = true;
            slider->value = (float)(mouseState.x - slider->x) / slider->width;
        }
        else {
            if (slider->pressed == true) {
                slider->pressed = false;
                switch (slider->type) {
                case BGM:
                    change_bgm("Assets/audio/menu_bgm.mp3");
                    break;
                case SFX:
                    al_play_sample(al_load_sample("Assets/audio/shooting.mp3"), SFX_VOLUME, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                    break;
                }
            }
        }
    }
    else {
        if (slider->pressed == true) {
            slider->pressed = false;
            switch (slider->type) {
            case BGM:
                change_bgm("Assets/audio/menu_bgm.mp3");
                break;
            case SFX:
                al_play_sample(al_load_sample("Assets/audio/shooting.mp3"), SFX_VOLUME, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                break;
            }
        }
        slider->hovered = false;
    }
}

static void update(void) {
	// button
    update_button(&backButton);
    if (mouseState.buttons && backButton.hovered == true) {
        change_scene(create_menu_scene());
    }

    //slider
    update_slider(&bgmSlider);
    update_slider(&sfxSlider);

    // Update BGM and SFX volume
    BGM_VOLUME = bgmSlider.value;
    SFX_VOLUME = sfxSlider.value;
}


static void draw_slider(Slider slider) {
    ALLEGRO_BITMAP* image = slider.hovered ? slider.hovered_image : slider.image;
    al_draw_filled_rectangle(
        slider.x, slider.y,
        slider.x + slider.width, slider.y + slider.height,
        al_map_rgb(208, 208, 208)
    );
    al_draw_scaled_bitmap(image,
        0, 0, 600, 20,
        slider.x, slider.y,
        slider.width * slider.value, slider.height,
        0
    );
}

static void draw(void) {
    // background
    al_draw_bitmap(background, 0, 0, 0);
    // button
    draw_button(backButton);
    //button text
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

    //slider
    draw_slider(bgmSlider);
    draw_slider(sfxSlider);

    al_draw_text(
        P2_FONT,
        al_map_rgb(255, 255, 255),
        SCREEN_W / 2,
        300 - 50,
        ALLEGRO_ALIGN_CENTER,
        "BGM VOLUME"
    );

    al_draw_text(
        P2_FONT,
        al_map_rgb(255, 255, 255),
        SCREEN_W / 2,
        450 - 50,
        ALLEGRO_ALIGN_CENTER,
        "SFX VOLUME"
    );
}

static void destroy(void) {
    destroy_button(&backButton);
    al_destroy_bitmap(background);
    al_destroy_bitmap(bgmSlider.image);
    al_destroy_bitmap(bgmSlider.hovered_image);
    al_destroy_bitmap(sfxSlider.image);
    al_destroy_bitmap(sfxSlider.hovered_image);
}

Scene create_setting_scene(void) {
    Scene scene;
    memset(&scene, 0, sizeof(Scene));

    scene.name = "setting";
    scene.init = &init;
    scene.draw = &draw;
    scene.update = &update;
    scene.destroy = &destroy;

    return scene;
}
