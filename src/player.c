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
int bullet_insertion_index = 0;

void init_player() {
  player.position = (Vector2){0, -(HALF_WINDOW_HEIGHT / 2)};
}

void input_player(float dt) {
  int player_speed = 450;
  if(IsKeyDown(KEY_W)) player.position.y += player_speed * dt;
  if(IsKeyDown(KEY_A)) player.position.x -= player_speed * dt;
  if(IsKeyDown(KEY_S)) player.position.y -= player_speed * dt;
  if(IsKeyDown(KEY_D)) player.position.x += player_speed * dt;

  if(IsKeyPressed(KEY_J)) {
    bullets[bullet_insertion_index].position.x = player.position.x;
    bullets[bullet_insertion_index].position.y = player.position.y;
    bullet_insertion_index = (bullet_insertion_index + 1) % TOTAL_BULLETS;
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
  Vector2 atlas_coord = {1, 2};
  render_ship(atlas_coord, player.position);
}

void render_bullets() {
  Vector2 atlas_coord = {0, 0};
  for(int i = 0; i < TOTAL_BULLETS; i++) {
    render_tile(atlas_coord, bullets[i].position);
  }
}