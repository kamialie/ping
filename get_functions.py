# remove if, while, return
import sys
import os
import re

if len(sys.argv) == 1:
    print('usage')
    exit(1)

c_files_funs = set()
h_files_funs = set()
exceptions = ['main', 'if', 'while', 'return']
for i in range(1, len(sys.argv)):
    for (dirpath, dirnames, filenames) in os.walk(sys.argv[i]):
        # print(dirpath, dirnames)
        print(f'{dirpath}:')
        for filename in filenames:
            if filename.endswith('.c') or filename.endswith('.h'):
                with open(os.path.join(dirpath, filename)) as f:
                    functions = set(re.findall('([a-zA-Z0-9_]+)\(.*\)', f.read()))
                    if filename.endswith('.c'):
                        c_files_funs.update(functions)
                    else:
                        h_files_funs.update(functions)
                    print(f'  {filename}:')
                    for fun in functions:
                        print(f'    {fun}')

c_files_funs = c_files_funs.difference(exceptions)
print('Function calls in c files:')
for fun in sorted(c_files_funs):
    print(f'    {fun}')
print('Function declarations in h files:')
for fun in sorted(h_files_funs):
    print(f'    {fun}')
print('Function call not found in headers')
for fun in sorted(c_files_funs.difference(h_files_funs)):
    print(f'    {fun}')
