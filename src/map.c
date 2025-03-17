Texture2D map_texture;
float map_scale = 1.88f;
float scaled_map_height;
float map_y1;
float map_y2;

void init_map() {
  map_texture = LoadTexture("../assets/map.png");
  SetTextureFilter(map_texture, TEXTURE_FILTER_BILINEAR);
  scaled_map_height = map_texture.height * map_scale;
  map_y1 = WINDOW_HEIGHT - scaled_map_height;
  map_y2 = map_y1 - scaled_map_height;
}

void update_map(float dt) {
  float map_speed = 50;
  map_y1 += map_speed * dt;
  map_y2 += map_speed * dt;

  if(map_y1 >= (WINDOW_HEIGHT + scaled_map_height - (scaled_map_height - WINDOW_HEIGHT))) {
    map_y1 = -scaled_map_height;
  }

  if(map_y2 >= (WINDOW_HEIGHT + scaled_map_height - (scaled_map_height - WINDOW_HEIGHT))) {
    map_y2 = -scaled_map_height;
  }
}

void render_map() {
  DrawTextureEx(map_texture, (Vector2){0, map_y1}, 0, map_scale, WHITE);
  DrawTextureEx(map_texture, (Vector2){0, map_y2}, 0, map_scale, WHITE);
}