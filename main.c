#include "raylib.h"

int main() {
  SetTraceLogLevel(LOG_WARNING);

  #define WINDOW_WIDTH  720
  #define WINDOW_HEIGHT 900

  SetConfigFlags(FLAG_WINDOW_UNDECORATED);
  InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Shump");
  SetTargetFPS(60);

  Texture2D ships_atlas_texture = LoadTexture("../assets/ships_packed.png");

  while(!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    DrawTexture(ships_atlas_texture, 0, 0, WHITE);

    EndDrawing();
  }

  CloseWindow();
  return 0;
}