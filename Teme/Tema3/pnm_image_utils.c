#include "pnm_image_utils.h"

PNM_STATUS getImage(const char* imageFile, PNM_IMAGE* image)
{
    ASSERT(
        imageFile == NULL || image == NULL,
        ,
        "Received null pointer parameters.\n",
        PNM_BAD_PARAMS
    );

    FILE* imageStream = fopen(imageFile, "rt");
    ASSERT(
        imageStream == NULL,
        ,
        "Unable to open image file.\n",
        PNM_IMAGE_NOT_FOUND
    );

    fread(image->header.format, sizeof(image->header[0]), 2, imageStream);
    ASSERT(
        image->header.format[1] != '5' && image->header.format[1] != '6',
        fclose(imageStream),
        "Unknown image format.\n",
        PNM_UNKNOWN_FORMAT;
    );

    fscanf(imageStream, "%hu %hu", &image->header.width, &image->header.height);
    fscanf(imageStream, "%u", &image->header.maxVal);

    int numBytes, retval;

    if (image->header.format[1] == '5')
    {
        numBytes = 1L * image->header.width * image->header.height;
    } else
    {
        numBytes = 1L * image->header.width * image->header.height * 3;
    }

    image->data = malloc(numBytes * sizeof(*image->data));
    ASSERT(
        image->data == NULL,
        fclose(imageStream),
        "Unable to allocate memory for the image.\n",
        PNM_NO_MEMORY;
    );

    retval = fread(image->data, sizeof(*image->data), numBytes, imageStream);
    ASSERT(
        retval < numBytes,
        fclose(imageStream); free(image->data),
        "Unable to read image file data.\n",
        PNM_NO_DATA;
    );

    fclose(imageStream);

    return PNM_OK;
}

PNM_STATUS padImage(PNM_IMAGE* image)
{
    ASSERT(
        image == NULL,
        ,
        "Received null pointer parameter.\n",
        PNM_BAD_PARAMS
    );

    char* newData;
    int stride, numBytes, newNumBytes, lineWidth, offsetNewData, offsetData;

    if (image->header.format[1] == '5')
    {
        stride = 1;
    } else
    {
        stride = 3;
    }

    lineWidth       = image->header.width * stride;
    numBytes        = image->header.width * image->header.height * stride;
    offsetNewData   = 1;
    offsetData      = 0;
    newNumBytes     = (image->header.width + 2) * (image->header.height + 2) * stride;

    newData = calloc(newNumBytes, sizeof(image->data[0]));
    ASSERT(
        newData == NULL,
        ,
        "Unable to allocate memory for the padded image.\n",
        PNM_NO_MEMORY
    );

    for (; offsetData < numBytes;
           offsetData += lineWidth, offsetNewData += lineWidth + 2 * stride)
    {
        memcpy(
            newData + offsetNewData,
            image->data + offsetData,
            lineWidth * sizeof(*newData)
        );
    }

    free(image->data);
    image->data = newData;

    return PNM_OK;
}

static int applyConvolution(
    char* inputData,
    char* filter,
    int pos,
    int offset,
    int stride
)
{
    int sum = 0;

    sum += inputImage->data[pos - offset - stride]  * filter[0];
    sum += inputImage->data[pos - offset]           * filter[1];
    sum += inputImage->data[pos - offset + stride]  * filter[2];
    sum += inputImage->data[pos - stride]           * filter[3];
    sum += inputImage->data[pos]                    * filter[4];
    sum += inputImage->data[pos + stride]           * filter[5];
    sum += inputImage->data[pos + offset - stride]  * filter[6];
    sum += inputImage->data[pos + offset]           * filter[7];
    sum += inputImage->data[pos + offset + stride]  * filter[8];

    return sum;
}

PNM_STATUS applyFilter(
    PNM_IMAGE* outputImage,
    PNM_IMAGE* inputImage,
    const char* filter
)
{
    ASSERT(
        inputImage == NULL || outputImage == NULL || filter == NULL,
        ,
        "Received null pointer parameters.\n",
        PNM_BAD_PARAMS
    );

    memcpy(
        &inputImage->header,
        &outputImage->header,
        sizeof(inputImage->header)
    );

    int i, j, pos, lineWidth, stride, numBytes, offset, sum;

    if (inputImage->header.format[1] == '5')
    {
        stride = 1;
    } else
    {
        stride = 3;
    }

    lineWidth   = (inputImage->header.width + 2) * stride;
    numBytes    = (inputImage->header.width + 2) * (image->header.height + 2)
                  * stride;
    offset      = (inputImage->header.width + 2) * stride;

    for (i = (image->header.width + 3) * stride; i < numBytes; i += offset)
    {
        for (j = 1; j <= lineWidth; ++j)
        {
            pos = i + j;

            outputImage->data[pos] = applyConvolution(
                inputImage,
                filter,
                pos,
                offset,
                stride
            );
        }
    }

    return PNM_OK;
}

PNM_STATUS addLine(
    PNM_IMAGE* image,
    char* line,
    LINE_POSITION pos
)
{
    ASSERT(
        image == NULL || line == NULL || (pos != BEGINNING && pos != END)
        ,
        "Received invalid parameters.\n",
        PNM_BAD_PARAMS
    );

    int stride, lineWidth, numBytes;

    if (inputImage->header.format[1] == '5')
    {
        stride = 1;
    } else
    {
        stride = 3;
    }

    lineWidth   = (inputImage->header.width + 2) * stride;
    numBytes    = inputImage->header.width * image->header.height * stride;



    return PNM_OK;
}