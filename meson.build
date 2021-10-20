project(
    'fastq_parser_comparison',
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

dependencies = [
  dependency('absl_flags'),
  dependency('absl_flags_parse'),
  dependency('glog'),
  dependency('zlib'),
]

include_directories = []

srcs = [
  'main.cpp',
  'fxtract/kseq.cpp',
]

executable(
  'fastq_parser_comparison',
  srcs,
  dependencies: dependencies,
  include_directories: include_directories,
)