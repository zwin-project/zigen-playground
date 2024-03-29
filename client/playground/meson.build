private_inc = [public_inc, include_directories('.')]

subdir('client')

shader_inputs = {
  'cuboid': ['vert', 'frag'],
  'ray': ['vert', 'frag'],
  'sphere': ['vert', 'frag'],
  'sphere-textured': ['frag']
}

shaders = []
foreach name, types : shader_inputs
  foreach type : types
    input_file = '@0@.@1@'.format(name, type)
    output_file = '@0@-@1@.h'.format(name, type)
    c_var_name = '@0@_@1@_shader'.format(name.underscorify(), type)

    target = custom_target(
      output_file,
      command: cmd_textify + [ '-n', c_var_name ],
      input: input_file,
      output: output_file,
    )

    shaders += target
  endforeach
endforeach

deps_zigen_playground = [
  dep_curl,
  dep_glm,
  dep_jpeg,
  dep_png,
  dep_zukou,
  dep_zigen_playground_client,
]

srcs_zigen_playground = [
  'cuboid-view.cc',
  'dnd-message-parser.cc',
  'playground.cc',
  'playground-view.cc',
  'image-downloader.cc',
  'ray-view.cc',
  'sphere-view.cc',
  zigen_client_protocol_h,
  zigen_shell_client_protocol_h,
  zigen_opengl_client_protocol_h,
] + shaders

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
