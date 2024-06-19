project('seu_projeto', 'c', default_options : ['c_std=c99'])

c_args = ['-I/usr/include/python3.10']
c_link_args = ['-L/usr/lib/python3.10/config-3.10-x86_64-linux-gnu', '-lpython3.10']

python_dep = dependency('python-3.10', required : true)
gtk_dep = dependency('gtk+-3.0')

subdir('functions/image')
subdir('functions/lista')
subdir('functions/interface')

executable('main', 'main.c',
    dependencies : [gtk_dep, python_dep],
    link_with : [functions_dep, lista_dep, interface_dep],
    c_args : c_args,
    link_args : c_link_args
)