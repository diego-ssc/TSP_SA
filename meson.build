project('TSP_SA', 'c',
        version : '0.1')

add_global_arguments('-lsqlite3',
                     language : 'c') #-g

cc = meson.get_compiler('c')
m_dep = cc.find_library('m', required : true)
sqlite = dependency('sqlite3')
glib = dependency('glib-2.0')

src = [
  'src/main.c',
  'src/database_loader.c',
  'src/report.c',
  'src/tsp.c',
  'src/normalizer.c',
  'src/city.c'
]

includes = include_directories('src/')

TSP_SA = static_library('TSP_SA', src,
                        dependencies: [ sqlite, glib, m_dep ],
                        include_directories: [ includes ],
                        install : true)

exe = executable('TSP_SA', src,
                 dependencies: [ sqlite, glib, m_dep ],
                 install : true)

# tests
checks = [ 'city' ]
foreach check : checks
  check_sources = [ 'test/test_' + check + '.c' ]
  check_check = executable('test_' + check, check_sources,
                           dependencies: [ sqlite, glib, m_dep ],
                           include_directories: [ includes ],
                           link_with: [ TSP_SA ])
  test('Test ' + check, check_check)
endforeach
