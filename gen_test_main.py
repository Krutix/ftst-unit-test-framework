import sys
import argparse
import subprocess
import os

def get_all_tests(files : [str]) -> [str]: # get list of test files and fetch all test cases
    pattern = '-G \'^TEST(.*)$\''
    cmd = f'grep {pattern} -h {" ".join(str(file) for file in files)}'
    output = subprocess.check_output(cmd, shell=True)
    return str(output).strip("'b\\n").split("\\n")

def create_test_run_main(files, stdout=1, **kwargs):
    ftst_test_runner = open('ftst_test_runner.c', 'w')

    ftst_test_runner.write("#define FTST_MAIN_FILE\n")
    ftst_test_runner.write("#include \"ftst.h\"\n")
    ftst_test_runner.write("\n")

    # test functions declaration
    tests = get_all_tests(files)
    for test in tests:
        ftst_test_runner.write(f"{test};\n")

    ftst_test_runner.write("\n")
    # main function
    ftst_test_runner.write("int main()\n")
    ftst_test_runner.write("{\n")

    init_str = f"\tFTST_INIT({'stderr' if stdout == 2 else 'stdout' if stdout == 1 else 'NULL'}, "
    init_str += "{});\n".format(f"\"{kwargs['file_stream']}\"" if 'file_stream' in kwargs else 'NULL')
    ftst_test_runner.write(init_str)
    ftst_test_runner.write("\n")

    # run test functions
    for test in tests:
        ftst_test_runner.write(f"\t{test.replace('TEST', 'RUNTEST')};\n")

    ftst_test_runner.write("\n")
    # return test status (0 - success, -1 fail)
    ftst_test_runner.write("\treturn FTST_EXIT();\n")
    ftst_test_runner.write("}\n")


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
    create_test_run_main(args.ftst_src_files, 2 if args.e != None else 0 if args.s != None else 1, **arg_dict)
