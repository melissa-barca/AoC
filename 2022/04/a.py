import sys
sys.path.append('..')
from aoc_utils import print_solution, print_test
from parse import parse

def isContained(pair):
    (p1, p2), (p3, p4) = pair
    if ((p1 >= p3) and (p2 <= p4)) or ((p3 >= p1) and (p4 <= p2)):
       return 1
    return 0

def isOverlap(pair):
    (p1, p2), (p3, p4) = pair
    if (((p1 >= p3) and (p1 <= p4)) or ((p2 >= p3) and (p2 <= p4)) or ((p3 >= p1) and (p3 <= p2)) or ((p4 >= p1) and (p4 <= p2))):
        return 1
    return 0

def get_pairs(file_name):
    input = open(file_name).read().splitlines()
    return [((r[0],r[1]),(r[2],r[3])) for r in [parse('{:d}-{:d},{:d}-{:d}', l) for l in input]]
    
def contains(file_name):
    return sum([isContained(pair) for pair in get_pairs(file_name)])

def overlaps(file_name):
    return sum([isOverlap(pair) for pair in get_pairs(file_name)])

print_test(1, 2, contains('sample'))
print_solution(1, contains('input'))
print_test(2, 4, overlaps('sample'))
print_solution(2, overlaps('input'))