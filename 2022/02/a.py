import sys
sys.path.append('..')
from aoc_utils import print_solution, print_test

def get_shape_score(shape):
    match shape:
        case 'A' | 'X': # rock
            return 1
        case 'B' | 'Y': # paper
            return 2
        case 'C' | 'Z': # scissors
            return 3

def get_winner_score(p1, p2):
    if get_shape_score(p1) == get_shape_score(p2):
        return [3, 3]
    if (p1 == 'A' and get_shape_score(p2) == 3) or (p1 == 'B' and get_shape_score(p2) == 1) or (p1 == 'C' and get_shape_score(p2) == 2):
        return [6, 0]
    return [0, 6]
    
def get_winner(shape):
    match shape:
        case 'A': 
            return 'B'
        case 'B':
            return 'C'
        case 'C':
            return 'A'

def get_loser(shape):
    match shape:
        case 'A':
            return 'C'
        case 'B':
            return 'A'
        case 'C':
            return 'B'
        
def get_p2_shape(p1, outcome):
    match outcome:
        case 'X':
            return get_loser(p1)
        case 'Y':
            return p1
        case 'Z':
            return get_winner(p1)
            
def get_points(round):
    p1, p2 = round[0], round[2]
    _, p2_score = get_winner_score(p1, p2)
    p2_score += get_shape_score(p2)
    return p2_score

def get_points_part2(round):
    p1, outcome = round[0], round[2]
    p2 = get_p2_shape(p1, outcome)
    round = [p1, '', p2]
    return get_points(round)

def get_score(file_name, points_fn):
    return sum([points_fn(round) for round in open(file_name).read().splitlines()])

print_test(1, 15, get_score('sample', get_points))
print_solution(1, get_score('input', get_points))
print_test(2, 12, get_score('sample', get_points_part2))
print_solution(2, get_score('input', get_points_part2))
