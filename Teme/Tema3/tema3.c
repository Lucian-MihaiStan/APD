#include <stdio.h>
#include <mpi.h>

#include "pnm_image_utils.h"

int main(int argc, char** argv)
{
    PNM_IMAGE image;
    int rank, retVal;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    retVal = readImage("input_files/PNM/lena.pnm", &image);

    writeImage(&image, "# muie pgp", "plm.pnm");

    char* data = calloc(image.height * image.width, sizeof(*data));
    memcpy(data, image.data, image.height * image.width * sizeof(*data));
    float filter[] = {1.f / 9.f, 1.f / 9.f, 1.f / 9.f, 1.f / 9.f, 1.f / 9.f, 1.f / 3.f, 1.f / 9.f, 1.f / 9.f, 1.f / 9.f};

    applyFilter(&image, data, filter);

    writeImage(&image, "# muie pgp", "plm_smooth.pnm");

    MPI_Finalize();

    return 0;
}