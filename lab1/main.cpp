#include <iostream>
#include <vector>
#include <fstream>
#include <thread>
#include <chrono>

using namespace std;

using Matrix = vector<vector<double>>;

// Чтение матрицы из файла
Matrix readMatrix(const string& filename, int& N) {
    ifstream file(filename);
    if (!file) {
        cout << "Ошибка открытия файла: " << filename << endl;
        exit(1);
    }

    file >> N;

    Matrix mat(N, vector<double>(N));
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            file >> mat[i][j];

    return mat;
}

// Запись матрицы в файл
void writeMatrix(const string& filename, const Matrix& mat) {
    ofstream file(filename);
    int N = mat.size();

    file << N << endl;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j)
            file << mat[i][j] << " ";
        file << endl;
    }
}

// Параллельное умножение части строк
void multiplyPart(const Matrix& A,
    const Matrix& B,
    Matrix& C,
    int startRow,
    int endRow,
    int N) {

    for (int i = startRow; i < endRow; ++i) {
        for (int j = 0; j < N; ++j) {
            for (int k = 0; k < N; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

int main() {
    int N1, N2;

    Matrix A = readMatrix("matrixA.txt", N1);
    Matrix B = readMatrix("matrixB.txt", N2);

    if (N1 != N2) {
        cout << "Размеры матриц не совпадают!" << endl;
        return 1;
    }

    int N = N1;
    Matrix C(N, vector<double>(N, 0));

    int threadCount;
    cout << "Enter number of threads: ";
    cin >> threadCount;
    if (threadCount == 0) threadCount = 4;

    vector<thread> threads;
    int blockSize = N / threadCount;

    auto start = chrono::high_resolution_clock::now();

    for (int t = 0; t < threadCount; ++t) {
        int startRow = t * blockSize;
        int endRow = (t == threadCount - 1) ? N : startRow + blockSize;

        threads.emplace_back(multiplyPart,
            cref(A),
            cref(B),
            ref(C),
            startRow,
            endRow,
            N);
    }

    for (auto& th : threads)
        th.join();

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;

    writeMatrix("result.txt", C);

    long long operations = 2LL * N * N * N;

    cout << "Matrix size: " << N << "x" << N << endl;
    cout << "Threads used: " << threadCount << endl;
    cout << "Operations (approx): " << operations << endl;
    cout << "Execution time: " << duration.count() << " seconds" << endl;

    return 0;
}