private_inc = [public_inc, include_directories('.')]

subdir('client')

shader_inputs = {
  'cuboid': ['vert', 'frag'],
}

shaders = []
foreach name, types : shader_inputs
  foreach type : types
    input_file = '@0@.@1@'.format(name, type)
    output_file = '@0@-@1@.h'.format(name, type)
    c_var_name = '@0@_@1@_shader'.format(name, type)

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
  dep_glm,
  dep_zukou,
  dep_zigen_playground_client,
]

srcs_zigen_playground = [
  'cuboid-view.cc',
  'playground.cc',
  'playground-view.cc',
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
