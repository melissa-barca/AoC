import numpy as np
from itertools import chain
 
import sys
sys.path.append('..')
from aoc_utils import print_solution, print_test

def read_file(file_name):
    with open(file_name) as f:
        enhancement_alogrithm, image_grid = f.read().strip().split('\n\n')
    return enhancement_alogrithm, image_grid

def scanner_enhance(enhancement_alogrithm, image_grid, max_run):
    binary = {}
 
    for i, val in enumerate(enhancement_alogrithm):
        if val == '#':
            binary[i] = 1
        else:
            binary[i] = 0
 
    image_grid = image_grid.split('\n')
    image_b = np.zeros((len(image_grid), len(image_grid[0])), dtype=int)
 
    for i, row in enumerate(image_grid):
        for j, val in enumerate(row):
            if val == '#':
                image_b[i,j] = 1
            else:
                image_b[i,j] = 0
 
    run = 1
 
    while run <= max_run:
        if binary[1] == 1 and not run % 2:
            image_pad = np.ones((image_b.shape[0] + 4,
                                 image_b.shape[1] + 4),
                                dtype=int)
        else:
            image_pad = np.zeros((image_b.shape[0] + 4,
                                  image_b.shape[1] + 4),
                                 dtype=int)
 
        image_pad[2:-2,2:-2] = image_b
 
        e_image = np.zeros((image_b.shape[0] + 2,
                            image_b.shape[1] + 2),
                           dtype=int)
 
        for loc, val in np.ndenumerate(image_pad):
            x, y = loc
            if (x in range(1, len(image_pad) - 1)
                and y in range(1, len(image_pad) - 1)):
                sub_pix = image_pad[x-1:x+2, y-1:y+2]
                pix_str = ''
                for row in sub_pix:
                    for s_val in row:
                        pix_str += str(s_val)
 
                e_image[x-1, y-1] = binary[int(pix_str, 2)]
        
        image_b = e_image.copy()
        run += 1
        
    return np.count_nonzero(image_b)
 
enhancement_alogrithm, image_grid = read_file('sample')
print_test(1, 35, scanner_enhance(enhancement_alogrithm, image_grid, 2))
print_test(2, 3351, scanner_enhance(enhancement_alogrithm, image_grid, 50))

enhancement_alogrithm, image_grid = read_file('input')
print_solution(1, scanner_enhance(enhancement_alogrithm, image_grid, 2))
print_solution(2, scanner_enhance(enhancement_alogrithm, image_grid, 50))
