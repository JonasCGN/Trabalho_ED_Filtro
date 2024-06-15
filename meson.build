project('Processamento_Image', 'c',
  version: '0.1',
  default_options: ['warning_level=3'])

gtk_dep = dependency('gtk+-3.0')

subdir('functions/image')
subdir('functions/lista')
subdir('functions/interface')

executable('main', 'main.c',
    dependencies : gtk_dep,
    link_with : [functions_dep, lista_dep, interface_dep]
)