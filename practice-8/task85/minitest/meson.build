project('minitest', 'c',
  version: '1.0.0',
  default_options: ['warning_level=3', 'c_std=c11']
)

inc = include_directories('include')

lib_src = files(
  # Библиотека только из заголовочного файла, не требует компиляции
  # Но для совместимости добавим пустой файл или можно создать фиктивный
)

# Так как minitest.h - это header-only библиотека,
# мы просто копируем заголовочный файл при установке
minitest_dep = declare_dependency(
  include_directories: inc
)

executable('minitest_demo',
  'src/main.c',
  dependencies: [minitest_dep]
)

test_exe = executable('test_minitest',
  'tests/test_minitest.c',
  dependencies: [minitest_dep]
)

test('minitest tests', test_exe)