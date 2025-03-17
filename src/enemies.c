typedef struct {
  Vector2 position;
  bool alive;
} Enemy;

#define TOTAL_ENEMIES 20
Enemy enemies[TOTAL_ENEMIES];

void update_enemies(float dt) {
  for(int i = 0; i < TOTAL_ENEMIES; i++) {
    if(!enemies[i].alive) {
      int x = GetRandomValue(-HALF_WINDOW_WIDTH + 10, HALF_WINDOW_WIDTH - 10);
      int y = GetRandomValue(HALF_WINDOW_HEIGHT, HALF_WINDOW_HEIGHT*2);
      enemies[i].position.x = (float)x;
      enemies[i].position.y = (float)y;
      enemies[i].alive = true;
    } else {
      float enemy_speed = 100;
      // enemies[i].position.x += enemy_speed * dt;
      enemies[i].position.y -= enemy_speed * dt;
    }
  }

}

void render_enemies() {
  for(int i = 0; i < TOTAL_ENEMIES; i++) {
    Vector2 atlas_coord = {0, 3};
    render_ship(atlas_coord, enemies[i].position);
  }
}