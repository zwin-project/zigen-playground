deps_zigen_playground = [
  dep_glm,
  dep_zukou,
]

srcs_zigen_playground = [
  'playground.cc',
  zigen_client_protocol_h,
  zigen_shell_client_protocol_h,
  zigen_opengl_client_protocol_h,
]

lib_zigen_playground = static_library(
  'zigen-playground',
  srcs_zigen_playground,
  install : false,
  dependencies : deps_zigen_playground,
  include_directories : public_inc,
)

dep_zigen_playground = declare_dependency(
  link_with : lib_zigen_playground,
  include_directories : public_inc,
)