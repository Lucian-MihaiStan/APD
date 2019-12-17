#include "image_processing.h"

/**
*   Aplica filtrele date imaginii primite tot ca parametru.
*   Returneaza daca toate operatiile s-au terminat cu succes sau codul de eroare
*   al operatiei care a esuat.
*/
static PNM_STATUS applyFilters(
    PNM_IMAGE* image,
    int rank,
    int numProc,
    int usedBytes,
    int argc,
    char** argv
)
{
    ASSERT(
        image == NULL || argv == NULL,
        ,
        "[applyFilters] Received null pointer parameters.\n",
        PNM_BAD_PARAMS
    );

    int retVal, i;
    uint8_t* aux;
    float filter[9];

    int lastLinePos = image->width * (image->height - 2);
    uint8_t* data   = malloc(usedBytes * sizeof(*data));
    ASSERT(
        data == NULL,
        ,
        "Unable to allocate memory for additional data.\n",
        PNM_NO_MEMORY
    );

    for (i = 3; i != argc; ++i)
    {   
        /* Procesele isi trimit prima si ultima linie */
        if (rank != numProc - 1)
        {
            MPI_Send(
                image->data + lastLinePos,
                image->width,
                MPI_CHAR,
                rank + 1,
                MPI_TAG_UB,
                MPI_COMM_WORLD
            );
        }

        if (rank != MASTER)
        {
            MPI_Recv(
                image->data,
                image->width,
                MPI_CHAR,
                rank - 1,
                MPI_ANY_TAG,
                MPI_COMM_WORLD,
                MPI_STATUS_IGNORE
            );

            MPI_Send(
                image->data + image->width,
                image->width,
                MPI_CHAR,
                rank - 1,
                MPI_TAG_UB,
                MPI_COMM_WORLD
            );
        }

        if (rank != numProc - 1)
        {
            MPI_Recv(
                image->data + lastLinePos + image->width,
                image->width,
                MPI_CHAR,
                rank + 1,
                MPI_ANY_TAG,
                MPI_COMM_WORLD,
                MPI_STATUS_IGNORE
            );
        }

        /* Dupa ce datele au fost actualizate se aplica filtrul */
        aux         = data;
        data        = image->data;
        image->data = aux;

        retVal = getFilter(filter, argv[i]);
        ASSERT(retVal != FILTER_OK, free(data), " ", retVal);

        retVal = applyFilter(image, data, filter);
        ASSERT(retVal != PNM_OK, free(data), " ", retVal);
    }

    free(data);

    return PNM_OK;
}

/**
*   MASTER primeste datele de la toate celelalte procese.
*/
static void gatherData(
    PNM_IMAGE* image,
    int rank,
    int numProc,
    int chunkSize,
    int numBytesGather
)
{
    int i;

    if (rank != MASTER)
    {
        /* Procesele trimit datele lor prelucrate catre MASTER */
        MPI_Send(
            image->data + image->width,
            numBytesGather,
            MPI_CHAR,
            MASTER,
            MPI_TAG_UB,
            MPI_COMM_WORLD
        );

    } else if (numProc > 1)
    {
        --numProc;

        /* Se primesc datele tuturor proceselor mai putin de la ultimul */
        for (i = 1; i != numProc; ++i)
        {
            MPI_Recv(
                image->data + i * chunkSize + image->width,
                chunkSize,
                MPI_CHAR,
                i,
                MPI_TAG_UB,
                MPI_COMM_WORLD,
                MPI_STATUS_IGNORE
            );
        }

        /**
        * Ultimul proces va avea in general mai putine date, pe care MASTER le
        * primeste separat
        */
        MPI_Recv(
            image->data + numProc * chunkSize + image->width,
            image->height * image->width - numProc * chunkSize - image->width,
            MPI_CHAR,
            numProc,
            MPI_TAG_UB,
            MPI_COMM_WORLD,
            MPI_STATUS_IGNORE
        );
    }
}

PNM_STATUS processImage(
    PNM_IMAGE* image,
    int rank,
    int numProc,
    int argc,
    char** argv
)
{
    ASSERT(
        image == NULL || argv == NULL,
        ,
        "[processImage] Received null pointer parameters.\n",
        PNM_BAD_PARAMS
    );

    int numBytesGather, usedBytes, retVal;

    /**
    * Fiecare proces calculeaza portiunea de imagine pe care va aplica
    * filtrele
    */
    int linesChunk      = ceil((double)(image->height - 2) / numProc);
    int chunkSize       = linesChunk * image->width;
    int initialHeight   = image->height;
    int start           = rank * linesChunk;
    int end             = MIN(image->height, (rank + 1) * linesChunk + 2);

    /* Se calculeaza parametrii necesari procesarii */
    if (rank != numProc - 1)
    {
        numBytesGather = chunkSize;
    } else
    {
        numBytesGather = initialHeight * image->width
                         - (numProc - 1) * chunkSize
                         - image->width;
    }

    image->height = end - start;

    /* Se calculeaza numarul de octeti care vor fi folositi de fiecare proces */
    if (rank == MASTER)
    {
        usedBytes = initialHeight * image->width;
    } else
    {
        usedBytes = image->height * image->width;
    }

    /* Se face procesarea efectiva */
    retVal = applyFilters(image, rank, numProc, usedBytes, argc, argv);
    ASSERT(retVal != PNM_OK, , " ", retVal);

    image->height = initialHeight;

    /* Se primesc datele procesate de la toate procesele */ 
    gatherData(image, rank, numProc, chunkSize, numBytesGather);

    return PNM_OK;
}