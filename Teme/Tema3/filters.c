#include "filters.h"

FILTER_STATUS getFilter(double* filter, char* type)
{
    ASSERT(
        filter == NULL || type == NULL,
        ,
        "[getFilter] Received null pointer parameters.\n",
        FILTER_BAD_PARAMS
    );

    int i;

    /**
    * Se scriu valorile efective ale filtrelor care sunt mai mult sau mai putin
    * hardcodate
    */
    if (!strcmp(type, "smooth"))
    {
        for (i = 0; i != 9; ++i)
        {
            filter[i] = 1.0 / 9.0;
        }
    } else if (!strcmp(type, "blur"))
    {
        for (i = 0; i != 9; ++i)
        {
            if (i & 1)
            {
                filter[i] = .0625;
            } else
            {
                if (i == 4)
                {
                    filter[i] = .25;
                } else
                {
                    filter[i] = .125;
                }
            } 
        }
    } else if (!strcmp(type, "sharpen"))
    {
        for (i = 0; i != 9; ++i)
        {
            if (i & 1)
            {
                filter[i] = -2.0 / 3.0;
            } else
            {
                if (i == 4)
                {
                    filter[i] = 11.0 / 3.0;
                } else
                {
                    filter[i] = 0.0;
                }
            } 
        }
    } else if (!strcmp(type, "mean"))
    {
        for (i = 0; i != 9; ++i)
        {
            if (i == 4)
            {
                filter[i] = 9.0;
            } else
            {
                filter[i] = -1.0;
            } 
        }
    } else if (!strcmp(type, "emboss"))
    {
        memset(filter, 0.f, 9 * sizeof(*filter));
        filter[1] = 1.0;
        filter[7] = -1.0;
    } else
    {
        memset(filter, 0.f, 9 * sizeof(*filter));
        filter[4] = 1.0;
    }

    return FILTER_OK;
}