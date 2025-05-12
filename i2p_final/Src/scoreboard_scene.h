#ifndef scoreboard_scene_h
#define scoreboard_scene_h

#include "utility.h"

typedef struct {
	int score;
	char name[10];
} Score;

Scene create_scoreboard_scene(void);
static void on_key_down(int keycode);

#endif /* scoreboard_scene_h */#pragma once
