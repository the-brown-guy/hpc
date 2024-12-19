#include <mpi.h>
#include <iostream>
#include <unistd.h> // For sleep (simulating workload)
using namespace std;

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);

    int world_rank, world_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int send_data = world_rank + 10;
    int recv_data;

    // Blocking Communication
    int next_rank = (world_rank + 1) % world_size;
    int prev_rank = (world_rank - 1 + world_size) % world_size;

    // Measure time for Blocking Communication
    double start_time_blocking = MPI_Wtime(); // Start timer

    // Blocking Send and Receive
    MPI_Send(&send_data, 1, MPI_INT, next_rank, 0, MPI_COMM_WORLD);
    MPI_Recv(&recv_data, 1, MPI_INT, prev_rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    double end_time_blocking = MPI_Wtime(); // End timer

    cout << "Blocking: Process " << world_rank << " sent " << send_data << " to " << next_rank
         << " and received " << recv_data << " from " << prev_rank
         << " | Time taken: " << (end_time_blocking - start_time_blocking) << " seconds" << endl;

    MPI_Barrier(MPI_COMM_WORLD); // Synchronize before non-blocking

    // Non-Blocking Communication
    MPI_Request send_request, recv_request;
    double start_time_nonblocking = MPI_Wtime(); // Start timer

    MPI_Isend(&send_data, 1, MPI_INT, next_rank, 0, MPI_COMM_WORLD, &send_request); // Non-blocking send
    MPI_Irecv(&recv_data, 1, MPI_INT, prev_rank, 0, MPI_COMM_WORLD, &recv_request); // Non-blocking receive

    MPI_Wait(&send_request, MPI_STATUS_IGNORE); // Wait for send to complete
    MPI_Wait(&recv_request, MPI_STATUS_IGNORE); // Wait for receive to complete

    double end_time_nonblocking = MPI_Wtime(); // End timer

    cout << "Non-Blocking: Process " << world_rank << " sent " << send_data << " to " << next_rank
         << " and received " << recv_data << " from " << prev_rank
         << " | Time taken: " << (end_time_nonblocking - start_time_nonblocking) << " seconds" << endl;

    MPI_Finalize();
    return 0;
}
