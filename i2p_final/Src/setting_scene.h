#ifndef setting_scene_h
#define setting_scene_h

#include "utility.h"

typedef enum _SliderType {
	BGM,
	SFX
}SliderType;

typedef struct _slider {
	float x, y;
	float width, height;
	float value;
	int type;
	ALLEGRO_BITMAP* image;
	ALLEGRO_BITMAP* hovered_image;
	bool hovered;
	bool pressed;
}Slider;

Scene create_setting_scene(void);

#endif /* setting_scene_h */
