project('finance_manager', 'c',
  version: '1.0.0',
  default_options: ['warning_level=3', 'c_std=c11', 'buildtype=debugoptimized']
)

inc = include_directories('include')

# Создаём директорию для данных
data_dir = meson.current_source_dir() / 'data'
run_command('mkdir', '-p', data_dir, check: false)

# Исходные файлы
src_files = files(
  'src/main.c',
  'src/data.c',
  'src/storage.c',
  'src/ui.c',
  'src/utils.c'
)

# Исполняемый файл
executable('finance_manager',
  src_files,
  include_directories: inc,
  dependencies: [],
  install: false
)

# Тесты
test_src = files('tests/test_finance.c')
test_exe = executable('test_finance',
  [test_src, src_files],
  include_directories: inc,
  dependencies: []
)

test('finance tests', test_exe)

# Конфигурация для разных типов сборки
if get_option('buildtype') == 'release'
  add_project_arguments('-DNDEBUG', language: 'c')
endif