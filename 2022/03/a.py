import sys
sys.path.append('..')
from aoc_utils import print_solution, print_test

def get_score(c):
    unicode = ord(c)
    if unicode > 96:
        return unicode - 96
    return unicode - 38

def split_runsack(runsack):
    half = len(runsack)//2
    return runsack[:half], runsack[half:]

def get_runsack_score(r):
    rs1, rs2 = split_runsack(r)
    both = set(rs1).intersection(set(rs2))
    return sum([get_score(c) for c in both]) 
    
def process_group(group):
    group = [sack.strip() for sack in group]
    badge = set(group[0]).intersection(set(group[1]), set(group[2]))
    return get_score(badge.pop())

def get_sum(file_name):
    runsacks = open(file_name).read().splitlines()
    return sum([get_runsack_score(r) for r in runsacks])

def get_badge_sum(file_name):
    groups = zip(*[open(file_name)]*3)
    return sum([process_group(group) for group in groups])

print_test(1, 157, get_sum('sample'))
print_solution(1, get_sum('input'))
print_test(2, 70, get_badge_sum('sample'))
print_solution(2, get_badge_sum('input'))