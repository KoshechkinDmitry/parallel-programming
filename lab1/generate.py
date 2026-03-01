import numpy as np

N = 1000 # размер матрицы 

A = np.random.randint(0, 10, (N, N))
B = np.random.randint(0, 10, (N, N))

with open("matrixA.txt", "w") as f:
    f.write(str(N) + "\n")
    for row in A:
        f.write(" ".join(map(str, row)) + "\n")

with open("matrixB.txt", "w") as f:
    f.write(str(N) + "\n")
    for row in B:
        f.write(" ".join(map(str, row)) + "\n")

print("Матрицы сгенерированы")
