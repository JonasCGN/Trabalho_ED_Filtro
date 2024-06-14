# Adiciona a biblioteca 'functions' que será usada por outros componentes
lista_sources = [
  'list.c'
]

lista_dep = static_library('lista', lista_sources, 
    install: true,
    install_dir: 'lib'
)

install_headers('list.h', subdir: 'lista')