project('strlib', 'c',
  version: '1.0.0',
  default_options: ['warning_level=3', 'c_std=c11']
)

inc = include_directories('include')

lib_src = files(
  'src/strlib_core.c',
  'src/strlib_search.c',
  'src/strlib_format.c'
)

strlib = static_library('strlib',
  lib_src,
  include_directories: inc
)

executable('strlib_demo',
  'src/main.c',
  include_directories: inc,
  link_with: strlib
)

test_exe = executable('test_strlib',
  'tests/test_strlib.c',
  include_directories: inc,
  link_with: strlib
)

test('strlib tests', test_exe)