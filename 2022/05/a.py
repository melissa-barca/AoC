from parse import parse
import sys
sys.path.append('..')
from aoc_utils import print_solution, print_test

def get_stacks_and_moves(file_name):
    input = open(file_name).read()
    stack, moves = input.split('\n\n')
    stack_lines = stack.split('\n')
    num_of_stacks = int(len(stack_lines[-1])/4) + 1
    height_of_stacks = len(stack_lines) - 1
    stack_lines = [l + ' '*4*num_of_stacks for l in stack_lines[:-1]]
    cols = [[stack_lines[height_of_stacks-1-i][4*c + 1] for i in range(height_of_stacks)] for c in range(num_of_stacks)]
    stacks = [[c for c in col if c != ' '] for col in cols]
    moves = [parse('move {:d} from {:d} to {:d}', line) for line in moves.splitlines()]
    return stacks, moves

def crate_mover_9000(file_name):
    stacks, moves = get_stacks_and_moves(file_name)
    for (count, frm, to) in moves:
        for _ in range(count):
            c = stacks[frm-1].pop()
            stacks[to-1].append(c)
    return ''.join(s[-1] for s in stacks)

def crate_mover_9001(file_name):
    stacks, moves = get_stacks_and_moves(file_name)
    for (count, frm, to) in moves:
        stacks[frm-1], move = stacks[frm-1][:-count], stacks[frm-1][-count:]
        stacks[to-1] += move
    return ''.join(s[-1] for s in stacks)

print_test(1, 'CMZ', crate_mover_9000('sample'))
print_solution(1, crate_mover_9000('input'))
print_test(2, 'MCD', crate_mover_9001('sample'))
print_solution(2, crate_mover_9001('input'))