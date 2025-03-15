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

typedef enum {
  GRASS,

  PLANT,
  SINGLE_TREE,
  COUPLE_TREE,
  HOUSE,
  TENT,
  FLAG,
  LAMP_POST,
  EXPLOSION_HOLE,
  PLANTING_SOIL,

  TOP_LEFT_SHORE,
  TOP_MIDDLE_SHORE,
  TOP_RIGHT_SHORE,
  MIDDLE_LEFT_SHORE,
  MIDDLE_RIGHT_SHORE,
  BOTTOM_LEFT_SHORE,
  BOTTOM_MIDDLE_SHORE,
  BOTTOM_RIGHT_SHORE,

  WATER,
  SMALL_ISLAND,
  LITTLE_ISLANDS,

  TOP_LEFT_LAKE_SHORE,
  TOP_RIGHT_LAKE_SHORE,
  BOTTOM_LEFT_LAKE_SHORE,
  BOTTOM_RIGHT_LAKE_SHORE,

  TOP_LEFT_PATH,
  TOP_MIDDLE_PATH,
  TOP_RIGHT_PATH,
  MIDDLE_LEFT_PATH,
  MIDDLE_RIGHT_PATH,
  BOTTOM_LEFT_PATH,
  BOTTOM_MIDDLE_PATH,
  BOTTOM_RIGHT_PATH,
  FOUR_WAY_PATH,

  T_JUNCTION_PATH_LEFT,
  T_JUNCTION_PATH_RIGHT,
  T_JUNCTION_PATH_DOWN,
  T_JUNCTION_PATH_UP,

  PATH_END_UP,
  PATH_END_LEFT,
  PATH_END_DOWN,
  PATH_END_RIGHT,

  TILE_TYPE_COUNT,
} TileType;

