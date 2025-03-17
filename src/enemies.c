typedef struct {
  Vector2 position;
  float bullet_spawn_time;
  bool alive;
} Enemy;

#define TOTAL_ENEMIES 20
Enemy enemies[TOTAL_ENEMIES];
float spawn_time = 0;

#define TOTAL_ENEMY_BULLETS 60
Bullet enemy_bullets[TOTAL_ENEMY_BULLETS];
int enemy_bullet_insertion_index = 0;

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

  for(int i = 0; i < TOTAL_ENEMY_BULLETS; i++) {
    enemy_bullets[i].position.y = HALF_WINDOW_HEIGHT + 100;
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

  for(int i = 0; i < TOTAL_ENEMIES; i++) {
    Enemy enemy = enemies[i];
    if(enemy.alive && enemy.bullet_spawn_time > 1) {
      Vector2 direction;
      direction.x = player.position.x - enemy.position.x;
      direction.y = player.position.y - enemy.position.y;
      enemy_bullets[enemy_bullet_insertion_index].position.x = enemy.position.x + direction.x;
      enemy_bullets[enemy_bullet_insertion_index].position.y = enemy.position.y + direction.y;
      enemy_bullet_insertion_index = (enemy_bullet_insertion_index + 1) % TOTAL_ENEMY_BULLETS;
      enemies[i].bullet_spawn_time = 0;
    }
    enemies[i].bullet_spawn_time += dt;
  }

  for(int i = 0; i < TOTAL_ENEMY_BULLETS; i++) {
    float bullet_speed = 60;
    enemy_bullets[i].position.x += bullet_speed * dt;
    enemy_bullets[i].position.y -= bullet_speed * dt;
  }
}

void render_enemies() {
  for(int i = 0; i < TOTAL_ENEMIES; i++) {
    Vector2 atlas_coord = {0, 3};
    render_ship(atlas_coord, enemies[i].position);
  }
}

void render_enemy_bullets() {
  for(int i = 0; i < TOTAL_ENEMY_BULLETS; i++) {
    Vector2 atlas_coord = {0, 1};
    render_tile(atlas_coord, enemy_bullets[i].position);
  }
}