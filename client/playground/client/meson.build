deps_zigen_playground_client = [
  dep_boost,
  dep_glm,
]

srcs_zigen_playground_client = [
  'client.cc',
  'client-impl.cc',
  'message-parser.cc',
  'messaging.cc',
  'message/error.cc',
  'message/new-resource-event.cc',
  'message/new-resource-request.cc',
  'message/noop-event.cc',
  'message/sync-event.cc',
  'message/sync-request.cc',
]

lib_zigen_playground_client = static_library(
  'zigen-playground-client',
  srcs_zigen_playground_client,
  install : false,
  dependencies : deps_zigen_playground_client,
  include_directories : private_inc
)

dep_zigen_playground_client = declare_dependency(
  link_with : lib_zigen_playground_client,
  include_directories : public_inc,
)
