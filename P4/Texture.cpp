#include <stdio.h>
#include "Texture.h"

bool Texture::Load(const char* filename) {
    if (!LoadBMP(filename))
        return false;

    glGenTextures(1, &index_);
    glBindTexture(GL_TEXTURE_2D, index_);
    
    // Scale linearly when the image size is not equal to the texture size.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    
    // 2d texture, level of detail 0 (normal), 3 components (red, green, blue),
    // x size from image, y size from image, border 0 (normal), rgb color data,
    // unsigned byte data, and finally the data itself.
    glTexImage2D(GL_TEXTURE_2D, 0, 3, sizeX, sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE,
                 data);
    return true;
}

// quick and dirty bitmap loader...for 24 bit bitmaps with 1 plane only.  
// See http://www.dcs.ed.ac.uk/~mxr/gfx/2d/BMP.txt for more info.
bool Texture::LoadBMP(const char *filename) {
    FILE *file;
    unsigned long size;                 // size of the image in bytes.
    unsigned long i;                    // standard counter.
    unsigned short int planes;          // number of planes in image (must be 1) 
    unsigned short int bpp;             // number of bits per pixel (must be 24)
    char temp;                          // temporary color storage for bgr-rgb conversion.
    
    // make sure the file is there.
    if ((file = fopen(filename, "rb"))==NULL)
    {
        // printf("File Not Found : %s\n",filename);
        return false;
    }
    
    // seek through the bmp header, up to the width/height:
    fseek(file, 18, SEEK_CUR);
    
    // read the width
    if ((i = fread(&sizeX, 4, 1, file)) != 1) {
        // printf("Error reading width from %s.\n", filename);
        return false;
    }
    // printf("Width of %s: %lu\n", filename, sizeX);
    
    // read the height 
    if ((i = fread(&sizeY, 4, 1, file)) != 1) {
        // printf("Error reading height from %s.\n", filename);
        return false;
    }
    // printf("Height of %s: %lu\n", filename, sizeY);
    
    // calculate the size (assuming 24 bits or 3 bytes per pixel).
    size = sizeX * sizeY * 3;
    
    // read the planes
    if ((fread(&planes, 2, 1, file)) != 1) {
        printf("Error reading planes from %s.\n", filename);
        return false;
    }
    if (planes != 1) {
        printf("Planes from %s is not 1: %u\n", filename, planes);
        return false;
    }
    
    // read the bpp
    if ((i = fread(&bpp, 2, 1, file)) != 1) {
        printf("Error reading bpp from %s.\n", filename);
        return false;
    }
    if (bpp != 24) {
        printf("Bpp from %s is not 24: %u\n", filename, bpp);
        return false;
    }
	
    // seek past the rest of the bitmap header.
    fseek(file, 24, SEEK_CUR);
    
    // read the data. 
    data = (char *) malloc(size);
    if (data == NULL) {
        // printf("Error allocating memory for color-corrected image data");
        return false;	
    }
    
    if ((i = fread(data, size, 1, file)) != 1) {
        // printf("Error reading image data from %s.\n", filename);
        return false;
    }
    
    for (i = 0; i < size; i += 3) { // reverse all of the colors. (bgr -> rgb)
        temp = data[i];
        data[i] = data[i + 2];
        data[i + 2] = temp;
    }
    
    return true;
}
