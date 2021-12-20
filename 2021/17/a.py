import sys
sys.path.append('..')
from aoc_utils import print_solution, print_test

def read_file(file) :
    with open(file) as f:
        input = [line.strip() for line in f]
        (_, _, x, y) = input[0].split(' ')
        (_, x_range) = [p.split('..') for p in x.split('=')]
        (_, y_range) = [p.split('..') for p in y.split('=')]
        x_range = [p.strip(',') for p in x_range]

        x_range = list(map(int, x_range))
        y_range = list(map(int, y_range))
        return x_range, y_range

def do_steps(init, target):
    velocity_x, velocity_y = init
    target_x, target_y = target
    px, py = (0, 0)
    max_y = 0

    while (px < target_x[0] or py > target_y[1]) and py > target_y[0]:
        px += velocity_x
        py += velocity_y

        if py > max_y:
            max_y = py
   
        if (velocity_x < 0):
            velocity_x += 1
        elif (velocity_x > 0):
            velocity_x -= 1
        velocity_y -= 1

    if target_x[0] <= px <= target_x[1] and target_y[0] <= py <= target_y[1]:
        return [True, max_y]
    else:
        return [False]

def process(file_name) :
    x_range, y_range = read_file(file_name)
    results = []
    for x in range (1, x_range[1] + 1):
        for y in range (y_range[0], x_range[1]):
            res = do_steps((x, y), (x_range, y_range))
            if res[0]:
                results.append(res[1])
    return results

results = process('sample')
print_test(1, 45, max(results))
print_test(2, 112, len(results))
results = process('input')
print_solution(1, max(results))
print_solution(2, len(results))