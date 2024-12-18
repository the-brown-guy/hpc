#include <iostream>
#include <mpi.h>
#include <vector>
#include <cstdlib>
using namespace std;

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);
    int world_size, world_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    int n = 10;
    vector<int> local_array(n);
    int local_sum = 0, total_sum = 0;
    srand((unsigned int)time(0) + world_rank);

    for (int i = 0; i < n; ++i)
    {
        local_array[i] = rand() % 100;
        local_sum += local_array[i];
    }
    cout << "Process " << world_rank << " local array: ";

    for (int i : local_array)
    {
        cout << i << " ";
    }
    cout << "\nProcess " << world_rank << " local sum: " << local_sum << endl;

    MPI_Reduce(&local_sum, &total_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Bcast(&total_sum, 1, MPI_INT, 0, MPI_COMM_WORLD);
    
    if (world_rank == 0)
    {
        double average = static_cast<double>(total_sum) / (n * world_size);
        cout << "Total sum: " << total_sum << endl;
        cout << "Average: " << average << endl;
    }
    MPI_Finalize();
    return 0;
}
