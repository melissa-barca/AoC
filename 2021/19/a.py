import re
from copy import copy
from collections import defaultdict, deque
from itertools import product, combinations

import sys
sys.path.append('..')
from aoc_utils import print_solution, print_test

MIN_OVERLAP = 12
ORIENTATION_COUNT = 24

def diff(point1, point2):
    return point1[0] - point2[0], point1[1] - point2[1], point1[2] - point2[2]

def add(point1, point2):
    return point1[0] + point2[0], point1[1] + point2[1], point1[2] + point2[2]

class Scanner:
    def __init__(self, id):
        self.id = id
        self.beacons = None
        self.diffs = None

    def find_diffs_between_beacons(self):
        pairs = combinations(self.beacons, 2)
        diffs = defaultdict(set)

        for p1, p2 in pairs:
            diffs[p1].add(diff(p1, p2))
            diffs[p2].add(diff(p2, p1))

        self.diffs = diffs

    def rotate(self, i):
        def rotations(beacon, i):
            x, y, z = beacon
            orientations = [
                    (x,y,z),
                    (y,-x,z),
                    (-x,-y,z),
                    (-y,x,z),
                    (x,-y,-z),
                    (y,x,-z),
                    (-x,y,-z),
                    (-y,-x,-z),
                    (-z,y,x),
                    (y,z,x),
                    (z,-y,x),
                    (-y,-z,x),
                    (z,y,-x),
                    (-y,z,-x),
                    (-z,-y,-x),
                    (y,-z,-x),
                    (x,-z,y),
                    (z,x,y),
                    (-x,z,y),
                    (-z,-x,y),
                    (x,z,-y),
                    (-z,x,-y),
                    (-x,-z,-y),
                    (z,-x,-y)]
            return orientations[i]
        beacons = [rotations(beacon, i) for beacon in self.beacons]
        self.beacons = beacons
        self.find_diffs_between_beacons()

def find_overlaps(scanner1, scanner2):
    overlaps = defaultdict(set)
    beacon_pairs = product(scanner1.beacons, scanner2.beacons)

    for beacon1, beacon2 in beacon_pairs:
        diff1 = scanner1.diffs[beacon1]
        diff2 = scanner2.diffs[beacon2]
        overlap_count = len(diff1 & diff2)
        if overlap_count >= MIN_OVERLAP - 1:
            overlaps[beacon1].add(beacon2)

    return overlaps

def find_max_distance(scanner_coords):
    max_dist = 0
    for s1, s2 in product(scanner_coords, repeat=2):
        dist = sum(map(abs, diff(s1, s2)))
        if dist > max_dist:
            max_dist = dist
    return max_dist

def find_and_normalize(scanners):
    anchor = scanners.popleft()
    scanner_coords = {}
    
    while scanners:
        test_scanner = scanners.popleft()
        met_overlap_threshold = False

        # test each scanner orientation for min number of overlaps
        for i in range(ORIENTATION_COUNT):
            rotated_scanner = copy(test_scanner)
            rotated_scanner.rotate(i)
            overlaps = find_overlaps(anchor, rotated_scanner)
            if overlaps:
                met_overlap_threshold = True
                beacon1 = list(overlaps.keys())[0]
                beacon2 = overlaps[beacon1].pop()

                # calculate the offset from the anchor and apply to all beacons
                offset = diff(beacon1, beacon2)
                scanner_coords[rotated_scanner.id] = offset

                new_beacons = []
                for b in rotated_scanner.beacons:
                    x, y, z = b
                    x_offset, y_offset, z_offset = offset
                    new_beacons.append(add(b, offset))

                anchor.beacons = list(set(anchor.beacons + new_beacons))
                anchor.find_diffs_between_beacons()
                break

        if not met_overlap_threshold:
            scanners.append(test_scanner)

    return len(set(anchor.beacons)), scanner_coords

def read_file(file_name):
    scanner_regex = r'--- scanner (\d+) ---'
    scanners = []
    beacons = []
    current = None

    with open(file_name) as f:
        for line in f:
            scanner_info = re.findall(scanner_regex, line)
            if scanner_info:
                if current:
                    current.beacons = beacons
                    beacons = []

                new_scanner = Scanner(scanner_info[0])
                scanners.append(new_scanner)
                current = new_scanner

            elif line.strip():
                beacon = tuple(map(int, line.split(',')))
                beacons.append(beacon)

    current.beacons = beacons

    for s in scanners:
        s.find_diffs_between_beacons()

    return deque(scanners)

scanners = read_file('sample')
unique_beacons, scanner_coords = find_and_normalize(scanners)
print_test(1, 79, unique_beacons)
print_test(2, 3621, find_max_distance(scanner_coords.values()))

scanners = read_file('input')
unique_beacons, scanner_coords = find_and_normalize(scanners)
print_solution(1, unique_beacons)
print_solution(2, find_max_distance(scanner_coords.values()))
