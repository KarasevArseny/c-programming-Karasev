project('fileutils', 'c',
  version: '1.0.0',
  default_options: ['warning_level=3', 'c_std=c11']
)

inc = include_directories('include')

lib_src = files(
  'src/path.c',
  'src/io.c'
)

fileutils = static_library('fileutils',
  lib_src,
  include_directories: inc
)

executable('fileutils_demo',
  'src/main.c',
  include_directories: inc,
  link_with: fileutils
)

test_exe = executable('test_fileutils',
  'tests/test_fileutils.c',
  include_directories: inc,
  link_with: fileutils
)

test('fileutils tests', test_exe)