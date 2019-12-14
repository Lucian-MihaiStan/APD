#include "pnm_image_utils.h"

PNM_STATUS readImage(const char* imageFile, PNM_IMAGE* image)
{
    /* Se verifica daca parametrii dati functiei sunt valizi */
    ASSERT(
        imageFile == NULL || image == NULL,
        ,
        "[getImage] Received null pointer parameters.\n",
        PNM_BAD_PARAMS
    );

    /* Se deschide fisierul de input */
    FILE* inputStream = fopen(imageFile, "rt");
    ASSERT(
        inputStream == NULL,
        ,
        "Unable to open image file.\n",
        PNM_IMAGE_NOT_FOUND
    );

    fgets(image->header.format, FORMAT_LENGTH, inputStream);
    ASSERT(
        image->header.format[1] != '5' && image->header.format[1] != '6',
        fclose(inputStream),
        "Unknown image format.\n",
        PNM_UNKNOWN_FORMAT;
    );

    char title[MAX_TITLE_LENGTH];
    int stride, numBytes, offsetData, rowWidth;
    int i, retval;

    /* Se citesc metadatele imaginii */
    fgets(title, MAX_TITLE_LENGTH, inputStream);
    fscanf(inputStream, "%d %d\n", &image->header.height, &image->header.width);  // de ce nu vrea hu?
    fscanf(inputStream, "%hhu\n", &image->header.maxVal);

    /* O imagine RGB va avea o latime de 3 ori mai mare */
    if (image->header.format[1] == '6')
    {
        stride = 3;
        image->header.width *= stride;
    } else
    {
        stride = 1;
    }

    rowWidth    = image->header.width + 2 * stride;
    numBytes    = rowWidth * (image->header.height + 2);
    offsetData  = rowWidth + stride;

    /* Se aloca memoria in care se va retine imaginea in forma liniarizata */
    image->data = calloc(numBytes, sizeof(*image->data));
    ASSERT(
        image->data == NULL,
        fclose(inputStream),
        "Unable to allocate memory for the image.\n",
        PNM_NO_MEMORY;
    );

    /* Se citesc datele ce constituie pixelii imaginii linie cu linie*/
    for (i = 0; i != image->header.height; ++i, offsetData += rowWidth)
    {
        retval = fread(
            image->data + offsetData,
            sizeof(*image->data),
            image->header.width,
            inputStream
        );
        ASSERT(
            retval != image->header.width,
            fclose(inputStream); free(image->data),
            "Unable to read image file data.\n",
            PNM_NO_DATA;
        );
    }

    fclose(inputStream);
    image->header.width     = rowWidth;
    image->header.height    += 2;

    return PNM_OK;
}

PNM_STATUS writeImage(
    PNM_IMAGE* image,
    const char* title,
    const char* outputFile
)
{
    /* Verificare ca parametrii sa nu fie nuli */
    ASSERT(
        image == NULL || title == NULL || outputFile == NULL,
        ,
        "[writeImage] Received null pointer parameters.\n",
        PNM_BAD_PARAMS
    );

    /* Se deschide fisierul de iesire */
    FILE* outputStream = fopen(outputFile, "w");
    ASSERT(
        outputStream == NULL,
        ,
        "Unable to open output file.\n",
        PNM_FILE_NOT_FOUND
    );

    int stride, offsetData, rowWidth, maxRow;
    int i, retval;

    /* O imagine RGB va avea o latime de 3 ori mai mare */
    if (image->header.format[1] == '6')
    {
        stride = 3;
    } else
    {
        stride = 1;
    }

    rowWidth    = image->header.width - 2 * stride;
    offsetData  = image->header.width + stride;
    maxRow      = image->header.height - 2;

    /* Se scrie antetul imaginii */
    fprintf(
        outputStream,
        "%s%s\n%d %d\n%d\n",
        image->header.format,
        title,
        image->header.height - 2,
        rowWidth / stride,
        image->header.maxVal
    );

    /* Se scriu datele ce constituie pixelii imaginii linie cu linie*/
    for (i = 0; i != maxRow; ++i, offsetData += image->header.width)
    {
        retval = fwrite(
            image->data + offsetData,
            sizeof(*image->data),
            rowWidth,
            outputStream
        );
        ASSERT(
            retval != rowWidth,
            fclose(outputStream); free(image->data),
            "Unable to write image data to file.\n",
            PNM_NO_DATA;
        );
    }    

    fclose(outputStream);

    return PNM_OK;    
}

static inline float applyConvolution(
    const char* inputData,
    const float* filter,
    const int pos,
    const int width,
    const int stride,
    const uint8_t maxVal
)
{
    float sum = 0;

    sum += inputData[pos - width - stride]  * filter[0];
    sum += inputData[pos - width]           * filter[1];
    sum += inputData[pos - width + stride]  * filter[2];
    sum += inputData[pos - stride]          * filter[3];
    sum += inputData[pos]                   * filter[4];
    sum += inputData[pos + stride]          * filter[5];
    sum += inputData[pos + width - stride]  * filter[6];
    sum += inputData[pos + width]           * filter[7];
    sum += inputData[pos + width + stride]  * filter[8];

    sum = round(sum);
    sum = sum > maxVal ? maxVal : sum;

    return sum;
}

PNM_STATUS applyFilter(
    PNM_IMAGE* outputImage,
    const uint8_t* inputData,
    const float* filter
)
{
    ASSERT(
        inputData == NULL || outputImage == NULL || filter == NULL,
        ,
        "Received null pointer parameters.\n",
        PNM_BAD_PARAMS
    );

    int stride, rowWidth, maxRow, rowOffset;
    int i, j, pos;

    /* O imagine RGB va avea o latime de 3 ori mai mare */
    if (outputImage->header.format[1] == '6')
    {
        stride = 3;
    } else
    {
        stride = 1;
    }

    rowWidth    = outputImage->header.width - 2 * stride;
    maxRow      = outputImage->header.height - 1;

    for (i = 1; i != maxRow; ++i)
    {
        for (j = 1; j <= rowWidth; ++j)
        {
            pos = i * outputImage->header.width + j;

            outputImage->data[pos] = (uint8_t)applyConvolution(
                inputData,
                filter,
                pos,
                outputImage->header.width,
                stride,
                outputImage->header.maxVal
            );
        }
    }

    return PNM_OK;
}