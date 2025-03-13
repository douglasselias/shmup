#include "raylib.h"
#include "raymath.h"

#include <stdio.h>
#include <time.h>

#define WINDOW_WIDTH  720
#define WINDOW_HEIGHT 928
#define HALF_WINDOW_WIDTH  (WINDOW_WIDTH  / 2)
#define HALF_WINDOW_HEIGHT (WINDOW_HEIGHT / 2)

#define ATLAS_SHIP_SIZE 32
#define ATLAS_TILE_SIZE 16
#define SHIP_SIZE_TARGET (ATLAS_SHIP_SIZE * 2)
#define TILE_SIZE_TARGET (ATLAS_TILE_SIZE * 2)

#define EXTRA_COLS_TO_FILL_SCREEN 2
#define COLS ((WINDOW_WIDTH  / TILE_SIZE_TARGET) + EXTRA_COLS_TO_FILL_SCREEN)
#define ROWS ((WINDOW_HEIGHT / TILE_SIZE_TARGET) + 1)

typedef struct {
  Vector2 atlas_coord;
  Vector2 position;
} Tile;

Tile background_tiles[COLS][ROWS] = {0};

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

void render_tile(Tile tile) {
  Rectangle source = {tile.atlas_coord.x * ATLAS_TILE_SIZE, tile.atlas_coord.y * ATLAS_TILE_SIZE, ATLAS_TILE_SIZE, ATLAS_TILE_SIZE};
  Vector2 position = cartesian_to_screen(tile.position);
  Rectangle dest = {position.x, position.y, TILE_SIZE_TARGET, TILE_SIZE_TARGET};
  Vector2 origin = {TILE_SIZE_TARGET/2, TILE_SIZE_TARGET/2};
  float rotation = 0;
  Color color = Fade(WHITE, 0.75f);
  DrawTexturePro(tiles_atlas_texture, source, dest, origin, rotation, color);
}

int main() {
  // printf("Cols: %.2f\n", (float)WINDOW_WIDTH  / TILE_SIZE_TARGET);
  // printf("COLS: %.2f\n", (float)COLS);
  // printf("Rows: %.2f\n", (float)WINDOW_HEIGHT / TILE_SIZE_TARGET);
  // printf("ROWS: %.2f\n", (float)ROWS);
  SetTraceLogLevel(LOG_WARNING);

  SetConfigFlags(FLAG_WINDOW_UNDECORATED);
  InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Shump");
  SetRandomSeed((int)time(NULL));

  ships_atlas_texture = LoadTexture("../assets/ships_packed.png");
  tiles_atlas_texture = LoadTexture("../assets/tiles_packed.png");

  Player player = {};

  for(int col = 0; col < COLS; col++) {
    for(int row = 0; row < ROWS; row++) {
      background_tiles[col][row].atlas_coord = (Vector2){GetRandomValue(0, 11), GetRandomValue(3, 9)};
      float half_width_offset = TILE_SIZE_TARGET * ((COLS / 2) - (EXTRA_COLS_TO_FILL_SCREEN / 2));
      background_tiles[col][row].position = (Vector2){-half_width_offset + col * TILE_SIZE_TARGET, row * TILE_SIZE_TARGET};
    }
  }

  while(!WindowShouldClose()) {
    float dt = GetFrameTime();

    int player_speed = 450;
    if(IsKeyDown(KEY_W)) player.position.y += player_speed * dt;
    if(IsKeyDown(KEY_A)) player.position.x -= player_speed * dt;
    if(IsKeyDown(KEY_S)) player.position.y -= player_speed * dt;
    if(IsKeyDown(KEY_D)) player.position.x += player_speed * dt;

    for(int col = 0; col < COLS; col++) {
      for(int row = 0; row < ROWS; row++) {
        Tile* tile = &background_tiles[col][row];
        tile->position.y -= 100 * dt;
        tile->position.y = Wrap(tile->position.y, -HALF_WINDOW_HEIGHT - TILE_SIZE_TARGET/2, HALF_WINDOW_HEIGHT + TILE_SIZE_TARGET/2);
      }
    }

    BeginDrawing();
    ClearBackground(RAYWHITE);

    for(int col = 0; col < COLS; col++) {
      for(int row = 0; row < ROWS; row++) {
        Tile tile = background_tiles[col][row];
        render_tile(tile);
        Vector2 screen_position = cartesian_to_screen(tile.position);
        // DrawRectangleLinesEx((Rectangle){screen_position.x - ATLAS_TILE_SIZE, screen_position.y - ATLAS_TILE_SIZE, TILE_SIZE_TARGET, TILE_SIZE_TARGET}, 2, RED);
      }
    }

    render_player(player);

    // DrawLineV((Vector2){HALF_WINDOW_WIDTH, 0},  (Vector2){HALF_WINDOW_WIDTH, WINDOW_HEIGHT}, RED);
    // DrawLineV((Vector2){0, HALF_WINDOW_HEIGHT}, (Vector2){WINDOW_WIDTH, HALF_WINDOW_HEIGHT}, RED);

    EndDrawing();
  }

  CloseWindow();
  return 0;
}