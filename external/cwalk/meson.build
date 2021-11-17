project('cwalk', 'c',
  license: 'MIT',
  meson_version: '>= 0.45.1'
)

cwalk_inc = include_directories('include')

cwalk = library('cwalk', 'src/cwalk.c',
  install: true,
  include_directories: cwalk_inc
)

install_headers('include/cwalk.h')

cwalk_dep = declare_dependency(include_directories: 'include', link_with: cwalk)
