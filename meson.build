project('TSP_SA', 'c',
        version : '0.1')

add_global_arguments('-lsqlite3',
                     language : 'c') #-g

sqlite = dependency('sqlite3')

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
                        dependencies: [ sqlite ],
                        include_directories: [ includes ],
                        install : true)

exe = executable('TSP_SA', src,
                 dependencies: [ sqlite ],
                 install : true)
