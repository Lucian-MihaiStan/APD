#ifndef PNM_IMAGE_UTILS_H
#define PNM_IMAGE_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define PNM_OK                      (0)
#define PNM_IMAGE_NOT_FOUND         (-1)
#define PNM_NO_MEMORY               (-2)
#define PNM_NO_DATA                 (-3)
#define PNM_UNKNOWN_FORMAT          (-4)
#define PNM_BAD_PARAMS              (-5)

typedef int PNM_STATUS;

typedef struct __attribute__((__packed__)) PNM_IMAGE_HEADER
{
    char format[2];
    uint8_t maxVal;
    uint16_t width, height;
};

typedef struct PNM_IMAGE
{
    char* data;
    PNM_IMAGE_HEADER header;
};

typedef enum LINE_POSITION
{
    BEGINNING,
    END
};

/**
*   Citeste o imagine iar datele sunt puse intr-o structura de tip PNM_IMAGE
*/
PNM_STATUS getImage(const char* imageFile, PNM_IMAGE* image);

/**
*   Bordeaza o imagine cu 0 in-place pentru a usura aplicarea unui filtru.
*   Se presupune ca imaginea are un format corect.
*/
PNM_STATUS padImage(PNM_IMAGE* image);

/**
*   Aplica un filtru unei imagini-input si stocheaza rezultatul filtrarii intr-o
*   imagine-output. Cele doua imagini nu pot fi una si aceeasi.
*   Atat filtrul cat si imaginea sunt liniarizate.
*   Se presupune ca imaginea are un format corect.
*/
PNM_STATUS applyFilter(
    PNM_IMAGE* outputImage,
    PNM_IMAGE* inputImage,
    const char* filter
);

/**
*   Adauga o linie imaginii date ca parametru, fie in fata, fie in spate.
*   Se presupune ca imaginea are un format corect si ca linia are aceeasi lungime
*   ca o linie din imagine.
*/
PNM_STATUS addLine(
    PNM_IMAGE* image,
    char* line,
    LINE_POSITION pos
);

#endif  /* !PNM_IMAGE_UTILS_H */