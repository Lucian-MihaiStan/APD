#ifndef FILTERS_H
#define FILTERS_H

#include <string.h>

#include "general_utils.h"

typedef int FILTER_STATUS;

#define FILTER_OK				(0)
#define FILTER_BAD_PARAMS		(-1)

/**
*   Pune in stringul dat filtrul corespunzator tipului oferit.
*   Pune filtrul unitate daca nu se ofera un tip de filtru valid.
* 	Memoria pentru filtru trebuie sa fie deja alocata si sa fie de 9 floaturi.
*/
FILTER_STATUS getFilter(float* filter, char* type);

#endif  /* !FILTERS_H */