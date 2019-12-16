#ifndef IMAGE_PROCESSING_H
#define IMAGE_PROCESSING_H

#include <mpi.h>

#include "pnm_image_utils.h"
#include "filters.h"

#define MASTER 			(0)

/**
*	Imparte imaginea de la procesul MASTER la toate celelalte si aplica filtrele
*	date pe aceasta.
*	La final, aduna imaginile tuturor proceselor in cea a procesulul MASTER.
*/
int processImage(
    PNM_IMAGE* image,
    int rank,
    int numProc,
    int argc,
    char** argv
);

#endif