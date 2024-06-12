# Adiciona a biblioteca 'interface' que será usada por outros componentes
interface_sources = [
  'tela.c'
]

interface_dep = static_library('interface', interface_sources, 
    dependencies: gtk_dep,
    install: true,
    install_dir: 'lib'
)

install_headers('tela.h', subdir: 'interface')
