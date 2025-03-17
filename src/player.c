#define ATLAS_SHIP_SIZE 32
#define SHIP_SIZE_TARGET (ATLAS_SHIP_SIZE * 2)
Texture2D ships_atlas_texture;

typedef struct {
  Vector2 position;
} Player;

Player player = {0};

void init_player() {
  ships_atlas_texture = LoadTexture("../assets/ships_packed.png");
  player.position = (Vector2){0, -(HALF_WINDOW_HEIGHT / 2)};
}

void input_player(float dt) {
  int player_speed = 450;
  if(IsKeyDown(KEY_W)) player.position.y += player_speed * dt;
  if(IsKeyDown(KEY_A)) player.position.x -= player_speed * dt;
  if(IsKeyDown(KEY_S)) player.position.y -= player_speed * dt;
  if(IsKeyDown(KEY_D)) player.position.x += player_speed * dt;

  player.position.x = Clamp(player.position.x, -HALF_WINDOW_WIDTH  + (SHIP_SIZE_TARGET/2), HALF_WINDOW_WIDTH  - (SHIP_SIZE_TARGET/2));
  player.position.y = Clamp(player.position.y, -HALF_WINDOW_HEIGHT + (SHIP_SIZE_TARGET/2), HALF_WINDOW_HEIGHT - (SHIP_SIZE_TARGET/2));
}

void render_player() {
  int x = 1, y = 2;
  Rectangle source = {(float)x * ATLAS_SHIP_SIZE, (float)y * ATLAS_SHIP_SIZE, ATLAS_SHIP_SIZE, ATLAS_SHIP_SIZE};
  Vector2 position = cartesian_to_screen(player.position);
  Rectangle dest = {position.x, position.y, SHIP_SIZE_TARGET, SHIP_SIZE_TARGET};
  Vector2 origin = {SHIP_SIZE_TARGET/2, SHIP_SIZE_TARGET/2};
  float rotation = 0;
  Color color = WHITE;
  DrawTexturePro(ships_atlas_texture, source, dest, origin, rotation, color);
}