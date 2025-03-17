#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"

#include <stdio.h>

#include "src/window.c"
#include "src/tiles.c"
#include "src/ships.c"
#include "src/anti_alias.c"
#include "src/map.c"
#include "src/enemies.c"
#include "src/player.c"
#include "src/main_menu.c"

typedef enum {
  MAIN_SCREEN,
  PLAYING,
  GAME_OVER,
} GameState;

int main() {
  init_window();
  init_map();
  init_tiles();
  init_ships();
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
      update_enemies(dt);

      for(int enemy_index = 0; enemy_index < TOTAL_ENEMIES; enemy_index++) {
        float enemy_size = 50;
        Rectangle enemy_rect = {enemies[enemy_index].position.x - (enemy_size/2), enemies[enemy_index].position.y - (enemy_size/2), enemy_size, enemy_size};

        for(int bullet_index = 0; bullet_index < TOTAL_BULLETS; bullet_index++) {
          float bullet_size = 30;
          Rectangle bullet_rect = {bullets[bullet_index].position.x - (bullet_size/2), bullets[bullet_index].position.y - (bullet_size/2), bullet_size, bullet_size};

          if(CheckCollisionRecs(enemy_rect, bullet_rect)) {
            enemies[enemy_index].alive = false;
            bullets[bullet_index].position.y = HALF_WINDOW_HEIGHT + 100;
          }
        }
      }
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
        render_enemies();
        render_bullets();
        render_player();
        end_anti_alias();

        for(int enemy_index = 0; enemy_index < TOTAL_ENEMIES; enemy_index++) {
          Vector2 screen_position = cartesian_to_screen(enemies[enemy_index].position);
          float size = 50;
          Rectangle enemy_rect = {screen_position.x - (size/2), screen_position.y - (size / 2), size, size};
          DrawRectangleLinesEx(enemy_rect, 3, MAGENTA);
        }

        for(int bullet_index = 0; bullet_index < TOTAL_BULLETS; bullet_index++) {
          Vector2 screen_position = cartesian_to_screen(bullets[bullet_index].position);
          float size = 30;
          Rectangle bullet_rect = {screen_position.x - (size/2), screen_position.y - (size/2), size, size};
          DrawRectangleLinesEx(bullet_rect, 3, LIME);
        }
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