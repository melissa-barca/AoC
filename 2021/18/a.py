import functools
import math

import sys
sys.path.append('..')

from aoc_utils import print_test, print_solution

class Node(object):
    def __init__(self, parent, numbers):
        self.parent = parent
        if isinstance(numbers, int):
            self.value = numbers
        else:
            self.value = None
            self.left = Left(self, numbers[0])
            self.right = Right(self, numbers[1])

    def leaf(self):
        return self.value is not None

    def tuple(self):
        if self.value is not None:
            return self.value
        else:
            return [self.left.tuple(), self.right.tuple()]
           
    def magnitude(self):
        if self.value is not None:
            return self.value
        else:
            return 3 * self.left.magnitude() + 2 * self.right.magnitude()

class Left(Node):
    pass

class Right(Node):
    pass

class Root(Node):
    def __init__(self, numbers):
        self.value = None
        self.parent = None
        self.left = Left(self, numbers[0])
        self.right = Right(self, numbers[1])

def descend_to_pair(node, depth):
    if node.leaf():
        return None
    if depth == 0:
        return node
    return descend_to_pair(node.left, depth - 1) or descend_to_pair(node.right, depth - 1)

def next_over_10(node):
    if node.leaf():
        return node if node.value >= 10 else None
    return next_over_10(node.left) or next_over_10(node.right)

def split(node):
    value = node.value
    l = math.floor(value / 2)
    r = math.ceil(value / 2)
    node.left = Left(node, l)
    node.right = Right(node, r)
    node.value = None

def add(node1, node2):
    r = Root([node1, node2])
    while True:
        n = descend_to_pair(r, 4)
        if n:
            explode(n)
            continue
        n = next_over_10(r)
        if n:
            split(n)
        else:
            return r.tuple()

def next_left_node(node):
    if isinstance(node, Right):
        n = node.parent.left
        while not n.leaf():
            n = n.right
        return n
    elif isinstance(node, Left):
        return next_left_node(node.parent)
    return None

def next_right_node(node):
    if isinstance(node, Left):
        n = node.parent.right
        while not n.leaf():
            n = n.left
        return n
    elif isinstance(node, Right):
        return next_right_node(node.parent)
    return None

def explode(node):
    l = node.left.value
    r = node.right.value
    n = next_left_node(node)
    if n:
        n.value += l
    n = next_right_node(node)
    if n:
        n.value += r
    node.value = 0
    node.left = None
    node.right = None

def largest_sum(numbers):
    largest_sum = 0
    for i, num1 in enumerate(numbers):
        for j, num2 in enumerate(numbers):
            if i == j:
                continue
            num_sum = Root(add(num1, num2)).magnitude()
            if num_sum > largest_sum:
                largest_sum = num_sum
    return largest_sum

def read_file(file_name):
    data = open(file_name).read().strip()
    pairs = list(map(eval, data.split("\n")))
    return pairs

pairs = read_file('sample')
final_sum = Root(functools.reduce(add, pairs))
largest = largest_sum(pairs)
print_test(1, 4140, final_sum.magnitude())
print_test(2, 3993, largest_sum(pairs))

pairs = read_file('input')
final_sum = Root(functools.reduce(add, pairs))
print_solution(1, final_sum.magnitude())
print_solution(2, largest_sum(pairs))
