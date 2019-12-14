#include <stdio.h>
#include <mpi.h>

#include "pnm_image_utils.h"
#include "filters.h"

#define MASTER          (0)

int main(int argc, char** argv)
{
    PNM_IMAGE image;
    int rank, retVal;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == MASTER)
    {
        retVal = readImage(argv[1], &image);
        ASSERT(
            retVal != PNM_OK,
            ,
            " ",
            retVal;
        );

        // TODO: calculeaza ce trimiti cui si trimite
    } else
    {
        // TODO: primeste de la toata lumea
    }

    char* data = malloc(image.height * image.width * sizeof(*data));
    char* aux;
    float filter[9];

    for (int i = 3; i != argc; ++i)
    {
        aux = data;
        data = image.data;
        image.data = aux;

        if (!strcmp(argv[i], "smooth"))
        {
            getFilter(filter, SMOOTHING);
        } else if (!strcmp(argv[i], "blur"))
        {
            getFilter(filter, GAUSSIAN_BLUR);
        } else if (!strcmp(argv[i], "sharpen"))
        {
            getFilter(filter, SHARPEN);
        } else if (!strcmp(argv[i], "mean"))
        {
            getFilter(filter, MEAN_REMOVAL);
        } else if (!strcmp(argv[i], "emboss"))
        {
            getFilter(filter, EMBOSS);
        } else
        {
            getFilter(filter, -1);
        }

        retVal = applyFilter(&image, data, filter);
        ASSERT(
            retVal != PNM_OK,
            free(data); free(image.data),
            " ",
            retVal;
        );
    }

    if (rank == MASTER)
    {
        // TODO: aduna toate

        retVal = writeImage(&image, "# Created by Teodor-Stefan Dutu, using MPI :D", argv[2]);
        ASSERT(
            retVal != PNM_OK,
            free(data); free(image.data),
            " ",
            retVal;
        );
    } else
    {
        // TODO: trimite ce ai
    }

    MPI_Finalize();
    free(image.data);
    free(data);

    return 0;
}