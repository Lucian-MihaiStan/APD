#ifndef GENERAL_UTILS_H
#define GENERAL_UTILS_H

#include <stdio.h>

/**
*   Verifica o conditie si daca aceasta este indeplinita, se efectueaza actiunea
*   data, se afiseaza mesajul dat si returneaza valoarea specificata.
*   E practic un `DIE` modificat.
*/
#define ASSERT(condition, action, msg, retVal)          \
    do                                          		\
    {                                           		\
        if (condition)                          		\
        {                                       		\
        	action;                             		\
            fprintf(stderr, msg);               		\
            return retVal;                      		\
        }                                       		\
    } while (0)

#define MIN(x, y) (((x) < (y)) ? (x) : (y))

#define MAX(x, y) (((x) > (y)) ? (x) : (y))

#define MASTER 			(0)

#endif  /* !GENERAL_UTILS_H */