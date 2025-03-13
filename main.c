#include "raylib.h"

#define WINDOW_WIDTH  720
#define WINDOW_HEIGHT 900
#define HALF_WINDOW_WIDTH  (WINDOW_WIDTH  / 2)
#define HALF_WINDOW_HEIGHT (WINDOW_HEIGHT / 2)

#define ATLAS_SHIP_SIZE 32
#define ATLAS_TILE_SIZE 16
#define SHIP_SIZE_TARGET (ATLAS_SHIP_SIZE * 2)
#define TILE_SIZE_TARGET (ATLAS_TILE_SIZE * 2)

Texture2D ships_atlas_texture;
Texture2D tiles_atlas_texture;

Vector2 cartesian_to_screen(Vector2 v) {
  return (Vector2){v.x + HALF_WINDOW_WIDTH, -v.y + HALF_WINDOW_HEIGHT};
}

typedef struct {
  Vector2 position;
} Player;

void render_player(Player player) {
  int x = 1, y = 2;
  Rectangle source = {x * ATLAS_SHIP_SIZE, y * ATLAS_SHIP_SIZE, ATLAS_SHIP_SIZE, ATLAS_SHIP_SIZE};
  Vector2 position = cartesian_to_screen(player.position);
  Rectangle dest = {position.x, position.y, SHIP_SIZE_TARGET, SHIP_SIZE_TARGET};
  Vector2 origin = {SHIP_SIZE_TARGET/2, SHIP_SIZE_TARGET/2};
  float rotation = 0;
  Color color = WHITE;
  DrawTexturePro(ships_atlas_texture, source, dest, origin, rotation, color);
}

int main() {
  SetTraceLogLevel(LOG_WARNING);

  SetConfigFlags(FLAG_WINDOW_UNDECORATED);
  InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Shump");
  SetTargetFPS(60);

  ships_atlas_texture = LoadTexture("../assets/ships_packed.png");
  tiles_atlas_texture = LoadTexture("../assets/tiles_packed.png");

  Player player = {};

  while(!WindowShouldClose()) {
    float dt = GetFrameTime();

    int player_speed = 450;
    if(IsKeyDown(KEY_W)) player.position.y += player_speed * dt;
    if(IsKeyDown(KEY_A)) player.position.x -= player_speed * dt;
    if(IsKeyDown(KEY_S)) player.position.y -= player_speed * dt;
    if(IsKeyDown(KEY_D)) player.position.x += player_speed * dt;

    BeginDrawing();
    ClearBackground(RAYWHITE);

    render_player(player);

    DrawLineV((Vector2){HALF_WINDOW_WIDTH, 0},  (Vector2){HALF_WINDOW_WIDTH, WINDOW_HEIGHT}, RED);
    DrawLineV((Vector2){0, HALF_WINDOW_HEIGHT}, (Vector2){WINDOW_WIDTH, HALF_WINDOW_HEIGHT}, RED);

    EndDrawing();
  }

  CloseWindow();
  return 0;
}