import sys
import argparse
import subprocess
import os

def get_all_tests(files : [str]) -> [str]: # get list of test files and fetch all test cases
    pattern = '-G \'^TEST(.*)$\''
    cmd = f'grep {pattern} -h {" ".join(str(file) for file in files)}'
    output = subprocess.check_output(cmd, shell=True)
    return str(output).strip("'b\\n").split("\\n")

def create_test_run_main(files, stdout='stdout', **kwargs):
    ftst_test_runner = open('ftst_test_runner.c', 'w')
    tests = get_all_tests(files)
    nl = '\n'
    main_function_text = \
f'''
#define FTST_MAIN_FILE
#include "ftst.h"

{nl.join([test_case + ';' for test_case in tests])}

int ftst_entry_point()
{{

	FTST_INIT({stdout});

	{nl.join([test_case.replace('TEST', 'RUNTEST') + ';' for test_case in tests])}

   	return FTST_EXIT();
}}
'''
    ftst_test_runner.write(main_function_text)


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Setup ftst run tests')
    parser.add_argument('ftst_src_files', metavar='FILES', type=str, nargs='+', help='a files which contain ftst test cases')
    parser.add_argument('-f', metavar='result_file_name', nargs=1, type=str, help='a file name to save test result')
    parser.add_argument('-e', action='store_const', const='stderr', help='output stream go to stderr')
    parser.add_argument('-s', action='store_const', const='NULL', help='no output stream in console')

    args = parser.parse_args()
    arg_dict = {}
    if args.f != None:
        arg_dict = { 'file_stream': args.f[0] }
    output = 2 if args.e != None else 0 if args.s != None else 1
    output = 'stderr' if output == 2 else 'stdout' if output == 1 else 'NULL' 
    create_test_run_main(args.ftst_src_files, output, **arg_dict)
