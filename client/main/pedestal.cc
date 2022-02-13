#include "pedestal.h"

#include <linux/input.h>
#include <zigen-opengl-client-protocol.h>

#include <glm/gtx/intersect.hpp>
#include <glm/gtx/quaternion.hpp>
#include <iostream>

#include "pedestal-frag.h"
#include "pedestal-vert.h"

struct Vertex {
  glm::vec3 p;
  glm::vec3 n;
  glm::vec2 uv;
};

std::shared_ptr<Pedestal> Pedestal::Create(
    std::shared_ptr<zukou::Application> app,
    std::shared_ptr<zukou::CuboidWindow> cuboid_window) {
  return std::shared_ptr<Pedestal>(new Pedestal(app, cuboid_window));
}

bool Pedestal::Init() {
  component_ = std::make_shared<zukou::OpenGLComponent>(app_, cuboid_window_);
  uint32_t element_count;

  // shader
  shader_ = zukou::OpenGLShaderProgram::Create(app_);
  shader_->SetVertexShader(pedestal_vert_shader);
  shader_->SetFragmentShader(pedestal_frag_shader);
  shader_->Link();
  shader_->SetUniformVariable("color", glm::vec4(1.0, 0.0, 1.0, 1.0));
  component_->Attach(shader_);

  // element array buffer
  element_array_buffer_ =
      CreateElementArrayBuffer(resolution_, threshold_, &element_count);
  component_->Attach(element_array_buffer_);

  // vertex buffer
  vertex_buffer_ = CreateVertexBuffer(resolution_, threshold_);
  component_->Attach(vertex_buffer_);

  // component
  component_->SetCount(element_count);
  component_->SetTopology(ZGN_OPENGL_TOPOLOGY_TRIANGLES);
  component_->AddVertexAttribute(
      0, 3, ZGN_OPENGL_VERTEX_ATTRIBUTE_TYPE_FLOAT, false, sizeof(Vertex), 0);
  component_->AddVertexAttribute(1, 3, ZGN_OPENGL_VERTEX_ATTRIBUTE_TYPE_FLOAT,
      false, sizeof(Vertex), offsetof(Vertex, n));
  component_->AddVertexAttribute(2, 2, ZGN_OPENGL_VERTEX_ATTRIBUTE_TYPE_FLOAT,
      false, sizeof(Vertex), offsetof(Vertex, uv));

  needs_redraw_ = true;

  return true;
}

bool Pedestal::Draw() {
  if (needs_redraw_ == false) return false;
  glm::mat4 transform(1);
  transform = glm::translate(transform, position_);
  transform = transform * glm::toMat4(quaternion_);
  transform = glm::scale(transform, glm::vec3(radius_));
  shader_->SetUniformVariable("transform", transform);
  component_->Attach(shader_);
  needs_redraw_ = false;
  return true;
}

float Pedestal::Intersect(glm::vec3 origin, glm::vec3 direction) {
  glm::vec3 intersect_point, norm;
  glm::mat4 transform(1);

  transform = glm::translate(transform, position_);
  transform = transform * glm::toMat4(quaternion_);
  glm::vec4 global_position = transform * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

  if (glm::intersectRaySphere(origin, direction, glm::vec3(global_position),
          radius_, intersect_point, norm) == false)
    return -1;

  float y = intersect_point[1];
  float threshold_y =
      cos(M_PI_2 / (float)resolution_ * (float)threshold_) * radius_;

  if ((-radius_ < y && y < -threshold_y) || (threshold_y < y && y < radius_)) {
    ray_distance_ = glm::length(intersect_point - origin);
  } else {
    float len = direction[1] > 0 ? (threshold_y - origin[1]) / direction[1]
                                 : (-threshold_y - origin[1]) / direction[1];
    float x = origin[0] + direction[0] * len;
    float z = origin[2] + direction[2] * len;

    if (x * x + z * z < radius_ * radius_ - threshold_y * threshold_y) {
      ray_distance_ = len;
    } else {
      return -1;
    }
  }

  return ray_distance_;
}

void Pedestal::RayEnter() {
  shader_->SetUniformVariable("color", glm::vec4(1, 1, 0, 1));
  component_->Attach(shader_);
  cuboid_window_->ScheduleNextFrame();
}

void Pedestal::RayLeave() {
  shader_->SetUniformVariable("color", glm::vec4(1, 0, 1, 1));
  component_->Attach(shader_);
  cuboid_window_->ScheduleNextFrame();
}

void Pedestal::RayMotion(glm::vec3 origin, glm::vec3 direction, uint32_t time) {
  (void)origin;
  (void)direction;
  (void)time;
  app_->SetRayLength(ray_distance_);
}

void Pedestal::RayButton(
    uint32_t serial, uint32_t time, uint32_t button, bool pressed) {
  (void)time;
  if (button == BTN_LEFT && pressed) {
    cuboid_window_->Move(serial);
  }
}

void Pedestal::RayAxis(uint32_t time, uint32_t axis, float value) {
  (void)time;
  (void)axis;
  (void)value;
}

void Pedestal::RayFrame() {}

void Pedestal::RayAxisDiscrete(uint32_t axis, int32_t discrete) {
  (void)axis;
  (void)discrete;
}

