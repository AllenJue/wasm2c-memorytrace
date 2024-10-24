# this file:
#  - reads the solutions from one file
#  - verifies the solutions by performing a matrix multiplication
#  - prints the results (pass/fail)
# The actual file is structured as follows:
#  - Some amount of lines with non-numeric characters
#  - The first line contains the dimensions of the first matrix 'n' (it assumes that the matrix is square)
#  - The next 'n' lines contain the first matrix with 'n' elements in each line
#  - The next line contains the dimensions of the second matrix 'm' (it assumes that the matrix is square)
#  - The next 'm' lines contain the second matrix with 'm' elements in each line
#  - The next line contains the dimensions of the result matrix 'r' (it assumes that the matrix is square)
#  - The next 'r' lines contain the result matrix with 'r' elements in each line

import sys
import numpy as np

def read_matrix(file):
    # ignore non-numeric inputs
    while True:
      line = file.readline().split()
      if len(line) > 1 and line[1].isdigit():
        break

    # return
    n = int(line[1])
    matrix = []
    for i in range(n):
        matrix.append(list(map(int, file.readline().split())))
    return np.array(matrix)

def verify_solutions(file):
    A = read_matrix(file)
    B = read_matrix(file)
    R = read_matrix(file)
    C = np.matmul(A, B)

    # debugging: print A,B,R,C
    # print(A)
    # print(B)
    # print(R)
    # print(C)

    if np.array_equal(C, R):
        print("PASS")
    else:
        print("FAIL")
    
if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python3 verify_solution.py <solutions_file>")
        sys.exit(1)
    with open(sys.argv[1], "r") as file:
        verify_solutions(file)