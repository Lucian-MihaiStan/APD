#include <stdio.h>
#include <math.h>
#include <mpi.h>

#include "pnm_image_utils.h"
#include "filters.h"

#define MIN(x, y)       (((x) < (y)) ? (x) : (y))

#define MASTER          (0)

int getDataFromMaster(PNM_IMAGE* image, int rank, int numProc)
{
    int initialHeight, numBytes, i;

    int start = rank * ceil((double)image->height / numProc);
    int end   = MIN(
                    image->height,
                    (rank + 1) * ceil((double)image->height / numProc)
                );

    if (rank != MASTER)
    {
        --start;
    }
    if (rank != numProc - 1)
    {
        ++end;
    }

    initialHeight   = image->height;
    image->height   = end - start;
    numBytes        = image->height * image->width;

    if (rank != MASTER)
    {
        image->data = malloc(numBytes * sizeof(*image->data));

        MPI_Recv(
            image->data,
            numBytes,
            MPI_CHAR,
            MASTER,
            MPI_ANY_TAG,
            MPI_COMM_WORLD,
            MPI_STATUS_IGNORE
        );

    } else if (numProc > 1)
    {
        --numProc;

        /* Se trimit datele tuturor proceselor mai putin ultimului */
        for (i = 1; i != numProc; ++i)
        {
            MPI_Send(
                image->data + i * numBytes,
                numBytes,
                MPI_CHAR,
                i,
                MPI_TAG_UB,
                MPI_COMM_WORLD
            );
        }

        /**
        * Ultimul proces va avea in general mai putine date, care i se trimit
        * separat
        */
        MPI_Send(
            image->data + i * numBytes,
            initialHeight * image->width - i * numBytes,
            MPI_CHAR,
            i,
            MPI_TAG_UB,
            MPI_COMM_WORLD
        );
    }

    return numBytes;
}

PNM_STATUS processImage(
    PNM_IMAGE* image,
    int rank,
    int numProc,
    int argc,
    char** argv
)
{
    int retVal, i, numBytes, initialHeight;
    char* aux;
    char* data;
    float filter[9]; 

    initialHeight   = image->height;
    numBytes        = getDataFromMaster(image, rank, numProc);
    data            = malloc(numBytes * sizeof(*data));

    for (int i = 3; i != argc; ++i)
    {
        aux         = data;
        data        = image->data;
        image->data = aux;

        retVal = getFilter(filter, argv[i]);
        ASSERT(retVal != FILTER_OK, free(data); free(image->data), " ", retVal);

        retVal = applyFilter(image, data, filter);
        ASSERT(retVal != PNM_OK, free(data); free(image->data), " ", retVal);
    }

    free(data);

    // TODO: aduna datele

    return PNM_OK;
}

int main(int argc, char** argv)
{
    PNM_IMAGE image;
    int rank, retVal, numProc;
    int start, end, initialHeight;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);

    if (rank == MASTER)
    {
        retVal = readImage(argv[1], &image, TRUE);
        ASSERT(retVal != PNM_OK, MPI_Finalize(), " ", retVal);

        initialHeight = image.height;
    } else
    {
        retVal = readImage(argv[1], &image, FALSE);
        ASSERT(retVal != PNM_OK, MPI_Finalize(), " ", retVal);
    }

    retVal = processImage(&image, rank, numProc, argc, argv);
    ASSERT(retVal != PNM_OK, MPI_Finalize(), " ", retVal);

    if (rank == MASTER)
    {
        retVal = writeImage(
            &image,
            "# Created by Teodor-Stefan Dutu, using MPI :D",
            argv[2]
        );
        ASSERT(
            retVal != PNM_OK,
            free(image.data);
            MPI_Finalize(),
            " ",
            retVal
        );
    }

    MPI_Finalize();
    free(image.data);

    return 0;
}