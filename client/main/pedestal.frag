#version 410 core
uniform vec4 color;
out vec4 outputColor;
in vec4 frontColor;

void main()
{
  outputColor = vec4(frontColor.xyz, 1.0);
  // outputColor = color;
}
