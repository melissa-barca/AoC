import sys
sys.path.append('..')
from aoc_utils import print_solution, print_test

def get_cals(file_name):
    elves = open(file_name).read().split('\n\n')
    cals = [[int(c) for c in elf.splitlines()] for elf in elves]
    return [sum(c) for c in cals]

def most_elf_cals(file_name):
    return max(get_cals(file_name))

def top_three_elf_cals(file_name):
    ordered = list(reversed(sorted(get_cals(file_name))))
    return sum(ordered[:3])

print_test(1, 24000, most_elf_cals('sample'))
print_solution(1, most_elf_cals('input'))

print_test(2, 45000, top_three_elf_cals('sample'))
print_solution(2, top_three_elf_cals('input'))