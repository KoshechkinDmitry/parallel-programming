import numpy as np

def read_matrix(filename):
    with open(filename) as f:
        N = int(f.readline())
        data = [list(map(float, f.readline().split())) for _ in range(N)]
    return np.array(data)

A = read_matrix("matrixA.txt")
B = read_matrix("matrixB.txt")
C_cpp = read_matrix("result.txt")

C_py = np.matmul(A, B)

if np.allclose(C_cpp, C_py):
    print("Результат корректен.")
else:
    print("Ошибка! Результаты не совпадают.")