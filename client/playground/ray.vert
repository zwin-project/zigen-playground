#version 410

uniform mat4 zMVP;
uniform mat4 transform;
layout(location = 0) in vec4 position;

void main() { gl_Position = zMVP * transform * position; }
