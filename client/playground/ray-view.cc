#include "ray-view.h"

#include <zigen-opengl-client-protocol.h>

#include <glm/gtx/quaternion.hpp>

#include "model.h"
#include "ray-frag.h"
#include "ray-vert.h"

struct Vertex {
  glm::vec3 p;
};

static glm::vec4 user_colors[] = {
    glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),
    glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),
};

namespace zigen_playground {
RayView::RayView(std::shared_ptr<zukou::Application> app,
    std::shared_ptr<zukou::VirtualObject> virtual_object,
    std::shared_ptr<model::Ray> ray)
    : app_(app),
      virtual_object_(virtual_object),
      ray_(ray),
      needs_redraw_(false) {}

bool RayView::Init() {
  component_.reset(new zukou::OpenGLComponent(app_, virtual_object_));

  // shader
  shader_ = zukou::OpenGLShaderProgram::Create(app_);
  shader_->SetVertexShader(ray_vert_shader);
  shader_->SetFragmentShader(ray_frag_shader);
  shader_->Link();
  shader_->SetUniformVariable("color", user_colors[ray_->client_id % 2]);
  component_->Attach(shader_);

  // vertex
  vertex_buffer_ = zukou::OpenGLVertexBuffer::Create(app_, sizeof(Vertex) * 2);
  {
    auto *vertices = vertex_buffer_->GetData<Vertex>();
    vertices[0].p = ray_->origin;
    vertices[1].p = ray_->target;
  }
  component_->Attach(vertex_buffer_);

  component_->SetCount(2);
  component_->SetTopology(ZGN_OPENGL_TOPOLOGY_LINES);
  component_->AddVertexAttribute(
      0, 3, ZGN_OPENGL_VERTEX_ATTRIBUTE_TYPE_FLOAT, false, sizeof(Vertex), 0);

  return true;
}

bool RayView::Draw() {
  if (needs_redraw_ == false) return false;

  glm::mat4 transform(1);
  transform = glm::translate(transform, position_);
  transform = transform * glm::toMat4(quaternion_);

  shader_->SetUniformVariable("transform", transform);
  component_->Attach(shader_);

  needs_redraw_ = false;

  return true;
}

void RayView::SetGeometry(glm::vec3 position, glm::quat quaternion) {
  position_ = position;
  quaternion_ = quaternion;
  needs_redraw_ = true;
}

void RayView::UpdateRay(std::shared_ptr<model::Ray> ray) {
  ray_ = ray;

  auto vertices = vertex_buffer_->GetData<Vertex>();
  vertices[0].p = ray_->origin;
  vertices[1].p = ray_->target;
  vertex_buffer_->ReattachBuffer();
  component_->Attach(vertex_buffer_);

  needs_redraw_ = true;
}

uint64_t RayView::GetId() { return ray_->client_id; }

}  // namespace zigen_playground
