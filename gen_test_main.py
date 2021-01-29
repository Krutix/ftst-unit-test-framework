import sys
import subprocess
import os

def get_all_tests(files : [str]) -> [str]: # get list of test files and fetch all test cases
    pattern = '-G \'^TEST(.*)$\''
    cmd = f'grep {pattern} -h {" ".join(str(file) for file in files)}'
    output = subprocess.check_output(cmd, shell=True)
    return str(output).strip("'b\\n").split("\\n")

def create_test_run_main(files):
    ftst_test_runner = open('ftst_test_runner.c', 'w')

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
    ftst_test_runner.write("\tFTST_INIT();\n")
    ftst_test_runner.write("\n")

    # run test functions
    for test in tests:
        ftst_test_runner.write(f"\t{test.replace('TEST', 'RUNTEST')};\n")

    ftst_test_runner.write("\n")
    ftst_test_runner.write("\tFTST_EXIT();\n")
    ftst_test_runner.write("}\n")


if __name__ == '__main__':
    if len(sys.argv) == 1:
        exit
    create_test_run_main(sys.argv[1:])
