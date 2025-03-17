#include "raylib.h"
#include "raymath.h"

#include <stdio.h>

#include "stb_image_write.h"

#include "src/window.c"
#include "src/main_menu.c"
#include "src/player.c"
#include "src/editor.c"

typedef enum {
  MAIN_SCREEN,
  PLAYING,
  GAME_OVER,
  EDITOR,
} GameState;

Vector2 get_atlas_coord_from_tile_type(TileType tile_type) {
  switch(tile_type) {
    case PLANT: return (Vector2){0, 3};
    case SINGLE_TREE: return (Vector2){0, 4};
    case COUPLE_TREE: return (Vector2){0, 5};
    case HOUSE: return (Vector2){0, 6};
    case TENT: return (Vector2){0, 7};
    case FLAG: return (Vector2){0, 8};
    case LAMP_POST: return (Vector2){0, 9};

    case EXPLOSION_HOLE: return (Vector2){3, 9};
    case PLANTING_SOIL: return (Vector2){1, 9};

    case TOP_LEFT_SHORE: return (Vector2){1, 3};
    case TOP_MIDDLE_SHORE: return (Vector2){2, 3};
    case TOP_RIGHT_SHORE: return (Vector2){3, 3};
    case MIDDLE_LEFT_SHORE: return (Vector2){1, 4};
    case MIDDLE_RIGHT_SHORE: return (Vector2){3, 4};
    case BOTTOM_LEFT_SHORE: return (Vector2){1, 5};
    case BOTTOM_MIDDLE_SHORE: return (Vector2){2, 5};
    case BOTTOM_RIGHT_SHORE: return (Vector2){3, 5};

    case WATER: return (Vector2){5, 3};
    case SMALL_ISLAND: return (Vector2){4, 5};
    case LITTLE_ISLANDS: return (Vector2){5, 5};

    case TOP_LEFT_LAKE_SHORE: return (Vector2){4, 3};
    case TOP_RIGHT_LAKE_SHORE: return (Vector2){5, 3};
    case BOTTOM_LEFT_LAKE_SHORE: return (Vector2){4, 4};
    case BOTTOM_RIGHT_LAKE_SHORE: return (Vector2){5, 4};

    case TOP_LEFT_PATH: return (Vector2){1, 6};
    case TOP_MIDDLE_PATH: return (Vector2){2, 6};
    case TOP_RIGHT_PATH: return (Vector2){3, 6};
    case MIDDLE_LEFT_PATH: return (Vector2){1, 7};
    case MIDDLE_RIGHT_PATH: return (Vector2){3, 7};
    case BOTTOM_LEFT_PATH: return (Vector2){1, 8};
    case BOTTOM_MIDDLE_PATH: return (Vector2){2, 8};
    case BOTTOM_RIGHT_PATH: return (Vector2){3, 8};

    case FOUR_WAY_PATH: return (Vector2){2, 7};

    case T_JUNCTION_PATH_LEFT: return (Vector2){4, 6};
    case T_JUNCTION_PATH_RIGHT: return (Vector2){5, 6};
    case T_JUNCTION_PATH_DOWN: return (Vector2){4, 7};
    case T_JUNCTION_PATH_UP: return (Vector2){5, 7};

    case PATH_END_UP: return (Vector2){4, 8};
    case PATH_END_LEFT: return (Vector2){5, 8};
    case PATH_END_DOWN: return (Vector2){4, 9};
    case PATH_END_RIGHT: return (Vector2){5, 9};

    case GRASS:
    case TILE_TYPE_COUNT:
    default: return (Vector2){2, 4};
  }
}

