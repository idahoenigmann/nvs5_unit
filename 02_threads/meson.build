project('02_threads',
        'cpp',
        version: '0.0.1',
        meson_version: '>0.46',
        default_options : ['cpp_std=c++2a', 'warning_level=3', 'werror=true'])

inc_dir = include_directories(['include', get_option('resources')])
src = ['src/main.cpp']

thread_dep = dependency('threads')

executable('go',
           sources : src,
           include_directories : inc_dir,
           dependencies: thread_dep)