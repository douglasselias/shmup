typedef struct {
  Vector2 position;
  bool alive;
} Enemy;

#define TOTAL_ENEMIES 20
Enemy enemies[TOTAL_ENEMIES];
float spawn_time = 0;

void spawn_enemy(Enemy* enemy) {
  int x = GetRandomValue(-HALF_WINDOW_WIDTH + 10, HALF_WINDOW_WIDTH - 10);
  int y = GetRandomValue(HALF_WINDOW_HEIGHT, HALF_WINDOW_HEIGHT * 2);
  enemy->position.x = (float)x;
  enemy->position.y = (float)y;
  enemy->alive = true;
}

void init_enemies() {
  for(int i = 0; i < TOTAL_ENEMIES; i++) {
    spawn_enemy(&enemies[i]);
  }
}

void update_enemies(float dt) {
  for(int i = 0; i < TOTAL_ENEMIES; i++) {
    if(enemies[i].alive) {
      float enemy_speed = 100;
      enemies[i].position.y -= enemy_speed * dt;
      if(enemies[i].position.y <= -HALF_WINDOW_HEIGHT - 20) {
        enemies[i].alive = false;
      }
    } else if(spawn_time > 1) {
      spawn_enemy(&enemies[i]);
      spawn_time = 0;
    }
  }

  spawn_time += dt;
}

void render_enemies() {
  for(int i = 0; i < TOTAL_ENEMIES; i++) {
    Vector2 atlas_coord = {0, 3};
    render_ship(atlas_coord, enemies[i].position);
  }
}