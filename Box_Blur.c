#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <mpi.h>

extern int* imageToMat(char* name, int* dims);
extern void matToImage(char* name, int* mat, int* dims);

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (argc < 2) {
        if (rank == 0) {
            fprintf(stderr, "Usage: %s output_image.jpg\n", argv[0]);
        }
        MPI_Finalize();
        return 1;
    }

    char* output_image = argv[1];
    int *matrix = NULL, *temp = NULL;
    char *name = "image.jpg";
    int *dims = (int*)malloc(2 * sizeof(int));

    if (rank == 0) {
        matrix = imageToMat(name, dims);
    }

    MPI_Bcast(dims, 2, MPI_INT, 0, MPI_COMM_WORLD);

    int height = dims[0];
    int width = dims[1];
    int k = 10;

    int local_height = height / size + (rank < height % size ? 1 : 0);
    int *local_matrix = (int*)malloc(local_height * width * sizeof(int));
    int *local_temp = (int*)malloc(local_height * width * sizeof(int));

    int *sendcounts = (int*)malloc(size * sizeof(int));
    int *displs = (int*)malloc(size * sizeof(int));
    int sum = 0;

    for (int i = 0; i < size; i++) {
        sendcounts[i] = (height / size + (i < height % size ? 1 : 0)) * width;
        displs[i] = sum;
        sum += sendcounts[i];
    }

    MPI_Scatterv(matrix, sendcounts, displs, MPI_INT,
                 local_matrix, local_height * width, MPI_INT,
                 0, MPI_COMM_WORLD);

    int global_i_offset = rank * (height / size) + (rank < height % size ? rank : height % size);

    for (int i = 0; i < local_height; i++) {
        for (int j = 0; j < width; j++) {
            int index = i * width + j;
            int sum = 0, counter = 0;
            for (int u = -k; u <= k; u++) {
                for (int v = -k; v <= k; v++) {
                    int global_i = global_i_offset + i - u;
                    int global_j = j - v;
                    if (global_i < 0 || global_i >= height || global_j < 0 || global_j >= width)
                        continue;
                    int local_i = global_i - global_i_offset;
                    if (local_i >= 0 && local_i < local_height) {
                        int cindex = local_i * width + global_j;
                        sum += local_matrix[cindex];
                        counter++;
                    }
                }
            }
            local_temp[index] = sum / counter;
        }
    }

    if (rank == 0) {
        temp = (int*)malloc(height * width * sizeof(int));
    }

    MPI_Gatherv(local_temp, local_height * width, MPI_INT,
                temp, sendcounts, displs, MPI_INT,
                0, MPI_COMM_WORLD);

    if (rank == 0) {
        matToImage(output_image, temp, dims);
        free(matrix);
        free(temp);
    }

    free(dims);
    free(local_matrix);
    free(local_temp);
    free(sendcounts);
    free(displs);

    MPI_Finalize();
    return 0;
}
