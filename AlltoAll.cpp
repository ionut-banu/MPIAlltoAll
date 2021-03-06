// AlltoAll.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "mpi.h"
#include "math.h"
#include "vector"
#define M_LOG2E 1.44269504088896340736 // log2(e)

using namespace std;

int main(int argc, char* argv[])
{
	int rank, size; //I am process RANK and we are a total of SIZE
	MPI_Status status;
	//MPI_Request request;
	MPI_Init(&argc, &argv);

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);	//Generate the RANK in the WORLD
	MPI_Comm_size(MPI_COMM_WORLD, &size);	//Generate the SIZE of the WORLD

	MPI_Comm cube;
	int nDim = log2(size);					//number of dimensions of cartesian grid
	int *processPerDim;						//integer array of size ndims specifying the number of processes in each dimension
	int *period;							//logical array of size ndims specifying whether the grid is periodic (1) or not (0) in each dimension 
	processPerDim = new int[nDim];	
	period = new int[nDim];					
	for (int i = 0; i < nDim; i++) {
		processPerDim[i] = 2;
		period[i] = 1;
	}			
	MPI_Cart_create(MPI_COMM_WORLD, nDim, processPerDim, period, true, &cube);	//create hypercube

	int source, *neighbors, result[128], rec[128];
	neighbors =  new int(nDim);
	//result = new int(size);
	//rec = new int(size);
	// get the addresses of neighbor processors
	printf("My rank in the world is: %d. My neighbors are: ", rank);
	for (int i = 0; i < nDim; i++) {
		MPI_Cart_shift(cube, i, 1, &source, &neighbors[i]);
		printf("%d ", neighbors[i]);
	}

	for (int i = 0; i < size; i++) {
		result[i] = -1;
		rec[i] = -1;
	}
	result[rank] = rank;

	for (int i = 0; i < nDim; i++) {
		for (int j = 0; j < nDim; j++) {
			int t = i == 0 ? 1 : pow(2, i);
			if ((rank ^ neighbors[j]) == t) {
				//printf("Sending %d to %d--", rank, neighbors[j]);
				MPI_Send(result, 128, MPI_INT, neighbors[j], 0, MPI_COMM_WORLD);
				//printf("Sent %d to %d--", rank, neighbors[j]);
				printf("Receiving from %d: ", neighbors[j]);
				MPI_Recv(&rec, 128, MPI_INT, neighbors[j], 0, MPI_COMM_WORLD, &status);
				for (int m = 0; m < size; m++) {
					if (rec[m] != -1) {
						result[rec[m]] = rec[m];
						printf("%d, ", rec[m]);
					}
				}
				break;
			}
		}
	}

	printf("ID: %d  ", rank);
	for (int i = 0; i < size; i++) {
		printf("%d ", result[i]);
	}
	
	
	MPI_Finalize();
}


const int log2(const int x) {
	return log(x) * M_LOG2E;
}

/*MPI_Send(&result, 1, MPI_INT, neighbors[j], 0, MPI_COMM_WORLD);
MPI_Recv(&rec, 1, MPI_INT, neighbors[j], 0, MPI_COMM_WORLD, &status);
for (int t = 0; t < size; t++) {
if (rec[t] != -1 && result[t] != -1) {
result[t] = rec[t];
}

MPI_Irecv(&rec, 1, MPI_INT, neighbors[j], 0, MPI_COMM_WORLD, &request);
if (result[rec] == -1) {
result[rec] = rec;
}

for (int m = 0; m < size; m++) {
if (result[m] != -1) {
//printf("Sending %d to %d--", rank, neighbors[j]);
MPI_Send(&result[m], 1, MPI_INT, neighbors[j], 0, MPI_COMM_WORLD);
//printf("Sent %d to %d--", rank, neighbors[j]);
//printf("Receiving from %d--", neighbors[j]);
MPI_Recv(&rec, 1, MPI_INT, neighbors[j], 0, MPI_COMM_WORLD, &status);
printf("Received %d from %d--", rec, neighbors[j]);
//if (result[rec] == -1) {
//	result[rec] = -2;
//}
}
}


for (int m = 0; m < size; m++) {
if (result[m] != -1) {
//printf("Sending %d to %d--", rank, neighbors[j]);
MPI_Send(&result[m], 1, MPI_INT, neighbors[j], 0, MPI_COMM_WORLD);
//printf("Sent %d to %d--", rank, neighbors[j]);
//printf("Receiving from %d--", neighbors[j]);
MPI_Recv(&rec, 1, MPI_INT, neighbors[j], 0, MPI_COMM_WORLD, &status);
printf("Received %d from %d--", rec, neighbors[j]);
//if (result[rec] == -1) {
//	result[rec] = 5;
//}
}
}

printf("|%d ^ %d = %d (%d)|", rank, neighbors[j], rank ^ neighbors[j], i);
//printf("|%d ^ %d = %d|", rank, neighbors[j], rank ^ neighbors[j]);}*/