void Pedestal::DataDeviceEnter(
    uint32_t serial, std::weak_ptr<zukou::DataOffer> data_offer) {
  (void)serial;
  (void)data_offer;
}

void Pedestal::DataDeviceLeave() {}

void Pedestal::DataDeviceMotion(
    uint32_t time, glm::vec3 origin, glm::vec3 direction) {
  (void)time;
  (void)origin;
  (void)direction;
}

void Pedestal::DataDeviceDrop() {}

void Pedestal::SetGeometry(float r, glm::vec3 position, glm::quat quaternion) {
  radius_ = r;
  position_ = position;
  quaternion_ = quaternion;
  needs_redraw_ = true;
}

Pedestal::Pedestal(std::shared_ptr<zukou::Application> app,
    std::shared_ptr<zukou::CuboidWindow> cuboid_window)
    : app_(app),
      cuboid_window_(cuboid_window),
      resolution_(20),
      threshold_(7) {}

std::shared_ptr<zukou::OpenGLElementArrayBuffer>
Pedestal::CreateElementArrayBuffer(
    uint32_t resolution, uint32_t threshold, uint32_t *count) {
  uint32_t poll_element_count = 3 * 4 * resolution * 2;
  uint32_t other_element_count = (4 * resolution) * 2 * threshold * 6;
  *count = poll_element_count + other_element_count;

  auto element_array_buffer =
      zukou::OpenGLElementArrayBuffer::Create(app_, sizeof(ushort) * *count,
          ZGN_OPENGL_ELEMENT_ARRAY_INDICES_TYPE_UNSIGNED_SHORT);

  int i = 0;
  ushort *indices = element_array_buffer->GetData<ushort>();
  uint32_t last_index = (4 * resolution + 1) * threshold * 2 + 1;

  // clang-format off
  // top
  for (uint32_t j = 0; j < (4 * resolution); j++) {
    indices[i] = 0; i++;
    indices[i] = j + 1; i++;
    indices[i] = j + 2; i++;
  }

  for (uint32_t j = 0; j < 2 * threshold - 1; j++) {
    if (j == threshold -1) continue;
    for (uint32_t k = 0; k < 4 * resolution; k++) {
      indices[i] = (4 * resolution + 1) * j + k + 1; i++;
      indices[i] = (4 * resolution + 1) * j + k + 2; i++;
      indices[i] = (4 * resolution + 1) * (j + 1) + k + 1; i++;

      indices[i] = (4 * resolution + 1) * (j + 1) + k + 1; i++;
      indices[i] = (4 * resolution + 1) * (j + 1) + k + 2; i++;
      indices[i] = (4 * resolution + 1) * j + k + 2; i++;
    }
  }

  for (uint32_t j = 0; j < 4 * resolution; j++) {
    indices[i] = last_index; i++;
    indices[i] = last_index - j - 1; i++;
    indices[i] = last_index - j - 2; i++;
  }
  // clang-format on

  return element_array_buffer;
}

std::shared_ptr<zukou::OpenGLVertexBuffer> Pedestal::CreateVertexBuffer(
    uint32_t resolution, uint32_t threshold) {
  uint32_t vertex_count = (4 * resolution + 1) * threshold * 2 + 2;
  auto vertex_buffer =
      zukou::OpenGLVertexBuffer::Create(app_, sizeof(Vertex) * vertex_count);

  Vertex *vertices = vertex_buffer->GetData<Vertex>();

  int i = 0;
  float delta = M_PI_2 / resolution;

  // top
  vertices[i].p = glm::vec3(0, 1, 0);
  vertices[i].n = vertices[i].p;
  vertices[i].uv = glm::vec2(0.5, 0);
  i++;

  // around top
  for (int j = resolution - 1; j > (int)resolution - 1 - (int)threshold; j--) {
    float theta = delta * j;
    float y = sin(theta);
    float cos_theta = cos(theta);
    float v = (float)(-j + resolution) / (float)(2 * resolution);
    for (int k = 0; k < (int)(4 * resolution + 1); k++) {
      float phi = delta * k;
      float x = -cos_theta * sin(phi);
      float z = -cos_theta * cos(phi);
      float u = (float)k / (float)(2 * resolution);
      vertices[i].p = glm::vec3(x, y, z);
      vertices[i].n = vertices[i].p;
      vertices[i].uv = glm::vec2(u, v);
      i++;
    }
  }

  // around bottom
  for (int j = threshold - (int)resolution; j > -(int)resolution; j--) {
    float theta = delta * j;
    float y = sin(theta);
    float cos_theta = cos(theta);
    float v = (float)(-j + resolution) / (float)(2 * resolution);
    for (int k = 0; k < (int)(4 * resolution + 1); k++) {
      float phi = delta * k;
      float x = -cos_theta * sin(phi);
      float z = -cos_theta * cos(phi);
      float u = (float)k / (float)(2 * resolution);
      vertices[i].p = glm::vec3(x, y, z);
      vertices[i].n = vertices[i].p;
      vertices[i].uv = glm::vec2(u, v);
      i++;
    }
  }

  // bottom
  vertices[i].p = glm::vec3(0, -1, 0);
  vertices[i].n = vertices[i].p;
  vertices[i].uv = glm::vec2(0.5, 1);

  return vertex_buffer;
}
