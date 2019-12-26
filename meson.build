# Project
project('cwalk', 'c')

# Library
cwalk = shared_library(	'cwalk', 'src/cwalk.c',
						install: true,
						include_directories: 'include',
						soversion: 1, version: '1.0.0'
)

install_headers('include/cwalk.h')

cwalk_dep = declare_dependency(include_directories: 'include', link_with: cwalk)
