project(
  'zigen-playground', 'c', 'cpp',
  version : '0.0.1',
  license : 'Apache-2.0',
  meson_version : '>= 0.52.1',
  default_options :  [ 'warning_level=3', 'werror=true', 'optimization=2', 'cpp_std=gnu++11' ],
)

dep_zukou = dependency('libzukou')
dep_glm = dependency('glm')
dep_boost = dependency('boost', modules : ['thread'])
dep_curl = dependency('libcurl')
dep_jpeg = dependency('libjpeg')
dep_png = dependency('libpng')

prog_python = import('python').find_installation('python3')
files_textify_py = files('tools/textify.py')
cmd_textify = [ prog_python, files_textify_py, '@INPUT@', '@OUTPUT@']

subdir('client')
