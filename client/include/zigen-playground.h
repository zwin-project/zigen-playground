#ifndef ZIGEN_PLAYGROUND_H
#define ZIGEN_PLAYGROUND_H

#include <zukou/zukou.h>

#include <functional>
#include <glm/glm.hpp>
#include <memory>
#include <string>
#include <vector>

namespace zigen_playground {

namespace model {
class Resource;
}  // namespace model

class PlaygroundView;
class Client;

class Playground : public std::enable_shared_from_this<Playground>,
                   public zukou::objects::IObject {
 public:
  static std::shared_ptr<Playground> Create(
      std::shared_ptr<zukou::Application> app,
      std::shared_ptr<zukou::VirtualObject> virtual_object,
      std::string remote_host, std::string remote_port, std::string user_id);

 public:
  void SetGeometry(glm::vec3 half_size, glm::vec3 position);

  virtual bool Init() final;
  virtual bool Draw() final;
  virtual float Intersect(glm::vec3 origin, glm::vec3 direction) final;
  virtual void RayEnter() final;
  virtual void RayLeave() final;
  virtual void RayMotion(
      glm::vec3 origin, glm::vec3 direction, uint32_t time) final;
  virtual void RayButton(
      uint32_t serial, uint32_t time, uint32_t button, bool pressed) final;

  struct error_signal {};

  void Connect(
      const error_signal, const std::function<void(std::string)> &slot);

 private:
  Playground(std::shared_ptr<zukou::Application> app,
      std::shared_ptr<zukou::VirtualObject> virtual_object,
      std::string remote_host, std::string remote_port, std::string user_id);

  void NoopEvent();
  void SyncEvent(std::vector<std::shared_ptr<model::Resource>> resources);
  void ClientErrorEvent(std::string reason);

 private:
  glm::vec3 half_size_;
  glm::vec3 position_;
  std::shared_ptr<zukou::Application> app_;
  std::shared_ptr<zukou::VirtualObject> virtual_object_;
  std::shared_ptr<PlaygroundView> view_;
  std::shared_ptr<Client> client_;
  std::function<void(std::string)> error_callback_;
  std::string remote_host_;
  std::string remote_port_;
  std::string user_id_;
};

}  // namespace zigen_playground

#endif  //  ZIGEN_PLAYGROUND_H