typedef struct {
  Vector2 atlas_coord;
  Vector2 position;
  TileType type;
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

typedef enum {
  MAIN_SCREEN,
  PLAYING,
  GAME_OVER,
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

TileType get_next_tile_type_on_the_right(TileType tile_type) {
  switch(tile_type) {
    case PLANT:
    case SINGLE_TREE:
    case COUPLE_TREE:
    case HOUSE:
    case TENT:
    case FLAG:
    case LAMP_POST:
    case EXPLOSION_HOLE:
    case PLANTING_SOIL:
    case GRASS:
    case MIDDLE_LEFT_SHORE:
    case TOP_RIGHT_LAKE_SHORE:
    case BOTTOM_RIGHT_LAKE_SHORE:
    case TOP_RIGHT_PATH:
    case MIDDLE_LEFT_PATH:
    case MIDDLE_RIGHT_PATH:
    case BOTTOM_RIGHT_PATH:
    case T_JUNCTION_PATH_RIGHT:
    case PATH_END_UP:
    case PATH_END_DOWN:
    case PATH_END_RIGHT: {
      TileType allowed_tile_types[] = {PLANT,SINGLE_TREE,COUPLE_TREE,HOUSE,TENT,FLAG,LAMP_POST,GRASS,EXPLOSION_HOLE,PLANTING_SOIL,MIDDLE_RIGHT_SHORE,TOP_LEFT_PATH,MIDDLE_LEFT_PATH,BOTTOM_LEFT_PATH,PATH_END_UP,PATH_END_DOWN,PATH_END_LEFT,T_JUNCTION_PATH_LEFT,TOP_LEFT_LAKE_SHORE,BOTTOM_LEFT_LAKE_SHORE};
      int size = sizeof(allowed_tile_types) / sizeof(allowed_tile_types[0]);
      int random_index = GetRandomValue(0, size - 1);
      return allowed_tile_types[random_index];
    }
    case TOP_LEFT_SHORE: {
      TileType allowed_tile_types[] = {TOP_MIDDLE_SHORE,TOP_RIGHT_SHORE,BOTTOM_RIGHT_LAKE_SHORE};
      int size = sizeof(allowed_tile_types) / sizeof(allowed_tile_types[0]);
      int random_index = GetRandomValue(0, size - 1);
      return allowed_tile_types[random_index];
    }
    case TOP_MIDDLE_SHORE: {
      TileType allowed_tile_types[] = {TOP_MIDDLE_SHORE,TOP_RIGHT_SHORE,BOTTOM_RIGHT_LAKE_SHORE};
      int size = sizeof(allowed_tile_types) / sizeof(allowed_tile_types[0]);
      int random_index = GetRandomValue(0, size - 1);
      return allowed_tile_types[random_index];
    }
    case TOP_RIGHT_SHORE:
    case MIDDLE_RIGHT_SHORE:
    case BOTTOM_RIGHT_SHORE: {
      TileType allowed_tile_types[] = {WATER,SMALL_ISLAND,LITTLE_ISLANDS};
      int size = sizeof(allowed_tile_types) / sizeof(allowed_tile_types[0]);
      int random_index = GetRandomValue(0, size - 1);
      return allowed_tile_types[random_index];
    }
    case BOTTOM_LEFT_SHORE: {
      TileType allowed_tile_types[] = {BOTTOM_MIDDLE_SHORE,TOP_RIGHT_LAKE_SHORE};
      int size = sizeof(allowed_tile_types) / sizeof(allowed_tile_types[0]);
      int random_index = GetRandomValue(0, size - 1);
      return allowed_tile_types[random_index];
    }
    case BOTTOM_MIDDLE_SHORE: {
      TileType allowed_tile_types[] = {BOTTOM_MIDDLE_SHORE,BOTTOM_RIGHT_SHORE,TOP_RIGHT_LAKE_SHORE};
      int size = sizeof(allowed_tile_types) / sizeof(allowed_tile_types[0]);
      int random_index = GetRandomValue(0, size - 1);
      return allowed_tile_types[random_index];
    }
    case WATER:
    case SMALL_ISLAND:
    case LITTLE_ISLANDS: {
      TileType allowed_tile_types[] = {WATER,SMALL_ISLAND,LITTLE_ISLANDS,TOP_LEFT_SHORE,MIDDLE_LEFT_SHORE,BOTTOM_LEFT_SHORE};
      int size = sizeof(allowed_tile_types) / sizeof(allowed_tile_types[0]);
      int random_index = GetRandomValue(0, size - 1);
      return allowed_tile_types[random_index];
    }
    case TOP_LEFT_LAKE_SHORE: {
      TileType allowed_tile_types[] = {TOP_RIGHT_LAKE_SHORE, BOTTOM_MIDDLE_SHORE,BOTTOM_RIGHT_SHORE};
      int size = sizeof(allowed_tile_types) / sizeof(allowed_tile_types[0]);
      int random_index = GetRandomValue(0, size - 1);
      return allowed_tile_types[random_index];
    }
    case BOTTOM_LEFT_LAKE_SHORE: {
      TileType allowed_tile_types[] = {BOTTOM_RIGHT_LAKE_SHORE, TOP_MIDDLE_SHORE,TOP_RIGHT_SHORE};
      int size = sizeof(allowed_tile_types) / sizeof(allowed_tile_types[0]);
      int random_index = GetRandomValue(0, size - 1);
      return allowed_tile_types[random_index];
    }
    case TOP_LEFT_PATH: {
      TileType allowed_tile_types[] = {TOP_MIDDLE_PATH,TOP_RIGHT_PATH,PATH_END_RIGHT,FOUR_WAY_PATH,T_JUNCTION_PATH_RIGHT,T_JUNCTION_PATH_DOWN,T_JUNCTION_PATH_UP};
      int size = sizeof(allowed_tile_types) / sizeof(allowed_tile_types[0]);
      int random_index = GetRandomValue(0, size - 1);
      return allowed_tile_types[random_index];
    }
    case TOP_MIDDLE_PATH: {
      TileType allowed_tile_types[] = {TOP_MIDDLE_PATH,TOP_RIGHT_PATH,PATH_END_RIGHT,FOUR_WAY_PATH,T_JUNCTION_PATH_RIGHT,T_JUNCTION_PATH_DOWN,T_JUNCTION_PATH_UP};
      int size = sizeof(allowed_tile_types) / sizeof(allowed_tile_types[0]);
      int random_index = GetRandomValue(0, size - 1);
      return allowed_tile_types[random_index];
    }
    case BOTTOM_LEFT_PATH:
    case BOTTOM_MIDDLE_PATH: {
      TileType allowed_tile_types[] = {BOTTOM_MIDDLE_PATH,BOTTOM_RIGHT_PATH,T_JUNCTION_PATH_DOWN,T_JUNCTION_PATH_UP,T_JUNCTION_PATH_RIGHT,PATH_END_LEFT,FOUR_WAY_PATH};
      int size = sizeof(allowed_tile_types) / sizeof(allowed_tile_types[0]);
      int random_index = GetRandomValue(0, size - 1);
      return allowed_tile_types[random_index];
    }

    case FOUR_WAY_PATH:
    case T_JUNCTION_PATH_LEFT:
    case T_JUNCTION_PATH_DOWN:
    case T_JUNCTION_PATH_UP:
    case PATH_END_LEFT: {
      TileType allowed_tile_types[] = {FOUR_WAY_PATH,T_JUNCTION_PATH_RIGHT,T_JUNCTION_PATH_DOWN,T_JUNCTION_PATH_UP,PATH_END_LEFT,TOP_MIDDLE_PATH,TOP_RIGHT_PATH,BOTTOM_MIDDLE_PATH,BOTTOM_RIGHT_PATH};
      int size = sizeof(allowed_tile_types) / sizeof(allowed_tile_types[0]);
      int random_index = GetRandomValue(0, size - 1);
      return allowed_tile_types[random_index];
    }

    case TILE_TYPE_COUNT:
    default: return GRASS;
  }
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

int main() {
  SetTraceLogLevel(LOG_WARNING);

  SetConfigFlags(FLAG_WINDOW_UNDECORATED);
  InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Shmup");
  SetRandomSeed((int)time(NULL));

  ships_atlas_texture = LoadTexture("../assets/ships_packed.png");
  tiles_atlas_texture = LoadTexture("../assets/tiles_packed.png");

  Player player = {0, -(HALF_WINDOW_HEIGHT / 2)};
  GameState game_state = MAIN_SCREEN;

  for(int col = 0; col < COLS; col++) {
    for(int row = 0; row < ROWS; row++) {
      Tile* tile = &background_tiles[col][row];
      TileType random_type = simpler_random_tile();
      if(row + 1 < ROWS) {
        TileType previous_type = background_tiles[col][row+1].type;
        if(previous_type == PLANT) random_type = PLANT;
        if(previous_type == SINGLE_TREE) random_type = SINGLE_TREE;
        if(previous_type == COUPLE_TREE) random_type = COUPLE_TREE;
      }
      tile->atlas_coord = get_atlas_coord_from_tile_type(random_type);
      // int random_type = GetRandomValue(0, TILE_TYPE_COUNT); // @todo: maybe add an array with the number of percent of each tile.
      // tile->atlas_coord = get_atlas_coord_from_tile_type(random_type);

      // if(col - 1 >= 0) {
      //   TileType previous_type_on_the_left = background_tiles[col-1][row].type;
      //   TileType next_type = get_next_tile_type_on_the_right(previous_type_on_the_left);
      //   tile->type = next_type;

      //   tile->atlas_coord = get_atlas_coord_from_tile_type(next_type);

      //   if(row - 1 >= 0) {
      //     /// @todo: Create function to get the next type based on the top tile
      //   }
      // }
      // tile->atlas_coord.x += 6;

      float half_width_offset = TILE_SIZE_TARGET * ((COLS / 2) - (EXTRA_COLS_TO_FILL_SCREEN / 2));
      tile->position = (Vector2){-half_width_offset + col * TILE_SIZE_TARGET, row * TILE_SIZE_TARGET};
    }
  }

  printf("tiles: %d\n", TILE_TYPE_COUNT);

  while(!WindowShouldClose()) {
    float dt = GetFrameTime();

    int player_speed = 450;
    if(IsKeyDown(KEY_W) && game_state == PLAYING) player.position.y += player_speed * dt;
    if(IsKeyDown(KEY_A) && game_state == PLAYING) player.position.x -= player_speed * dt;
    if(IsKeyDown(KEY_S) && game_state == PLAYING) player.position.y -= player_speed * dt;
    if(IsKeyDown(KEY_D) && game_state == PLAYING) player.position.x += player_speed * dt;
    if(IsKeyPressed(KEY_ENTER) && game_state == MAIN_SCREEN) game_state = PLAYING;

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
      }
    }

    render_player(player);


    static bool press_start_blink_state = false;
    static float current_frame = 0;
    const char* press_start_text = "Press Start";
    int font_size = 40;
    int press_start_text_width = MeasureText(press_start_text, font_size);
    Vector2 press_start_text_position = {HALF_WINDOW_WIDTH - (press_start_text_width / 2), HALF_WINDOW_HEIGHT};
    if(!press_start_blink_state && game_state == MAIN_SCREEN) {
      DrawText(press_start_text, press_start_text_position.x + 2, press_start_text_position.y + 3, font_size, BLACK);
      DrawText(press_start_text, press_start_text_position.x + 0, press_start_text_position.y + 0, font_size, MAROON);
    }
    current_frame += dt;

    if(current_frame >= 0.6) {
      current_frame = 0;
      press_start_blink_state = !press_start_blink_state;
    }

    EndDrawing();
  }

  CloseWindow();
  return 0;
}