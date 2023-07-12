"""
License: WTFPL
Usage: make --dry-run --always-make | python makefile_to_compile_commands.py > compile_commands.json
"""

import json
import os
import re
import sys

def match_directory_line(line):
    return re.search("Entering directory '([^']*)'", line)

def match_compiler_line(line):
    # Add any missing compilers here
    compiler_list = ['cc', 'gcc', 'g\+\+', 'clang', 'clang\+\+']
    compilers = '(%s)' % ('|'.join(compiler_list))
    return re.search("%s.*" % compilers, line) # Match any compiler in the list and the rest of the line

def main():
    compile_commands = []
    current_directory = os.getcwd()
    for line in sys.stdin:
        dir_match = match_directory_line(line)
        compiler_match = match_compiler_line(line)
        if dir_match:
            current_directory = dir_match.group(1) # If we move into a directory, update the current directory
        elif compiler_match:
            compile_command = compiler_match.group(0)
            compile_commands.append({
                'directory': current_directory,
                'command': compile_command,
                'file': compile_command.strip().split(' ')[-1] # The file is the last space separated value in the command
            })
    print(json.dumps(compile_commands))

if __name__ == '__main__':
    main()
