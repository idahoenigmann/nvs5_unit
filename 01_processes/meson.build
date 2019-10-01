project('01_processes',
        'cpp',
        version: '0.0.1',
        meson_version: '>0.46',
        default_options : ['cpp_std=c++2a', 'warning_level=3', 'werror=true'])

inc_dir = include_directories('include')
src = ['src/aba.cpp']

executable('go',
           sources : src,
           include_directories : inc_dir)

executable('go2',
           sources : 'src/main.cpp',
           include_directories : inc_dir)