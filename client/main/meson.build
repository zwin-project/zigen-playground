shader_inputs = {
  'pedestal': ['vert', 'frag'],
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

deps_zigen_playground_sample = [
  dep_glm,
  dep_zigen_playground,
  dep_zukou,
]

srcs_zigen_playground_sample = [
  'main.cc',
  'pedestal.cc',
  'window.cc',
  zigen_client_protocol_h,
  zigen_shell_client_protocol_h,
  zigen_opengl_client_protocol_h,
] + shaders

executable(
  'zigen-playground',
  srcs_zigen_playground_sample,
  install : true,
  dependencies : deps_zigen_playground_sample,
)
