#ifndef FILTERS_H
#define FILTERS_H

#include <string.h>

/* Tipurile de filtre ce vor putea fi aplicate pozelor */
typedef enum
{
    SMOOTHING = 0,
    GAUSSIAN_BLUR,
    SHARPEN,
    MEAN_REMOVAL,
    EMBOSS
} FILTER_TYPE;

/**
*   Pune in stringul dat filtrul corespunzator tipului ofertit.
*   Pune filtrul unitate daca nu se ofera un tip de filtru valid.
* 	Memoria pentru filtru trebuie sa fie deja alocata si sa fie de 9 floaturi.
*/
void getFilter(float* filter, FILTER_TYPE type);

#endif  /* !FILTERS_H */