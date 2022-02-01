dir_zgn_protocol_base = dep_zgn_protocol.get_variable('pkgdatadir')
prog_scanner = find_program(dep_wayland_scanner.get_variable('wayland_scanner'))

protocols = [ 'zigen', 'zigen-shell', 'zigen-opengl' ]

foreach proto : protocols
  xml_path = join_paths(dir_zgn_protocol_base, '@0@.xml'.format(proto))

  output_file = '@0@-client-protocol.h'.format(proto)

  var_name = output_file.underscorify()
  target = custom_target(
    '@0@ @1@'.format(proto, 'client-header'),
    command: [ prog_scanner, 'client-header', '@INPUT@', '@OUTPUT@' ],
    input: xml_path,
    output: output_file,
  )

  set_variable(var_name, target)
endforeach
