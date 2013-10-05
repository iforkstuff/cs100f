#ifndef TEXTURE_H
#define TEXTURE_H

#include <stdlib.h>

#if defined(__APPLE__)
#include <GLUT/glut.h>
#include <OpenGL/glext.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#elif defined(__LINUX__)
#include <GL/glut.h>
#else
#include "glut.h"
#endif

// Disable stupid Windows warnings.
#pragma warning( disable : 4996)

/* Image type - contains height, width, and data */
class Texture {
public:
    bool Load(const char* filename);
    int index() const { return index_; }
    void SetIndex(GLuint index) { index_ = index; }
    
protected:
    unsigned long sizeX;
    unsigned long sizeY;
    char *data;
    GLuint index_;    
    bool LoadBMP(const char* filename);
};

#endif

