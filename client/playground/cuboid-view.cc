#include "cuboid-view.h"

#include <zigen-opengl-client-protocol.h>

#include <cstring>
#include <glm/gtx/quaternion.hpp>

#include "cuboid-frag.h"
#include "cuboid-vert.h"

struct Vertex {
  glm::vec3 p;
};

static const u_short frame_indices[24] = {
    0, 1, 2, 3, 4, 5, 6, 7, 0, 4, 1, 5, 2, 6, 3, 7, 0, 2, 1, 3, 4, 6, 5, 7};

namespace zigen_playground {

CuboidView::CuboidView(std::shared_ptr<zukou::Application> app,
    std::shared_ptr<zukou::VirtualObject> virtual_object,
    std::shared_ptr<model::Cuboid> cuboid)
    : app_(app), virtual_object_(virtual_object), cuboid_(cuboid) {}

bool CuboidView::Init() {
  component_.reset(new zukou::OpenGLComponent(app_, virtual_object_));

  // shader
  shader_ = zukou::OpenGLShaderProgram::Create(app_);
  shader_->SetVertexShader(cuboid_vert_shader);
  shader_->SetFragmentShader(cuboid_frag_shader);
  shader_->Link();
  shader_->SetUniformVariable("color", glm::vec4(0.0f, 1.0f, 1.0f, 1.0f));
  component_->Attach(shader_);

  // element array buffer
  element_array_buffer_ =
      zukou::OpenGLElementArrayBuffer::Create(app_, sizeof(frame_indices),
          ZGN_OPENGL_ELEMENT_ARRAY_INDICES_TYPE_UNSIGNED_SHORT);
  {
    ushort *indices = element_array_buffer_->GetData<ushort>();
    memcpy(indices, frame_indices, sizeof(frame_indices));
  }
  component_->Attach(element_array_buffer_);

  // vertex buffer
  vertex_buffer_ = zukou::OpenGLVertexBuffer::Create(app_, sizeof(Vertex) * 8);
  {
    Vertex *vertices = vertex_buffer_->GetData<Vertex>();
    int i = 0;
    for (int x = -1; x < 2; x += 2) {
      for (int y = -1; y < 2; y += 2) {
        for (int z = -1; z < 2; z += 2) {
          vertices[i].p.x = x;
          vertices[i].p.y = y;
          vertices[i].p.z = z;
          i++;
        }
      }
    }
  }
  component_->Attach(vertex_buffer_);

  // component
  component_->SetCount(24);
  component_->SetTopology(ZGN_OPENGL_TOPOLOGY_LINES);
  component_->AddVertexAttribute(
      0, 3, ZGN_OPENGL_VERTEX_ATTRIBUTE_TYPE_FLOAT, false, sizeof(Vertex), 0);

  return true;
}

bool CuboidView::Draw() { return true; }

float CuboidView::Intersect(glm::vec3 origin, glm::vec3 direction) {
  auto transform = GetTransformMatrix();
  return zukou::entities::intersection::ray_obb(
      origin, direction, cuboid_->half_size, transform);
}

void CuboidView::RayEnter() {}

void CuboidView::RayLeave() {}

void CuboidView::RayMotion(
    glm::vec3 origin, glm::vec3 direction, uint32_t time) {
  (void)origin;
  (void)direction;
  (void)time;
}

void CuboidView::RayButton(
    uint32_t serial, uint32_t time, uint32_t button, bool pressed) {
  (void)serial;
  (void)time;
  (void)button;
  (void)pressed;
}

void CuboidView::DataDeviceEnter(
    uint32_t serial, std::weak_ptr<zukou::DataOffer> data_offer) {
  data_offer_ = data_offer;
  data_device_enter_serial_ = serial;
}

void CuboidView::DataDeviceLeave() { data_offer_.reset(); }

void CuboidView::DataDeviceMotion([[maybe_unused]] uint32_t time,
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

void CuboidView::DataDeviceDrop() {
  if (auto data_offer = data_offer_.lock()) {
    for (auto mime_type : data_offer->mime_types()) {
      if (mime_type == "text/plain") {
        data_offer->Receive(mime_type,
            std::bind(&CuboidView::TextDropped, this, std::placeholders::_1));
        break;
      }
    }
  }
}

void CuboidView::TextDropped(int fd) {
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

void CuboidView::SetGeometry(
    glm::vec3 parent_position, glm::quat parent_quaternion) {
  parent_position_ = parent_position;
  parent_quaternion_ = parent_quaternion;
  auto transform = GetTransformMatrix();
  shader_->SetUniformVariable(
      "transform", glm::scale(transform, cuboid_->half_size));
  component_->Attach(shader_);
  virtual_object_->ScheduleNextFrame();
}

glm::mat4 CuboidView::GetTransformMatrix() {
  glm::mat4 transform(1);
  transform = glm::translate(transform, parent_position_);
  transform = transform * glm::toMat4(parent_quaternion_);
  transform = glm::translate(transform, cuboid_->position);
  return transform;
}

}  // namespace zigen_playground
