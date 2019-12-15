#include "filters.h"

FILTER_STATUS getFilter(float* filter, char* type)
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
            filter[i] = 1.f / 9.f;
        }
    } else if (!strcmp(type, "blur"))
    {
        for (i = 0; i != 9; ++i)
        {
            if (i & 1)
            {
                filter[i] = .0625f;
            } else
            {
                if (i == 4)
                {
                    filter[i] = .25f;
                } else
                {
                    filter[i] = .125f;
                }
            } 
        }
    } else if (!strcmp(type, "sharpen"))
    {
        for (i = 0; i != 9; ++i)
        {
            if (i & 1)
            {
                filter[i] = -2.f / 3.f;
            } else
            {
                if (i == 4)
                {
                    filter[i] = 11.f / 3.f;
                } else
                {
                    filter[i] = 0.f;
                }
            } 
        }
    } else if (!strcmp(type, "mean"))
    {
        for (i = 0; i != 9; ++i)
        {
            if (i == 4)
            {
                filter[i] = 9.f;
            } else
            {
                filter[i] = -1.f;
            } 
        }
    } else if (!strcmp(type, "emboss"))
    {
        memset(filter, 0.f, 9 * sizeof(*filter));
        filter[1] = -1.f;
        filter[7] = 1.f;
    } else
    {
        memset(filter, 0.f, 9 * sizeof(*filter));
        filter[4] = 1.f;
    }

    return FILTER_OK;
}