#version 410

uniform mat4 zMVP;
uniform mat4 transform;
layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uvIn;
out vec2 uv;

void main() {
  uv = uvIn;
  gl_Position = zMVP * transform * position;
}
