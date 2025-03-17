#define ATLAS_SHIP_SIZE 32
#define SHIP_SIZE_TARGET (ATLAS_SHIP_SIZE * 2)
Texture2D ships_atlas_texture;

typedef struct {
  Vector2 position;
} Player;

typedef struct {
  Vector2 position;
  Vector2 atlas_coord;
} Bullet;

Player player;
#define TOTAL_BULLETS 20
Bullet bullets[TOTAL_BULLETS];
int bullet_index = 0;

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

  if(IsKeyPressed(KEY_J)) {
    bullets[bullet_index].position.x = player.position.x;
    bullets[bullet_index].position.y = player.position.y;
    bullet_index = (bullet_index + 1) % TOTAL_BULLETS;
  }

  player.position.x = Clamp(player.position.x, -HALF_WINDOW_WIDTH  + (SHIP_SIZE_TARGET/2), HALF_WINDOW_WIDTH  - (SHIP_SIZE_TARGET/2));
  player.position.y = Clamp(player.position.y, -HALF_WINDOW_HEIGHT + (SHIP_SIZE_TARGET/2), HALF_WINDOW_HEIGHT - (SHIP_SIZE_TARGET/2));
}

void update_bullets(float dt) {
  float bullet_speed = 900;
  for(int i = 0; i < TOTAL_BULLETS; i++) {
    bullets[i].position.y += bullet_speed * dt;
  }
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

void render_bullets() {
  Vector2 atlas_coord = {0, 0};
  for(int i = 0; i < TOTAL_BULLETS; i++) {
    render_tile(atlas_coord, bullets[i].position);
  }
}