#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"

#include <stdio.h>

#include "src/window.c"
#include "src/anti_alias.c"
#include "src/tiles.c"
#include "src/map.c"
#include "src/main_menu.c"
#include "src/player.c"

typedef enum {
  MAIN_SCREEN,
  PLAYING,
  GAME_OVER,
} GameState;

int main() {
  init_window();
  init_map();
  init_tiles();
  init_anti_alias();
  init_player();

  GameState game_state = MAIN_SCREEN;
  bool paused = false;

  while(!WindowShouldClose()) {
    float dt = GetFrameTime();

    switch(game_state) {
      case MAIN_SCREEN: {
        if(IsKeyPressed(KEY_ENTER)) game_state = PLAYING;
        break;
      }
      case PLAYING: {
        if(!paused) {
          input_player(dt);
        }

        if(IsKeyPressed(KEY_SPACE)) {
          paused = !paused;
        }
        break;
      }
      case GAME_OVER: {
        break;
      }
    }

    if(!paused) {
      update_map(dt);
      update_bullets(dt);
    }

    BeginDrawing();
    ClearBackground(RAYWHITE);

    switch(game_state) {
      case MAIN_SCREEN: {
        begin_anti_alias();
        render_map();
        render_player();
        end_anti_alias();

        draw_main_menu();
        break;
      }
      case PLAYING: {
        begin_anti_alias();
        render_map();
        render_bullets();
        render_player();
        end_anti_alias();
        break;
      }
      case GAME_OVER: {
        break;
      }
    }

    EndDrawing();

    update_frame(dt);
  }

  CloseWindow();
  return 0;
}