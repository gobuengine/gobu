project('gobuengine', 'c',
  version : '0.1',
  default_options : ['warning_level=0'])
  
base_dir = meson.current_source_dir()

add_project_arguments('-DGRAPHICS_API_OPENGL_33', language: 'c')
add_project_arguments('-Wno-incompatible-pointer-types', language: 'c')

subdir('src')

