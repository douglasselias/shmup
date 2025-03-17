#define ATLAS_TILE_SIZE 16
#define TILE_SIZE_TARGET (ATLAS_TILE_SIZE * 2)

Texture2D tiles_atlas_texture;

void init_tiles() {
  tiles_atlas_texture = LoadTexture("../assets/tiles_packed.png");
}

void render_tile(Vector2 atlas_coord, Vector2 cartesian_position) {
  Rectangle source = {atlas_coord.x * ATLAS_TILE_SIZE, atlas_coord.y * ATLAS_TILE_SIZE, ATLAS_TILE_SIZE, ATLAS_TILE_SIZE};
  Vector2 position = cartesian_to_screen(cartesian_position);
  Rectangle dest = {position.x, position.y, TILE_SIZE_TARGET, TILE_SIZE_TARGET};
  Vector2 origin = {TILE_SIZE_TARGET/2, TILE_SIZE_TARGET/2};
  float rotation = 0;
  Color color = WHITE;
  DrawTexturePro(tiles_atlas_texture, source, dest, origin, rotation, color);
}