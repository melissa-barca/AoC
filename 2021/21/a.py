from os import read
import sys
sys.path.append('..')
from aoc_utils import print_solution, print_test

NUM_OF_SPACES = 10
MAX_ROLL = 100
ROLLS_PER_TURN = 3

def read_file(file_name):
    with open(file_name) as f:
        p1, p2 = f.read().strip().split('\n')
    return int(p1[-2:]), int(p2[-2:])
        
def roll_deterministic(prior_value):
    if prior_value == 100:
        return 1
    value = prior_value + 1
    return value

def move_player(start_pos, value):
    new_pos = (start_pos + value - 1) % 10 + 1
    return new_pos

def assess_determ_turns(start, die_value):
    for x in range(3):
        die_value = roll_deterministic(die_value)
        start = move_player(start, die_value)
    return start, die_value

def play_deterministic(p1_start, p2_start):
    _win_threshold = 1000

    p1_pts = p2_pts = 0
    determ_die_value = 0
    die_rolls = 0

    while p1_pts < _win_threshold and p2_pts < _win_threshold:
        p1_start, determ_die_value = assess_determ_turns(p1_start, determ_die_value)
        die_rolls += ROLLS_PER_TURN
        p1_pts += p1_start
        if p1_pts < _win_threshold:
            p2_start, determ_die_value = assess_determ_turns(p2_start, determ_die_value)
            die_rolls += ROLLS_PER_TURN
            p2_pts += p2_start
    
    return min(p1_pts, p2_pts), die_rolls

def calculate_solution(losing_score, die_rolls):
    return losing_score * die_rolls

def define_rolls():
    global _rolls
    _rolls = [0 for i in range(10)]
    for i in range(1,4):
        for j in range(1,4):
            for k in range(1,4):
                _rolls[i+j+k] += 1

def assess_turns(start, it, pts):
    value = _rolls[it]
    new_pos = (start + it - 1) % 10 + 1
    new_pts = pts + new_pos
    return value, new_pos, new_pts

def set_other_states(other_states, p1_start, p2_start, p1_pts, p2_pts, value, count):
    state = (p1_start, p2_start, p1_pts, p2_pts)
    other_states[state] = other_states.get(state, 0) + value * count
    return other_states
    
def take_turn(states, other_states, wins):
    for ((start, other_start, pts, other_pts), count) in states.items():
        for r in range(3, 10):
            value, new, new_pts = assess_turns(start, r, pts)
            if new_pts < _win_threshold:
                other_states = set_other_states(other_states, new, other_start, new_pts, other_pts, value, count)
            else:
                wins += value * count
    return wins, other_states
            
def real_game(p1_start, p2_start):
    global _win_threshold
    _win_threshold = 21
    p1_wins = p2_wins = 0
    p1_states = {(p1_start, p2_start, 0, 0): 1}
    p2_states = {}

    while p1_states:
        p1_wins, p2_states = take_turn(p1_states, p2_states, p1_wins)
        p1_states = {}

        for ((p1_start, p2_start, p1_pts, p2_pts), count) in p2_states.items():
            for r in range(3, 10):
                value, p2_next, p2_pts_next = assess_turns(p2_start, r, p2_pts)
                if p2_pts_next < _win_threshold:
                    p1_states = set_other_states(p1_states, p1_start, p2_next, p1_pts, p2_pts_next, value, count)
                else:
                    p2_wins += value * count
        p2_states = {}

    return max(p1_wins, p2_wins)

p1_start, p2_start = read_file('sample')
losing_score, die_rolls = play_deterministic(p1_start, p2_start)
print_test(1, 739785, calculate_solution(losing_score, die_rolls))

define_rolls()
print_test(2, 444356092776315, real_game(p1_start, p2_start))

p1_start, p2_start = read_file('input')
losing_score, die_rolls = play_deterministic(p1_start, p2_start)
print_solution(2, calculate_solution(losing_score, die_rolls))
print_solution(2, real_game(p1_start, p2_start))

