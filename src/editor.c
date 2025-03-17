#define ATLAS_TILE_SIZE 16
#define TILE_SIZE_TARGET (ATLAS_TILE_SIZE * 2)

#define ROWS ((WINDOW_HEIGHT / TILE_SIZE_TARGET) + 1)
#define EXTRA_COLS_TO_FILL_SCREEN 2
#define COLS ((WINDOW_WIDTH  / TILE_SIZE_TARGET) + EXTRA_COLS_TO_FILL_SCREEN)

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

Tile background_tiles[ROWS][COLS];
Texture2D tiles_atlas_texture;

bool is_editor = false;
Vector2 current_tile;
Camera2D camera_editor = {.zoom = 1,.target = {HALF_WINDOW_WIDTH, HALF_WINDOW_HEIGHT}};

Vector2 cartesian_to_texture_opengl(Vector2 v) {
  return (Vector2){v.x + HALF_WINDOW_WIDTH, v.y + HALF_WINDOW_HEIGHT};
}

void render_tile(Tile tile) {
  Rectangle source = {tile.atlas_coord.x * ATLAS_TILE_SIZE, tile.atlas_coord.y * ATLAS_TILE_SIZE, ATLAS_TILE_SIZE, ATLAS_TILE_SIZE};
  Vector2 position = cartesian_to_screen(tile.position);
  Rectangle dest = {position.x, position.y, TILE_SIZE_TARGET, TILE_SIZE_TARGET};
  Vector2 origin = {TILE_SIZE_TARGET/2, TILE_SIZE_TARGET/2};
  float rotation = 0;
  // Color color = Fade(WHITE, 0.75f);
  Color color = WHITE;
  DrawTexturePro(tiles_atlas_texture, source, dest, origin, rotation, color);
}

void render_tile_on_texture(Tile tile) {
  Rectangle source = {tile.atlas_coord.x * ATLAS_TILE_SIZE, tile.atlas_coord.y * ATLAS_TILE_SIZE, ATLAS_TILE_SIZE, ATLAS_TILE_SIZE};
  Vector2 position = cartesian_to_texture_opengl(tile.position);
  Rectangle dest = {position.x, position.y, TILE_SIZE_TARGET, TILE_SIZE_TARGET};
  Vector2 origin = {TILE_SIZE_TARGET/2, TILE_SIZE_TARGET/2};
  float rotation = 0;
  Color color = Fade(WHITE, 0.75f);
  DrawTexturePro(tiles_atlas_texture, source, dest, origin, rotation, color);
}

void save_bg_to_file() {
  RenderTexture2D render_texture = LoadRenderTexture(WINDOW_WIDTH, WINDOW_HEIGHT);
  BeginTextureMode(render_texture);
  int radius = 30;
  Vector2 position = {0,0};
  Vector2 texture_pos = cartesian_to_texture_opengl(position);
  // DrawCircle(texture_pos.x, texture_pos.y, radius, RED);

  for(int row = 0; row < ROWS; row++) {
    for(int col = 0; col < COLS; col++) {
      Tile* tile = &background_tiles[row][col];
      tile->position.y = Wrap(tile->position.y, -HALF_WINDOW_HEIGHT - TILE_SIZE_TARGET/2, HALF_WINDOW_HEIGHT + TILE_SIZE_TARGET/2);
      render_tile_on_texture(*tile);
    }
  }
  EndTextureMode();

  Image img = LoadImageFromTexture(render_texture.texture);
  ImageFlipVertical(&img);
  int number_of_color_channels = 4;
  int stride_in_bytes = 0;
  int result = stbi_write_png("../assets/bg.png", WINDOW_WIDTH, WINDOW_HEIGHT, number_of_color_channels, img.data, stride_in_bytes);
  if(result == 1) printf("Img Saved!");
}

float scale_factor = 1.88f;
void draw_source_tilemap() {
  Vector2 origin = {0,0};
  float rotation = 0;
  Color color = WHITE;

  for(int row = 0; row < 10; row++) {
    for(int col = 0; col < 12; col++) {
      Rectangle source = {(float)col * ATLAS_TILE_SIZE, (float)row * ATLAS_TILE_SIZE, ATLAS_TILE_SIZE, ATLAS_TILE_SIZE};
      Rectangle dest = {col * TILE_SIZE_TARGET*scale_factor, row * TILE_SIZE_TARGET*scale_factor, TILE_SIZE_TARGET*scale_factor, TILE_SIZE_TARGET*scale_factor};
      DrawTexturePro(tiles_atlas_texture, source, dest, origin, rotation, color);
    }
  }
}

void draw_editor() {
  Vector2 mouse_position = GetMousePosition();

  if(is_editor) {
    for(int row = 0; row < ROWS; row++) {
      for(int col = 0; col < COLS; col++) {
        Tile tile = background_tiles[row][col];
        render_tile(tile);
      }
    }
    Rectangle mouse_rect = {mouse_position.x, mouse_position.y, TILE_SIZE_TARGET, TILE_SIZE_TARGET};
    DrawRectangleRec(mouse_rect, Fade(GREEN, 0.35f));
    DrawRectangleLinesEx(mouse_rect, 2, Fade(GREEN, 0.8f));
  } else {
    draw_source_tilemap();
    // Vector2 cartesian_mouse = screen_to_cartesian(mouse_position);
    // Vector2 offset_mouse = {cartesian_mouse.x, cartesian_mouse.y};
    // Vector2 final_mouse_position = cartesian_to_screen(offset_mouse);
    float final_size_target = TILE_SIZE_TARGET * scale_factor;
    int denom = ( ((int)mouse_position.x % (int)(12)));
    Rectangle mouse_rect = {
      floorf(mouse_position.x - denom),
      floorf(mouse_position.y - denom),
      final_size_target,
      final_size_target,
    };
    DrawRectangleRec(mouse_rect, Fade(BLUE, 0.35f));
    DrawRectangleLinesEx(mouse_rect, 2, Fade(BLUE, 0.8f));
    // printf("POS:: X: %.2f, Y: %.2f\n", mouse_position.x, mouse_position.y);
    // printf("REC:: X: %.2f, Y: %.2f\n", mouse_rect.x, mouse_rect.y);

    for(int row = 0; row < 10; row++) {
      for(int col = 0; col < 12; col++) {
        float x = col * TILE_SIZE_TARGET * scale_factor;
        DrawLineV((Vector2){x, 0}, (Vector2){x, WINDOW_HEIGHT}, MAGENTA);
      }
      float y = row * TILE_SIZE_TARGET * scale_factor;
      DrawLineV((Vector2){0, y}, (Vector2){WINDOW_WIDTH, y}, MAGENTA);
    }
  }
}