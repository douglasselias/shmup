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

Shader anti_alias_shader;
void init_anti_alias() {
  anti_alias_shader = LoadShaderFromMemory(vs, fs);
}

void begin_anti_alias() {
  BeginShaderMode(anti_alias_shader);
  BeginBlendMode(BLEND_ALPHA_PREMULTIPLY);
}

void end_anti_alias() {
  EndBlendMode();
  EndShaderMode();
}