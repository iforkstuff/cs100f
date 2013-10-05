#include "Geometry.h"

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


void Vertex::Draw() const {
    if (has_col_)
        glColor3f(col_.x, col_.y, col_.z);
    glVertex3f(pos_.x, pos_.y, pos_.z);
}

Triangle::Triangle(const Vertex& v1, const Vertex& v2,
                   const Vertex& v3) {
    SetVertices(v1, v2, v3); 
}

void Triangle::SetVertices(const Vertex& v1, const Vertex& v2,
                           const Vertex& v3) {
    verts_[0] = v1;
    verts_[1] = v2;
    verts_[2] = v3;
}

void Triangle::Draw() const {
    verts_[0].Draw();
    verts_[1].Draw();
    verts_[2].Draw();
}

// Create and initialize triangles for a cube
void Model::CreateBox(float xsize, float ysize, float zsize) {
    vec3 colors[6];
    vec3 points[8];
    Vertex vertices[8];

    float min = 0.25;
    num_tris_ = 12;
    tris_ = new Triangle[num_tris_];

    /* The points on the cube go like this:
     *   6--7
     *  /  /|
     * 2--3 |
     * | 4| 5
     * |  |/
     * 0--1
     */

    // Set the six possible colors.
    colors[0] = vec3(1, 0, 0); // Red
    colors[1] = vec3(0, 1, 0); // Green
    colors[2] = vec3(0, 0, 1); // Blue
    colors[3] = vec3(1, 1, 0); // Yellow
    colors[4] = vec3(0, 1, 1); // Cyan
    colors[5] = vec3(1, 0, 1); // Purple

    // Create the eight possible points in 3D.
    points[0].set(min, min, min);
    points[1].set(xsize, min, min);
    points[2].set(min, ysize, min);
    points[3].set(xsize, ysize, min);
    points[4].set(min, min, zsize);
    points[5].set(xsize, min, zsize);
    points[6].set(min, ysize, zsize);
    points[7].set(xsize, ysize, zsize);

    // Combine the eight points with different colors to create the eight
    // possible vertices.
    vertices[0].SetPosCol(points[0], colors[0]);
    vertices[1].SetPosCol(points[1], colors[0]);
    vertices[2].SetPosCol(points[2], colors[1]);
    vertices[3].SetPosCol(points[3], colors[1]);
    vertices[4].SetPosCol(points[4], colors[2]);
    vertices[5].SetPosCol(points[5], colors[2]);
    vertices[6].SetPosCol(points[6], colors[3]);
    vertices[7].SetPosCol(points[7], colors[3]);

    // Create the twelve trianges of the cube using different combinations of
    // the eight vertices.
    tris_[0].SetVertices(vertices[2], vertices[1], vertices[0]); // Back
    tris_[1].SetVertices(vertices[1], vertices[2], vertices[3]);  
    tris_[2].SetVertices(vertices[5], vertices[4], vertices[0]); // Top
    tris_[3].SetVertices(vertices[0], vertices[1], vertices[5]);
    tris_[4].SetVertices(vertices[4], vertices[2], vertices[0]); // Left
    tris_[5].SetVertices(vertices[2], vertices[4], vertices[6]);
    tris_[6].SetVertices(vertices[1], vertices[3], vertices[5]); // Right
    tris_[7].SetVertices(vertices[7], vertices[5], vertices[3]);
    tris_[8].SetVertices(vertices[2], vertices[6], vertices[7]); // Bottom
    tris_[9].SetVertices(vertices[7], vertices[3], vertices[2]);
    tris_[10].SetVertices(vertices[4], vertices[5], vertices[6]); // Front
    tris_[11].SetVertices(vertices[7], vertices[6], vertices[5]);
}

void Model::Draw() const {
    glBegin(GL_TRIANGLES);
    for(int i = 0; i < num_tris_; i++)
        tris_[i].Draw();
    glEnd ();
}

// This code was adapted from OpenGL's glutSolidCube() function.
// (c) Copyright 1993, Silicon Graphics, Inc. (RIP)
void Model::DrawCube(float size, int tex_index) {
    static const GLfloat n[6][3] = {
        {-1.0, 0.0, 0.0},
        {0.0, 1.0, 0.0},
        {1.0, 0.0, 0.0},
        {0.0, -1.0, 0.0},
        {0.0, 0.0, 1.0},
        {0.0, 0.0, -1.0}
    };
    static const GLint faces[6][4] = {
        {0, 1, 2, 3},
        {3, 2, 6, 7},
        {7, 6, 5, 4},
        {4, 5, 1, 0},
        {5, 6, 2, 1},
        {7, 4, 0, 3}
    };
    GLfloat v[8][3];
    GLint i;

    v[0][0] = v[1][0] = v[2][0] = v[3][0] = -size / 2;
    v[4][0] = v[5][0] = v[6][0] = v[7][0] = size / 2;
    v[0][1] = v[1][1] = v[4][1] = v[5][1] = -size / 2;
    v[2][1] = v[3][1] = v[6][1] = v[7][1] = size / 2;
    v[0][2] = v[3][2] = v[4][2] = v[7][2] = -size / 2;
    v[1][2] = v[2][2] = v[5][2] = v[6][2] = size / 2;

    if (tex_index != 0)
        glBindTexture(GL_TEXTURE_2D, tex_index);

    for (i = 5; i >= 0; i--) {
        glBegin(GL_QUADS);
        glNormal3fv(&n[i][0]);
        glTexCoord2f(0.0f, 0.0f);  glVertex3fv(&v[faces[i][0]][0]);
        glTexCoord2f(1.0f, 0.0f);  glVertex3fv(&v[faces[i][1]][0]);
        glTexCoord2f(1.0f, 1.0f);  glVertex3fv(&v[faces[i][2]][0]);
        glTexCoord2f(0.0f, 1.0f);  glVertex3fv(&v[faces[i][3]][0]);
        glEnd();
    }
}
