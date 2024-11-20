#include <iostream>
#include <vector>
#include <omp.h>
using namespace std;

// Function to multiply two matrices sequentially
void multiplySequential(const vector<vector<int>> &A, const vector<vector<int>> &B, vector<vector<int>> &C, int N)
{
    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            C[i][j] = 0;
            for (int k = 0; k < N; ++k)
            {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

// Function to multiply two matrices using OpenMP
void multiplyParallel(const vector<vector<int>> &A, const vector<vector<int>> &B, vector<vector<int>> &C, int N)
{
    #pragma omp parallel for collapse(2)
    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            C[i][j] = 0;
            for (int k = 0; k < N; ++k)
            {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

int main()
{
    int N; // Matrix size (1000, 2000, 3000)
    cout << "Enter matrix size (e.g., 1000, 2000, 3000): ";
    cin >> N;

    // Initialize matrices A, B, and C
    vector<vector<int>> A(N, vector<int>(N, 1)); // Matrix A with all elements = 1
    vector<vector<int>> B(N, vector<int>(N, 1)); // Matrix B with all elements = 1
    vector<vector<int>> C(N, vector<int>(N, 0)); // Matrix C to store result

    // Sequential multiplication
    double start = omp_get_wtime();
    multiplySequential(A, B, C, N);
    double end = omp_get_wtime();
    double durationSeq = end - start;
    cout << "Time taken for sequential multiplication: " << durationSeq << " seconds" << endl;

    // Parallel multiplication using OpenMP
    start = omp_get_wtime();
    multiplyParallel(A, B, C, N);
    end = omp_get_wtime();
    double durationPar = end - start;
    cout << "Time taken for parallel multiplication (OpenMP): " << durationPar << " seconds" << endl;
    
    return 0;
}