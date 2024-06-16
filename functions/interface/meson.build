# Adiciona a biblioteca 'interface' que será usada por outros componentes
interface_sources = [
  'tela.c'
]

c_args = ['-I/usr/include/python3.10']
c_link_args = ['-L/usr/lib/python3.10/config-3.10-x86_64-linux-gnu', '-lpython3.10']

python_dep = dependency('python-3.10', required : true)
gtk_dep = dependency('gtk+-3.0')

interface_dep = static_library('interface', interface_sources, 
    dependencies: [gtk_dep, python_dep],
    install: true,
    c_args : c_args,
    link_args : c_link_args,
    install_dir: 'lib'
)

install_headers('tela.h', subdir: 'interface')
