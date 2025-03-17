#define ATLAS_SHIP_SIZE 32
#define SHIP_SIZE_TARGET (ATLAS_SHIP_SIZE * 2)
Texture2D ships_atlas_texture;

void init_ships() {
  ships_atlas_texture = LoadTexture("../assets/ships_packed.png");
}

void render_ship(Vector2 atlas_coord, Vector2 cartesian_position) {
  Rectangle source = {atlas_coord.x * ATLAS_SHIP_SIZE, atlas_coord.y * ATLAS_SHIP_SIZE, ATLAS_SHIP_SIZE, ATLAS_SHIP_SIZE};
  Vector2 position = cartesian_to_screen(cartesian_position);
  Rectangle dest = {position.x, position.y, SHIP_SIZE_TARGET, SHIP_SIZE_TARGET};
  Vector2 origin = {SHIP_SIZE_TARGET/2, SHIP_SIZE_TARGET/2};
  float rotation = 0;
  Color color = WHITE;
  DrawTexturePro(ships_atlas_texture, source, dest, origin, rotation, color);
}