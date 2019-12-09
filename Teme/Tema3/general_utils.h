#ifndef GENRAL_UTILS_H
#define GENRAL_UTILS_H

/**
    Verifica o conditie si daca aceasta este indeplinita, se efectueaza actiunea
    data, se afiseaza mesajul dat si returneaza valoarea specificata specificata.
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

#endif