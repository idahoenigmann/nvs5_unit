project('08_daytime',
        'cpp',
        version: '0.0.1',
        meson_version: '>0.46',
        default_options : ['cpp_std=c++2a', 'warning_level=3', 'werror=true'])

inc_dir = include_directories(['include', get_option('resources'), get_option('asio'), get_option('spdlog')])
src_client = ['src/daytime_client/main.cpp']

src_server = ['src/daytime_server/main.cpp']

thread_dep = dependency('threads')

executable('go_client',
           sources : src_client,
           include_directories : inc_dir,
           dependencies: thread_dep)

executable('go_server',
           sources : src_server,
           include_directories : inc_dir,
           dependencies: thread_dep)