def print_test(part, expected, calculated):
    print('Part', part, 'Sample test', 'PASSED:' if expected == calculated else 'FAILED:',
     'Expected', expected, 'and calculated', calculated)

def print_solution(part, solution):
    print('Part', part, 'Solution: ', solution)