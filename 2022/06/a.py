import sys
sys.path.append('..')
from aoc_utils import print_solution, print_test

def get_first_marker_count(file_name, char_len):
    datastream = open(file_name).read()
    for chars in range(char_len, len(datastream)):
        if len(set(datastream[chars - char_len:chars])) == char_len:
            break
    return chars

print_test(1, 7, get_first_marker_count('sample', 4))
print_solution(1, get_first_marker_count('input', 4))
print_test(2, 19, get_first_marker_count('sample', 14))
print_solution(2, get_first_marker_count('input', 14))