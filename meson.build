project(
    'cpp-skeleton',
    ['c', 'cpp'],
    default_options: [
        'buildtype=release',
        'optimization=3',
        'debug=true',
        'cpp_std=c++20',
        'warning_level=3',
    ],
)

cpp_args = []
add_project_arguments(cpp_args, language: 'cpp')

dependencies = []

include_directories = []

srcs = [
  'main.cpp',
]

executable(
  'cpp-skeleton',
  srcs,
  dependencies: dependencies,
  include_directories: include_directories,
)