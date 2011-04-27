#include "Loaders.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "png.h"

using namespace ThoseMagnificentMen;

struct PngBuffer
{
    PngBuffer(unsigned char *data) : Data(data), Position(0) { }

    unsigned char *Data;
    unsigned int Position;
};

void ReadData(png_structp pngPtr, png_bytep data, png_size_t length)
{
    PngBuffer *buffer = reinterpret_cast<PngBuffer *>(png_get_io_ptr(pngPtr));
    memcpy(reinterpret_cast<unsigned char *>(data), buffer->Data + buffer->Position, length);
    buffer->Position += length;
}

GLuint Loaders::LoadTexture(int resourceId)
{
    HRSRC resourceBlock = ::FindResource(NULL, MAKEINTRESOURCE(resourceId), RT_RCDATA);
    HGLOBAL resourceHandle = ::LoadResource(NULL, resourceBlock);
    unsigned char *resourceData = reinterpret_cast<unsigned char *>(::LockResource(resourceHandle));

    png_structp pngPtr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    png_infop infoPtr = png_create_info_struct(pngPtr);
    PngBuffer buffer(resourceData);
    png_set_read_fn(pngPtr, reinterpret_cast<voidp>(&buffer), ReadData);
    
    png_read_info(pngPtr, infoPtr);

    unsigned int width = infoPtr->width;
    unsigned int height = infoPtr->height;
    short bitDepth = infoPtr->bit_depth;
    short channels = infoPtr->channels;

    unsigned char *imageData = new unsigned char[width * height * bitDepth * channels / 8];
    unsigned char **rowPointers = new unsigned char *[height];

    for (unsigned int i = 0; i < height; i ++)
        rowPointers[i] = &(imageData[i * width * bitDepth * channels / 8]);

    png_read_image(pngPtr, (png_bytepp)rowPointers);

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    GLenum error = ::glGetError();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
    error = ::glGetError();

    delete [] imageData;
    delete [] rowPointers;

    return texture;
}
