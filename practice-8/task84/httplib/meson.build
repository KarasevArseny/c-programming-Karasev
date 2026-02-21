project('httplib', 'c',
  version: '1.0.0',
  default_options: ['warning_level=3', 'c_std=c11']
)

inc = include_directories('include')

lib_src = files(
  'src/http_request.c',
  'src/http_response.c'
)

httplib = static_library('httplib',
  lib_src,
  include_directories: inc
)

executable('httplib_demo',
  'src/main.c',
  include_directories: inc,
  link_with: httplib
)

test_exe = executable('test_httplib',
  'tests/test_httplib.c',
  include_directories: inc,
  link_with: httplib
)

test('httplib tests', test_exe)