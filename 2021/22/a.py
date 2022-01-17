from typing import NamedTuple

import sys
sys.path.append('..')
from aoc_utils import print_solution, print_test

INIT_MAX = 50
INIT_MIN = -50

class Step(NamedTuple):
    on: int
    x1: int
    x2: int
    y1: int
    y2: int
    z1: int
    z2: int

def read_file(file) :
    steps = []
    with open(file) as f:
        for line in f:
            (directive, ranges) = line.strip().split(' ')
            (x, y, z) = ranges.split(',')
            (_, x_range) = [p.split('..') for p in x.split('=')]
            (_, y_range) = [p.split('..') for p in y.split('=')]
            (_, z_range) = [p.split('..') for p in z.split('=')]
            x_range = [p.strip(',') for p in x_range]
            y_range = [p.strip(',') for p in y_range]
            x_range = list(map(int, x_range))
            y_range = list(map(int, y_range))
            z_range = list(map(int, z_range))
        
            steps.append(
                Step(1 if directive == "on" else -1, x_range[0], x_range[1], y_range[0], y_range[1], z_range[0], z_range[1]))
    return steps

def process_init(steps):
    on_cuboids = set()
    for step in steps:
        if all(INIT_MIN <= i  <= INIT_MAX for i in [step.x1, step.x2, step.y1, step.y2, step.z1, step.z2]):

            if step.on == 1:
                for x in range(step.x1, step.x2 + 1):
                    for y in range(step.y1, step.y2 + 1):
                        for z in range(step.z1, step.z2 + 1):
                            on_cuboids.add((x, y, z))
            else:
                for x in range(step.x1, step.x2 + 1):
                    for y in range(step.y1, step.y2 + 1):
                        for z in range(step.z1, step.z2 + 1):
                            try:
                                on_cuboids.remove((x, y, z))
                            except KeyError:
                                pass
    return on_cuboids

# Pt 2 inspired by Reddit: 
# https://www.reddit.com/r/adventofcode/comments/rlxhmg/2021_day_22_solutions/hqxczc4/
def intersection(cuboid1, cuboid2):
    mm = [lambda a, b:-b, max, min, max, min, max, min]
    n = [mm[i](cuboid1[i], cuboid2[i]) for i in range (7)]
    return None if n[1] > n[2] or n[3] > n[4] or n[5] > n[6] else n

def count_on(on_cuboids):
    count = 0
    for c in on_cuboids:
        count += c[0] * (c[2] - c[1] + 1) * (c[4] - c[3] + 1) * (c[6] - c[5] + 1)
    return count

def process(steps):
    on_cuboids = []
    for step in steps:
        to_add = [step] if step[0] == 1 else []
        for cuboid in on_cuboids:
            inter = intersection(step, cuboid)
            if inter:
                to_add += [inter]
        on_cuboids += to_add
    return on_cuboids

steps = read_file('sample')
on_cuboids = process_init(steps)
print_test(1, 590784, len(on_cuboids))

steps = read_file("sample2")
on_cuboids = process(steps)
print_test(2, 2758514936282235, count_on(on_cuboids))

steps = read_file("input")
on_cuboids = process_init(steps)
print_solution(1, len(on_cuboids))

on_cuboids = process(steps)
print_solution(2, count_on(on_cuboids))
