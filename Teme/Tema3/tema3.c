#include <stdio.h>
#include <mpi.h>

#include "pnm_image_utils.h"

int main(int argc, char** argv)
{
    PNM_IMAGE image, img;
    int rank, retVal;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    retVal = readImage("safe/Putu.pnm", &image);
    readImage("safe/Putu.pnm", &img);

    writeImage(&image, "# muie pgp", "plm.pnm");

    // char* data = calloc(image.header.height * image.header.width, sizeof(*data));
    // memcpy(data, image.data, image.header.height * image.header.width * sizeof(*data));
    float filter[] = {0.f / 9.f, 1.f / 3.f, 0.f / 9.f, 0.f / 3.f, 1.f / 3.f, 0.f / 3.f, 0.f / 9.f, 1.f / 3.f, 0.f / 9.f};

    applyFilter(&image, img.data, filter);

    writeImage(&image, "# muie pgp", "plm_smooth.pnm");

    MPI_Finalize();

    return 0;
}