#include "ImagePixels.h"
#include <png.h>
#include <stdio.h>
#include <stdlib.h>

U8* atPixelsKind(__ImagePixels imagePixels)
{
  return imagePixels - PIXELS_KIND_PTR_OFFSET;
}

U8* atPixelsChannelCount(__ImagePixels imagePixels)
{
  return imagePixels - PIXELS_CHANNEL_COUNT_PTR_OFFSET;
}

U8* atPixelsChannelBitDepth(__ImagePixels imagePixels)
{
  return imagePixels - PIXELS_CHANNEL_BIT_DEPTH_PTR_OFFSET;
}

U16* atPixelsWidth(__ImagePixels imagePixels)
{
  return (U16*)(imagePixels - PIXELS_WIDTH_PTR_OFFSET);
}

U16* atPixelsHeight(__ImagePixels imagePixels)
{
  return (U16*)(imagePixels - PIXELS_HEIGHT_PTR_OFFSET);
}

__ImagePixels __createImagePixels(
  U8 pixelsKind__,
  U8 pixelsChannelCount__,
  U8 pixelsChannelBitDepth__,
  U16 pixelsWidth,
  U16 pixelsHeight)
{
  U64 pixelsByteCount = pixelsChannelCount__ * (pixelsChannelBitDepth__ / BITS_PER_BYTE) * pixelsWidth * pixelsHeight;
  __ImagePixels imagePixelsResult = (__ImagePixels)malloc(pixelsByteCount + PIXELS_FIELDS_SIZE);
  imagePixelsResult += PIXELS_FIELDS_SIZE;
  *atPixelsKind(imagePixelsResult) = pixelsKind__;
  *atPixelsChannelCount(imagePixelsResult) = pixelsChannelCount__;
  *atPixelsChannelBitDepth(imagePixelsResult) = pixelsChannelBitDepth__;
  *atPixelsWidth(imagePixelsResult) = pixelsWidth;
  *atPixelsHeight(imagePixelsResult) = pixelsHeight;
  return imagePixelsResult;
}

Rgb8bitImagePixels createRgb8bitImagePixels(
  U16 pixelsWidth,
  U16 pixelsHeight)
{
  return __createImagePixels(
    __PixelsKind_RGB_8bit,
    __PixelsChannelCount_RGB,
    __PixelsChannelBitDepth_8,
    pixelsWidth,
    pixelsHeight);
}

// getting weird runtime error when filling pixels for 16bit rgb
// runtime error: store to misaligned address 0x756f9377e807 for type 'U16' (aka 'unsigned short'), which requires 2 byte alignment
// Rgb16bitImagePixels createRgb16bitImagePixels(
//   U16 pixelsWidth,
//   U16 pixelsHeight)
// {
//   return __createImagePixels(
//     __PixelsKind_RGB_16bit,
//     __PixelsChannelCount_RGB,
//     __PixelsChannelBitDepth_16,
//     pixelsWidth,
//     pixelsHeight);
// }

void freeImagePixels(__ImagePixels imagePixels)
{
  free(imagePixels - PIXELS_FIELDS_SIZE);
}

U8 getPixelsKind(__ImagePixels imagePixels)
{
  return *atPixelsKind(imagePixels);
}

U8 getPixelsChannelCount(__ImagePixels imagePixels)
{
  return *atPixelsChannelCount(imagePixels);
}

U8 getPixelsChannelBitDepth(__ImagePixels imagePixels)
{
  return *atPixelsChannelBitDepth(imagePixels);
}

U8 getPixelsChannelByteDepth(__ImagePixels imagePixels)
{
  return getPixelsChannelBitDepth(imagePixels) / BITS_PER_BYTE;
}

U8 getPixelsPixelByteCount(__ImagePixels imagePixels)
{
  return getPixelsChannelCount(imagePixels) * getPixelsChannelByteDepth(imagePixels);
}

U16 getPixelsWidth(__ImagePixels imagePixels)
{
  return *atPixelsWidth(imagePixels);
}

U16 getPixelsHeight(__ImagePixels imagePixels)
{
  return *atPixelsHeight(imagePixels);
}

void __writeImagePixels(
  U64 pngColorType__,
  __ImagePixels imagePixels,
  FILE* outputFilePtr)
{
  U16 pixelsHeight = getPixelsHeight(imagePixels);
  U16 pixelsWidth = getPixelsWidth(imagePixels);
  U8 pixelsPixelByteCount = getPixelsPixelByteCount(imagePixels);
  png_structp pngWriteStruct =
    png_create_write_struct(
      PNG_LIBPNG_VER_STRING,
      NULL,
      NULL,
      NULL);
  png_infop pngInfoPtr = png_create_info_struct(pngWriteStruct);
  png_bytep pixelRowsPtr[pixelsHeight];
  for (int rowIndex = 0; rowIndex < pixelsHeight; rowIndex++)
  {
    pixelRowsPtr[rowIndex] = &(imagePixels[rowIndex * pixelsWidth * pixelsPixelByteCount]);
  }
  png_init_io(
    pngWriteStruct,
    outputFilePtr);
  png_set_IHDR(
    pngWriteStruct,
    pngInfoPtr,
    getPixelsWidth(imagePixels),
    getPixelsHeight(imagePixels),
    getPixelsChannelBitDepth(imagePixels),
    pngColorType__,
    PNG_INTERLACE_NONE,
    PNG_COMPRESSION_TYPE_DEFAULT,
    PNG_FILTER_TYPE_DEFAULT);
  png_write_info(
    pngWriteStruct,
    pngInfoPtr);
  png_write_image(
    pngWriteStruct,
    pixelRowsPtr);
  png_write_end(
    pngWriteStruct,
    NULL);
  png_destroy_write_struct(
    &pngWriteStruct,
    &pngInfoPtr);
}

void writeRgbImagePixels(
  __ImagePixels imagePixels,
  FILE* outputFilePtr)
{
  __writeImagePixels(
    PNG_COLOR_TYPE_RGB,
    imagePixels,
    outputFilePtr);
}
