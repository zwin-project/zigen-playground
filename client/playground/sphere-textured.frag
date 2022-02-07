#version 410 core
out vec4 outputColor;
uniform sampler2D userTexture;
in vec2 uv;

void main()
{
  outputColor = texture(userTexture, uv);
}
