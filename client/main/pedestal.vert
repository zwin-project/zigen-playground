#version 410

struct ZLight {
  vec4 position;
  vec4 diffuse;
  vec4 ambiend;
  vec4 specular;
};

uniform mat4 zModel;
uniform mat4 zVP;
uniform ZLight zLight;
uniform mat4 transform;
layout(location = 0) in vec4 localPosition;
layout(location = 1) in vec3 norm;
layout(location = 2) in vec2 uvIn;
out vec4 frontColor;

void main() {
  vec4 position = zModel * localPosition;
  vec3 view = -normalize(position.xyz);
  vec3 light = normalize(
      (zLight.position * position.w - zLight.position.w * position).xyz);
  vec3 halfway = normalize(light + view);
  float diffuse = abs(dot(light, norm));
  float specular = abs(dot(norm, halfway));
  frontColor = zLight.diffuse * 0.5 * diffuse +
               zLight.specular * 0.5 * specular + zLight.ambiend * 10000;
  gl_Position = zVP * position;
}
