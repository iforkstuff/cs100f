#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "vec2.h"
#include "vec3.h"

// Represents some point on a polygon.
class Vertex {
public:
    Vertex() : has_col_(false), has_tex_(false) { }
    vec3 pos() { return pos_; }
    vec3 col() { return col_; }
    vec2 tex() { return tex_; }
    const vec3& pos() const { return pos_; }
    const vec3& col() const { return col_; }
    const vec2& tex() const { return tex_; }
    void SetPos(const vec3& pos) { pos_ = pos; }
    void SetCol(const vec3& col) { col_ = col; has_col_ = true; }
    void SetTex(const vec2& tex) { tex_ = tex; has_tex_ = true; }
    void SetPosCol(const vec3& pos, const vec3& col)
        { SetPos(pos); SetCol(col); }
    void SetPosColTex(const vec3& pos, const vec3& col, const vec2& tex)
        { SetPos(pos); SetCol(col); SetTex(tex);  }
    void Draw() const;

private:
    vec3 pos_;
    vec3 col_;
    vec2 tex_;
    bool has_col_;
    bool has_tex_;
};

// Represents a generic 3D triangle.
class Triangle {
public:
    Triangle() { }
    Triangle(const Vertex& v1, const Vertex& v2, const Vertex& v3);
    void SetVertices(const Vertex& v1, const Vertex& v2, const Vertex& v3);
    Vertex& verts(int which) { return verts_[which]; }
    const Vertex& verts(int which) const { return verts_[which]; }
    void Draw() const;

private:
    Vertex verts_[3];
};

// Represents a generic 3D model.
class Model {
public:

    // Draw a cube with texture coordinates.
    static void DrawCube(float size, int tex_index);

    Model() : num_tris_(0), tris_(NULL) { }
    ~Model() { delete tris_; }

    // Renders triangles to initialize this model to the shape of a cube
    void CreateBox(float xsize, float ysize, float zsize);
    void Draw() const;

private:
    int num_tris_;   // Number of triangles in the model.
    Triangle* tris_; // Pointer to any number of Triangle objects.
};

static const vec3 BLACK(0, 0, 0);
static const vec3 WHITE(1, 1, 1);
static const vec3 RED(1, 0, 0);
static const vec3 GREEN(0, 1, 0);
static const vec3 BLUE(0, 0, 1);
static const vec3 YELLOW(1, 1, 0);
static const vec3 PURPLE(1, 0, 1);
static const vec3 CYAN(0, 1, 1);

#endif
