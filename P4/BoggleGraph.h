#ifndef BOGGLE_GRAPH_H
#define BOGGLE_GRAPH_H

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

#include "BogglePlayer.h"
#include "Geometry.h"
#include "Texture.h"
#include <vector>
#include <string>
#include <set>
using std::vector;
using std::string;
using std::set;

class Label;
class BogglePlayer;

// Represents a two-dimensional board of Boggle pieces.
class BoggleGraph {
public:

    // Create a 4x4 graph with random strings on the faces (from the defaults).
    // Also, create the models and textures necessary to draw the board later.
    BoggleGraph();
    
    // Allow the user to see the width of the board.
    int width() const { return width_; }
    
    // Set the player_ to a subclass of BogglePlayer.
    void SetPlayer(BogglePlayer* player) { player_ = player; }

    // Create a (width x width) random board using a set of default faces.
    void Random(int width, int height);

    // Create a custom BoggleGraph using the BogglePlayer interface.
    void Custom();

    // Returns whether the given word is on the board.
    bool IsOnBoard(const string& str, vector<int>* locations);
    
    // Returns whether the given word is in the lexicon.
    bool IsInLexicon(const string& word);

    // Given a minimum word length, populate a vector of strings with all words
    // of at least that length on the board that are also in the lexicon.
    bool GetAllValidWords(int minimum_word_length, vector<string>* words);

    // Load texture images from their files.
    bool LoadTextures(const string& face_bmp,
                      const string& face_shaded_bmp);
    
    // Load the list of valid words from the list.
    bool LoadWords(const string& word_list);
    
    // Highlight the dice in the list.
    void HighlightLocations(const vector<int>& locations);

    // Issue OpenGL commands to draw the dice on the board, and position labels.
    void DrawDice(float zoom_board);
    
    // Draw the labels on the dice.
    void DrawLabels() const;

    // Return an std::string converted to lowercase.
    static string ToLower(const string& str);

private:
    Model cube_;
    vector<Label> dice_labels_;
    vector<string> dice_text_;
    vector<string> dice_lower_;
    vector<bool> highlighted_;
    int width_, height_;
    Texture face_texture_;
    Texture face_texture_shaded_;    
    BogglePlayer* player_;
        
    // Load an image from a RAW file and return the texture index.
    GLuint LoadTextureRAW(int width, int height, const char* filenme, int wrap);

    // Position a label on the face of the current die as we draw the dice.
    void PositionLabel(float cube_size, int which);
};


#endif
