#include "filters.h"

void getFilter(float* filter, FILTER_TYPE type)
{
    int i;

    /**
    * Se scriu valorile efective ale filtrelor care sunt mai mult sau mai putin
    * hardcodate
    */
    switch(type)
    {
        case SMOOTHING:
            for (i = 0; i != 9; ++i)
            {
                filter[i] = 1.f / 9.f;
            }

            break;

        case GAUSSIAN_BLUR:
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

            break;

        case SHARPEN:
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

            break;

        case MEAN_REMOVAL:
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

            break;

        case EMBOSS:
            memset(filter, 0.f, 9 * sizeof(*filter));
            filter[1] = -1.f;
            filter[7] = 1.f;

            break;

        default:
            memset(filter, 0.f, 9 * sizeof(*filter));
            filter[4] = 1.f;

            break;
    }
}