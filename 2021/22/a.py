from typing import NamedTuple

import sys
sys.path.append('..')
from aoc_utils import print_solution, print_test

INIT_MAX = 50
INIT_MIN = -50

class Step(NamedTuple):
    on: bool
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
                Step(directive == "on", x_range[0], x_range[1], y_range[0], y_range[1], z_range[0], z_range[1]))
    return steps

def process(steps, init):
    on_cuboids = set()
    for step in steps:
        if not init or (init and all(INIT_MIN <= i  <= INIT_MAX for i in [step.x1, step.x2, step.y1, step.y2, step.z1, step.z2])):

            if step.on:
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


steps = read_file('sample')
on_cuboids = process(steps, True)
print_test(1, 590784, len(on_cuboids))

steps = read_file("input")
on_cuboids = process(steps, True)
print_solution(1, len(on_cuboids))

#steps = read_file("sample2")
#on_cuboids = process(steps, False)
#print_test(2, 2758514936282235, len(on_cuboids))