#include "sphere-view.h"

#include <zigen-opengl-client-protocol.h>

#include <functional>
#include <glm/gtx/quaternion.hpp>
#include <iostream>

#include "image-downloader.h"
#include "sphere-frag.h"
#include "sphere-textured-frag.h"
#include "sphere-vert.h"

struct Vertex {
  glm::vec3 p;
  glm::vec3 n;
  glm::vec2 uv;
};

namespace zigen_playground {

SphereView::SphereView(std::shared_ptr<zukou::Application> app,
    std::shared_ptr<zukou::VirtualObject> virtual_object,
    std::shared_ptr<model::Sphere> sphere)
    : app_(app), virtual_object_(virtual_object), sphere_(sphere) {}

bool SphereView::Init() {
  component_.reset(new zukou::OpenGLComponent(app_, virtual_object_));
  uint32_t element_count;

  // shader
  shader_ = zukou::OpenGLShaderProgram::Create(app_);
  shader_->SetVertexShader(sphere_vert_shader);
  shader_->SetFragmentShader(sphere_frag_shader);
  shader_->Link();
  shader_->SetUniformVariable("color", glm::vec4(0.0f, 1.0f, 1.0f, 1.0f));
  component_->Attach(shader_);

  // element array buffer
  element_array_buffer_ =
      CreateFrameElementArrayBuffer(sphere_->resolution, &element_count);
  component_->Attach(element_array_buffer_);

  // vertex buffer
  vertex_buffer_ = CreateVertexBuffer(sphere_->resolution);
  component_->Attach(vertex_buffer_);

  component_->SetCount(element_count);
  component_->SetTopology(ZGN_OPENGL_TOPOLOGY_LINES);
  component_->AddVertexAttribute(
      0, 3, ZGN_OPENGL_VERTEX_ATTRIBUTE_TYPE_FLOAT, false, sizeof(Vertex), 0);
  component_->AddVertexAttribute(1, 3, ZGN_OPENGL_VERTEX_ATTRIBUTE_TYPE_FLOAT,
      false, sizeof(Vertex), offsetof(Vertex, n));
  component_->AddVertexAttribute(2, 2, ZGN_OPENGL_VERTEX_ATTRIBUTE_TYPE_FLOAT,
      false, sizeof(Vertex), offsetof(Vertex, uv));

  if (sphere_->texture != "")
    app_->AddPollEvent(std::make_shared<ImageDownloader>(
        sphere_->texture, std::bind(&SphereView::OnTextureLoaded, this,
                              std::placeholders::_1, std::placeholders::_2,
                              std::placeholders::_3, std::placeholders::_4)));

  return true;
}

bool SphereView::Draw() { return true; }

float SphereView::Intersect(glm::vec3 origin, glm::vec3 direction) {
  (void)origin;
  (void)direction;
  return -1;
}

void SphereView::RayEnter() {}

void SphereView::RayLeave() {}

void SphereView::RayMotion(
    glm::vec3 origin, glm::vec3 direction, uint32_t time) {
  (void)origin;
  (void)direction;
  (void)time;
}

void SphereView::RayButton(
    uint32_t serial, uint32_t time, uint32_t button, bool pressed) {
  (void)serial;
  (void)time;
  (void)button;
  (void)pressed;
}

void SphereView::DataDeviceEnter(
    uint32_t serial, std::weak_ptr<zukou::DataOffer> data_offer) {
  data_offer_ = data_offer;
  data_device_enter_serial_ = serial;
}

void SphereView::DataDeviceLeave() { data_offer_.reset(); }

void SphereView::DataDeviceMotion([[maybe_unused]] uint32_t time,
    [[maybe_unused]] glm::vec3 origin, [[maybe_unused]] glm::vec3 direction) {
  if (auto data_offer = data_offer_.lock()) {
    for (auto mime_type : data_offer->mime_types()) {
      if (mime_type == "text/plain") {
        data_offer->Accept(data_device_enter_serial_, mime_type);
        break;
      }
    }
  }
}

void SphereView::DataDeviceDrop() {
  if (auto data_offer = data_offer_.lock()) {
    for (auto mime_type : data_offer->mime_types()) {
      if (mime_type == "text/plain") {
        data_offer->Receive(mime_type,
            std::bind(&SphereView::TextDropped, this, std::placeholders::_1));
        break;
      }
    }
  }
}

void SphereView::TextDropped(int fd) {
  char buf[100];
  std::string str;
  while (true) {
    int size = read(fd, buf, 100);
    if (size <= 0) break;
    std::string fragment(buf, size);
    str += fragment;
  }

  (void)str;
}

void SphereView::SetGeometry(
    glm::vec3 parent_position, glm::quat parent_quaternion) {
  parent_position_ = parent_position;
  parent_quaternion_ = parent_quaternion;
  auto transform = GetTransformMatrix();
  shader_->SetUniformVariable(
      "transform", glm::scale(transform, glm::vec3(sphere_->r)));
  component_->Attach(shader_);
  virtual_object_->ScheduleNextFrame();
}

glm::mat4 SphereView::GetTransformMatrix() {
  glm::mat4 transform(1);
  transform = glm::translate(transform, parent_position_);
  transform = transform * glm::toMat4(parent_quaternion_);
  transform = glm::translate(transform, sphere_->position);
  return transform;
}

void SphereView::OnTextureLoaded(uint32_t width, uint32_t height,
    [[maybe_unused]] uint32_t pixel_size,
    std::function<void(void *data)> writer) {
  uint32_t element_count;

  // shader
  auto transform = GetTransformMatrix();
  shader_ = zukou::OpenGLShaderProgram::Create(app_);
  shader_->SetVertexShader(sphere_vert_shader);
  shader_->SetFragmentShader(sphere_textured_frag_shader);
  shader_->Link();
  shader_->SetUniformVariable(
      "transform", glm::scale(transform, glm::vec3(sphere_->r)));
  component_->Attach(shader_);

  // texture;
  texture_ = zukou::OpenGLTexture::Create(app_, width, height);
  auto data = texture_->GetData();
  writer(data);
  component_->Attach(texture_);

  // element array buffer
  element_array_buffer_ =
      CreateTextureElementArrayBuffer(sphere_->resolution, &element_count);
  component_->Attach(element_array_buffer_);

  // component
  component_->SetTopology(ZGN_OPENGL_TOPOLOGY_TRIANGLES);
  component_->SetCount(element_count);

  virtual_object_->ScheduleNextFrame();
}

/**
 * Create uv sphere vertices
 * 4 * resolution lines for longitude
 * (2 * resolution - 1) lines for latitude
 */
std::shared_ptr<zukou::OpenGLVertexBuffer> SphereView::CreateVertexBuffer(
    uint32_t resolution) {
  uint32_t vertex_count = (4 * resolution + 1) * (2 * resolution - 1) + 2;
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

  for (int j = resolution - 1; j > -(int)resolution; j--) {
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

std::shared_ptr<zukou::OpenGLElementArrayBuffer>
SphereView::CreateFrameElementArrayBuffer(
    uint32_t resolution, uint32_t *count) {
  uint32_t longitude_count = (4 * resolution) * (resolution * 2);
  uint32_t latitude_count = (2 * resolution - 1) * (4 * resolution);
  *count = (longitude_count + latitude_count) * 2;
  auto element_array_buffer =
      zukou::OpenGLElementArrayBuffer::Create(app_, sizeof(ushort) * *count,
          ZGN_OPENGL_ELEMENT_ARRAY_INDICES_TYPE_UNSIGNED_SHORT);

  int i = 0;
  ushort *indices = element_array_buffer->GetData<ushort>();
  uint32_t last_index = (4 * resolution + 1) * (2 * resolution - 1) + 1;

  // clang-format off
  // longitude
  for (uint32_t j = 0; j < 4 * resolution; j++) {
    // top
    indices[i] = 0; i++;
    indices[i] = j + 1; i++;

    for (uint32_t k = 0; k < 2 * resolution - 2; k++) {
      indices[i] = indices[i-1]; i++;
      indices[i] = indices[i-1] + (4 * resolution + 1); i++;
    }

    // bottom
    indices[i] = indices[i-1]; i++;
    indices[i] = last_index; i++;
  }

  // latitude
  for (uint32_t j = 0; j < 2 * resolution - 1; j++) {
    for (uint32_t k = 0; k < 4 * resolution; k++) {
      indices[i] = (4 * resolution + 1) * j + k + 1; i++;
      indices[i] = (4 * resolution + 1) * j + k + 2; i++;
    }
  }
  // clang-format on

  return element_array_buffer;
}

std::shared_ptr<zukou::OpenGLElementArrayBuffer>
SphereView::CreateTextureElementArrayBuffer(
    uint32_t resolution, uint32_t *count) {
  uint32_t poll_element_count = 3 * 4 * resolution;
  uint32_t other_element_count = (2 * resolution - 2) * (4 * resolution) * 6;

  *count = poll_element_count + other_element_count;

  auto element_array_buffer =
      zukou::OpenGLElementArrayBuffer::Create(app_, sizeof(ushort) * *count,
          ZGN_OPENGL_ELEMENT_ARRAY_INDICES_TYPE_UNSIGNED_SHORT);

  int i = 0;
  ushort *indices = element_array_buffer->GetData<ushort>();
  uint32_t last_index = (4 * resolution + 1) * (2 * resolution - 1) + 1;

  // clang-format off
  // top
  for (uint32_t j = 0; j < (4 * resolution); j++) {
    indices[i] = 0; i++;
    indices[i] = j + 1; i++;
    indices[i] = j + 2; i++;
  }

  for (uint32_t j = 0; j < (2 * resolution - 2); j++) {
    for (uint32_t k = 0; k < 4 * resolution; k++) {
      indices[i] = (4 * resolution + 1) * j + k + 1; i++;
      indices[i] = (4 * resolution + 1) * j + k + 2; i++;
      indices[i] = (4 * resolution + 1) * (j + 1) + k + 1; i++;

      indices[i] = (4 * resolution + 1) * (j + 1) + k + 1; i++;
      indices[i] = (4 * resolution + 1) * (j + 1) + k + 2; i++;
      indices[i] = (4 * resolution + 1) * j + k + 2; i++;
    }
  }

  // bottom
  for (uint32_t j = 0; j < (4 * resolution); j++) {
    indices[i] = last_index; i++;
    indices[i] = last_index - j - 1; i++;
    indices[i] = last_index - j - 2; i++;
  }
  // clang-format on

  return element_array_buffer;
}

}  // namespace zigen_playground