TileType get_tile_type_from_atlas_coord(int x, int y) {
  if(x == 0 && y == 3) return PLANT;
  if(x == 0 && y == 4) return SINGLE_TREE;
  if(x == 0 && y == 5) return COUPLE_TREE;
  if(x == 0 && y == 6) return HOUSE;
  if(x == 0 && y == 7) return TENT;
  if(x == 0 && y == 8) return FLAG;
  if(x == 0 && y == 9) return LAMP_POST;

  if(x == 3 && y == 9) return EXPLOSION_HOLE;
  if(x == 1 && y == 9) return PLANTING_SOIL;

  if(x == 1 && y == 3) return TOP_LEFT_SHORE;
  if(x == 2 && y == 3) return TOP_MIDDLE_SHORE;
  if(x == 3 && y == 3) return TOP_RIGHT_SHORE;
  if(x == 1 && y == 4) return MIDDLE_LEFT_SHORE;
  if(x == 3 && y == 4) return MIDDLE_RIGHT_SHORE;
  if(x == 1 && y == 5) return BOTTOM_LEFT_SHORE;
  if(x == 2 && y == 5) return BOTTOM_MIDDLE_SHORE;
  if(x == 3 && y == 5) return BOTTOM_RIGHT_SHORE;

  if(x == 5 && y == 3) return WATER;
  if(x == 4 && y == 5) return SMALL_ISLAND;
  if(x == 5 && y == 5) return LITTLE_ISLANDS;

  if(x == 4 && y == 3) return TOP_LEFT_LAKE_SHORE;
  if(x == 5 && y == 3) return TOP_RIGHT_LAKE_SHORE;
  if(x == 4 && y == 4) return BOTTOM_LEFT_LAKE_SHORE;
  if(x == 5 && y == 4) return BOTTOM_RIGHT_LAKE_SHORE;

  if(x == 1 && y == 6) return TOP_LEFT_PATH;
  if(x == 2 && y == 6) return TOP_MIDDLE_PATH;
  if(x == 3 && y == 6) return TOP_RIGHT_PATH;
  if(x == 1 && y == 7) return MIDDLE_LEFT_PATH;
  if(x == 3 && y == 7) return MIDDLE_RIGHT_PATH;
  if(x == 1 && y == 8) return BOTTOM_LEFT_PATH;
  if(x == 2 && y == 8) return BOTTOM_MIDDLE_PATH;
  if(x == 3 && y == 8) return BOTTOM_RIGHT_PATH;

  if(x == 2 && y == 7) return FOUR_WAY_PATH;

  if(x == 4 && y == 6) return T_JUNCTION_PATH_LEFT;
  if(x == 5 && y == 6) return T_JUNCTION_PATH_RIGHT;
  if(x == 4 && y == 7) return T_JUNCTION_PATH_DOWN;
  if(x == 5 && y == 7) return T_JUNCTION_PATH_UP;

  if(x == 4 && y == 8) return PATH_END_UP;
  if(x == 5 && y == 8) return PATH_END_LEFT;
  if(x == 4 && y == 9) return PATH_END_DOWN;
  if(x == 5 && y == 9) return PATH_END_RIGHT;

  return GRASS;
}

TileType simpler_random_tile() {
  TileType allowed_tile_types[] = {
    // PLANT,
    // PLANT,
    // PLANT,
    // PLANT,
    // PLANT,
    // PLANT,
    // PLANT,
    // PLANT,
    // PLANT,
    PLANT,
    PLANT,
    SINGLE_TREE,
    SINGLE_TREE,
    SINGLE_TREE,
    COUPLE_TREE,
    COUPLE_TREE,
    COUPLE_TREE,
    // HOUSE,
    // TENT,
    // FLAG,
    // LAMP_POST,
    GRASS,
    GRASS,
    GRASS,
    GRASS,
    GRASS,
    GRASS,
    GRASS,
    GRASS,
    GRASS,
    GRASS,
    GRASS,
    GRASS,
    GRASS,
    GRASS,
    GRASS,
    GRASS,
    GRASS,
    GRASS,
    GRASS,
    GRASS,
    GRASS,
    GRASS,
    GRASS,
    GRASS,
    GRASS,
    GRASS,
    GRASS,
    GRASS,
    GRASS,
    GRASS,
    GRASS,
    GRASS,
    GRASS,
    GRASS,
    GRASS,
    GRASS,
    GRASS,
    GRASS,
    GRASS,
    GRASS,
    GRASS,
    GRASS,
    GRASS,
    GRASS,
    GRASS,
  };
  int size = sizeof(allowed_tile_types) / sizeof(allowed_tile_types[0]);
  int random_index = GetRandomValue(0, size - 1);
  return allowed_tile_types[random_index];
}

void init_tiles() {
  for(int row = 0; row < ROWS; row++) {
    for(int col = 0; col < COLS; col++) {
      Tile* tile = &background_tiles[row][col];
      TileType random_type = simpler_random_tile();
      tile->atlas_coord = get_atlas_coord_from_tile_type(random_type);
      float half_width_offset = TILE_SIZE_TARGET * ((COLS / 2) - (EXTRA_COLS_TO_FILL_SCREEN / 2));
      tile->position = (Vector2){-half_width_offset + col * TILE_SIZE_TARGET, row * TILE_SIZE_TARGET};
    }
  }
}

int main() {
  init_window();
  init_player();

  tiles_atlas_texture = LoadTexture("../assets/tiles_packed.png");
  init_tiles();

  GameState game_state = EDITOR;

  while(!WindowShouldClose()) {
    float dt = GetFrameTime();

    switch(game_state) {
      case MAIN_SCREEN: {
        if(IsKeyPressed(KEY_ENTER)) game_state = PLAYING;
        break;
      }
      case PLAYING: {
        input_player(dt);
        break;
      }
      case GAME_OVER: {
        break;
      }
      case EDITOR: {
        if(IsKeyPressed(KEY_SPACE)) {
         save_bg_to_file();
        }
        if(IsKeyPressed(KEY_ONE)) {
          is_editor = !is_editor;
        }
        break;
      }
    }

    BeginDrawing();
    ClearBackground(RAYWHITE);

    switch(game_state) {
      case MAIN_SCREEN: {
        draw_main_menu();
        render_player();
        break;
      }
      case PLAYING: {
        render_player();
        break;
      }
      case GAME_OVER: {
        break;
      }
      case EDITOR: {
        draw_editor();
        break;
      }
    }

    EndDrawing();

    update_frame(dt);
  }

  CloseWindow();
  return 0;
}