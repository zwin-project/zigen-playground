#ifndef ZIGEN_PLAYGROUND_MESSAGE_HELPER_H
#define ZIGEN_PLAYGROUND_MESSAGE_HELPER_H

namespace zigen_playground {
namespace message {

static glm::vec3 ToVec3(boost::property_tree::ptree const &pt,
    boost::property_tree::ptree::key_type const &key) {
  glm::vec3 v;
  int i = 0;
  for (auto &item : pt.get_child(key)) {
    v[i++] = item.second.get_value<float>();
    if (i >= 3) break;
  }

  return v;
}

}  // namespace message
}  // namespace zigen_playground

#endif  //  ZIGEN_PLAYGROUND_MESSAGE_HELPER_H
