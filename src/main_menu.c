bool press_start_blink_state = false;
float current_frame = 0;

void draw_main_menu() {
  if(!press_start_blink_state) {
    const char* press_start_text = "Press Start";
    int font_size = 40;
    int press_start_text_width = MeasureText(press_start_text, font_size);
    Vector2 press_start_text_position = {HALF_WINDOW_WIDTH - (press_start_text_width / 2), HALF_WINDOW_HEIGHT};
    DrawText(press_start_text, press_start_text_position.x + 2, press_start_text_position.y + 3, font_size, BLACK);
    DrawText(press_start_text, press_start_text_position.x + 0, press_start_text_position.y + 0, font_size, MAROON);
  }
}

void update_frame(float dt) {
  current_frame += dt;

  if(current_frame >= 0.6) {
    current_frame = 0;
    press_start_blink_state = !press_start_blink_state;
  }
}