#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"

#include <stdio.h>

#include "src/window.c"
#include "src/main_menu.c"
#include "src/player.c"

typedef enum {
  MAIN_SCREEN,
  PLAYING,
  GAME_OVER,
} GameState;

const char* vs =
  "#version 330                                   \n"
  "in vec3 vertexPosition;                        \n"
  "in vec2 vertexTexCoord;                        \n"
  "in vec4 vertexColor;                           \n"
  "out vec2 fragTexCoord;                         \n"
  "out vec4 fragColor;                            \n"
  "uniform mat4 mvp;                              \n"
  "void main()                                    \n"
  "{                                              \n"
  "  fragTexCoord = vertexTexCoord;               \n"
  "  fragColor = vertexColor;                     \n"
  "  gl_Position = mvp*vec4(vertexPosition, 1.0); \n"
  "}                                              \n";
;

const char* fs =
  "#version 330                                                 \n"
  "in vec2 fragTexCoord;                                        \n"
  "in vec4 fragColor;                                           \n"
  "out vec4 finalColor;                                         \n"
  "uniform sampler2D texture0;                                  \n"
  "uniform vec4 colDiffuse;                                     \n"
  "void main()                                                  \n"
  "{                                                            \n"
  "  vec2 uv = fragTexCoord;                                    \n"
  "  vec2 texsize = vec2(textureSize(texture0,0));              \n"
  "  vec2 uv_texspace = uv*texsize;                             \n"
  "  vec2 seam = floor(uv_texspace+.5);                         \n"
  "  uv_texspace = (uv_texspace-seam)/fwidth(uv_texspace)+seam; \n"
  "  uv_texspace = clamp(uv_texspace, seam-.5, seam+.5);        \n"
  "  uv = uv_texspace/texsize;                                  \n"
  "  vec4 texelColor = texture(texture0, uv);                   \n"
  "  finalColor = texelColor*colDiffuse*fragColor;              \n"
  "}                                                            \n";
;

int main() {
  init_window();
  init_player();

  GameState game_state = MAIN_SCREEN;
  Texture2D map = LoadTexture("../assets/map.png");
  SetTextureFilter(map, TEXTURE_FILTER_BILINEAR);
  float map_scale = 1.88f;
  float scaled_map_height = map.height * map_scale;
  float y1 = WINDOW_HEIGHT - scaled_map_height;
  float y2 = y1 - scaled_map_height;
  bool paused = false;

  Shader pixel_art_aa = LoadShaderFromMemory(vs, fs);

  while(!WindowShouldClose()) {
    float dt = GetFrameTime();

    switch(game_state) {
      case MAIN_SCREEN: {
        if(IsKeyPressed(KEY_ENTER)) game_state = PLAYING;
        break;
      }
      case PLAYING: {
        input_player(dt);
        if(IsKeyPressed(KEY_SPACE)) {
          paused = !paused;
        }
        break;
      }
      case GAME_OVER: {
        break;
      }
    }

    if(!paused) {
      float map_speed = 10;
      y1 += map_speed * dt;
      y2 += map_speed * dt;
    }

    if(y1 >= (WINDOW_HEIGHT + scaled_map_height - (scaled_map_height - WINDOW_HEIGHT))) {
      y1 = -scaled_map_height;
    }

    if(y2 >= (WINDOW_HEIGHT + scaled_map_height - (scaled_map_height - WINDOW_HEIGHT))) {
      y2 = -scaled_map_height;
    }

    BeginDrawing();
    ClearBackground(RAYWHITE);

    switch(game_state) {
      case MAIN_SCREEN: {
        draw_main_menu();
        render_player();
        break;
      }
      case PLAYING: {
        render_player();
        break;
      }
      case GAME_OVER: {
        break;
      }
    }

    BeginShaderMode(pixel_art_aa);
    BeginBlendMode(BLEND_ALPHA_PREMULTIPLY);
    DrawTextureEx(map, (Vector2){0, y1}, 0, map_scale, WHITE);
    DrawTextureEx(map, (Vector2){0, y2}, 0, map_scale, WHITE);
    EndBlendMode();
    EndShaderMode();
    EndDrawing();

    update_frame(dt);
  }

  CloseWindow();
  return 0;
}