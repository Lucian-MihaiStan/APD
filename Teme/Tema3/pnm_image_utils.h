#ifndef PNM_IMAGE_UTILS_H
#define PNM_IMAGE_UTILS_H

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

#include "general_utils.h"

#define PNM_OK                      (0)
#define PNM_IMAGE_NOT_FOUND         (-1)
#define PNM_NO_MEMORY               (-2)
#define PNM_NO_DATA                 (-3)
#define PNM_UNKNOWN_FORMAT          (-4)
#define PNM_BAD_PARAMS              (-5)
#define PNM_FILE_NOT_FOUND          (-6)

#define FORMAT_LENGTH               (4)
#define MAX_TITLE_LENGTH            (100)

#define TRUE                        (1)
#define FALSE                       (0)

typedef int PNM_STATUS;

/**
*   Structura in care se va retine imaginea.
*/
typedef struct
{
    char format[FORMAT_LENGTH];
    char title[MAX_TITLE_LENGTH];
    uint8_t maxVal;
    int height, width;
    uint8_t* data;
} PNM_IMAGE;

/**
*   Citeste o imagine iar datele sunt puse intr-o structura de tip PNM_IMAGE.
*   Imaginea este salvata gata bordata.
*   Fiecare proces va citi doar o portiune din imagine.
*/
PNM_STATUS readImage(
    PNM_IMAGE* image,
    const char* imageFile,
    const int rank,
    const int numProc
);

/**
*   Scrie imaginea bordata data ca parametru intr-un fisier.
*   De asemenea, daca parametrul `title` nu este nul, acesta devine nout titlu
*   al imaginii.
*   Atat filtrul cat si imaginea sunt liniarizate.
*   Se presupune ca imaginea are un format corect.
*/
PNM_STATUS writeImage(
    PNM_IMAGE* image,
    const char* outputFile,
    const char* title
);

/**
*   Aplica un filtru unei imagini input si stocheaza rezultatul filtrarii intr-o
*   imagine output. Imaginea de input este reprezentata doar de campul "data"
*   al acesteia.
*   Atat filtrul cat si imaginile sunt liniarizate.
*   Se presupune ca imaginile au un format corect.
*/
PNM_STATUS applyFilter(
    PNM_IMAGE* outputImage,
    const uint8_t* inputData,
    const float* filter
);

#endif  /* !PNM_IMAGE_UTILS_H */