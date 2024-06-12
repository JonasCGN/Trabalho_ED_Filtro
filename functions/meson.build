# Adiciona a biblioteca 'functions' que será usada por outros componentes
functions_sources = [
  'image.c'
]

functions_dep = static_library('functions', functions_sources, 
    dependencies: gtk_dep,
    install: true,
    install_dir: 'lib'
)

install_headers('image.h', subdir: 'functions')
