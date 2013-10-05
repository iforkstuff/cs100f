#include "BoggleGraph.h"
#include "BoggleGUI.h"
#include "vec4.h"
#include <cstdio>
#include <cstring>
#include <algorithm>

BoggleGraph::BoggleGraph() : player_(NULL) {
    cube_.CreateBox(0.5, 0.5, 0.5);
}

bool BoggleGraph::LoadTextures(const string& face_bmp,
                               const string& face_shaded_bmp) {
    return face_texture_.Load(face_bmp.c_str()) &&
           face_texture_shaded_.Load(face_shaded_bmp.c_str());
}

bool BoggleGraph::LoadWords(const string& word_list_filename) {
    FILE* in = fopen(word_list_filename.c_str(), "r");
    if (in == NULL)
        return false;
    vector<string> word_list;
    char line[1000];
    while (fgets(line, 1000, in) != NULL) {
        line[strlen(line) - 1] = '\0';
        string s = ToLower(string(line));
        word_list.push_back(s);
    }
    fclose(in);
    player_->BuildLexicon(word_list);
    return true;
}

string BoggleGraph::ToLower(const string& str) {
    const char* cstr = str.c_str();
    string out;
    for (size_t i = 0; i < str.length(); ++i)
        out += tolower(cstr[i]);
    return out;
}

bool BoggleGraph::GetAllValidWords(int minimum_word_length,
                                   vector<string>* words) {
    return player_->GetAllValidWords(minimum_word_length, words);
}

bool BoggleGraph::IsOnBoard(const string& str, vector<int>* locations) {
    return player_->IsOnBoard(str, locations);
}

bool BoggleGraph::IsInLexicon(const string& word) {
    return player_->IsInLexicon(word);
}

void BoggleGraph::Random(int width, int height) {

    // The 16 official Boggle dice
    const string DICE[16][6] = {
        { "A", "O", "B", "B", "O", "J" },
        { "W", "H", "G", "E", "E", "N" },
        { "N", "R", "N", "Z", "H", "L" },
        { "N", "A", "E", "A", "G", "E" },
        { "D", "I", "Y", "S", "T", "T" },
        { "I", "E", "S", "T", "S", "O" },
        { "A", "O", "T", "T", "W", "O" },
        { "H", "Qu", "U", "M", "N", "I" },
        { "R", "Y", "T", "L", "T", "E" },
        { "P", "O", "H", "C", "S", "A" },
        { "L", "R", "E", "V", "Y", "D" },
        { "E", "X", "L", "D", "I", "R" },
        { "I", "E", "N", "S", "U", "E" },
        { "S", "F", "F", "K", "A", "P" },
        { "I", "O", "T", "M", "U", "C" },
        { "E", "H", "W", "V", "T", "R" } };
    
    srand(time(NULL));
    width_ = width;
    height_ = height;
    dice_text_.clear();
    highlighted_.clear();
    int max_index = width * height;
    
    // A list of indexes into DICE that we will randomly permute
    vector<int> dielist;
    for (int i = 0; i< 16; ++i) dielist.push_back(i);
    
    // Select and roll max_index dice, which is enough to fill the game.
    for(int die = 0; die < max_index; die++) {
        
        // Permute the die list every 16 draws
        if (die % 16 == 0)
            random_shuffle(dielist.begin(), dielist.end());
        
        // Pick a random face on DICE[dielist[die % 16]]
        int face = rand() % 6;
        dice_text_.push_back(DICE[dielist[die%16]][face]);
        highlighted_.push_back(false);
    }
    player_->SetBoard(dice_text_, width);
}

void BoggleGraph::Custom() {
    player_->GetCustomBoard(&dice_text_, &width_);
    player_->SetBoard(dice_text_, width_);
    height_ = dice_text_.size() / width_;
    highlighted_.clear();
    for (size_t i = 0; i < dice_text_.size(); ++i)
        highlighted_.push_back(false);
}

// Highlight the dice in the list.
void BoggleGraph::HighlightLocations(const vector<int>& locations) {
    for (size_t i = 0; i < highlighted_.size(); ++i)
        highlighted_[i] = false;
    for (vector<int>::const_iterator it = locations.begin();
         it != locations.end(); ++it)
        highlighted_[*it] = true;
}

// Draw the cube faces in the foreground.
void BoggleGraph::DrawDice(float zoom_board) {
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glEnable(GL_POLYGON_SMOOTH);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    
    // Determine where to position the blocks.
    float larger_dimension = width_ > height_ ? width_ : height_;
    vec4 window(-1.0f, 1.1f, 1.0f, -0.9f);
    vec3 shift((window.z - window.x) / larger_dimension,
               (window.w - window.y) / larger_dimension,
               0.35f + zoom_board);
    float cube_size = shift.x * 1; // Controls the spacing between the cubes.
    // float cube_size = shift.x * 0.95;
    
    int difference = abs(width_ - height_);
    if (width_ > height_) window.y += (difference / 2.f) * shift.y;
    if (height_ > width_) window.x += (difference / 2.f) * shift.x;
    
    glTranslatef(window.x - shift.x / 2.f, window.y + shift.y / 2.f, shift.z);
    glRotatef(-30, 1, 0, 0); // Rotate the board about the origin.
    // glRotatef(-30, 1, -0.5, -0.05);
        
    dice_labels_.clear();
    for (int i = 0; i < width_ * height_; i++) {
        glTranslatef(shift.x, 0.0, 0.0);
        PositionLabel(cube_size, i);
        int index = highlighted_[i] ? face_texture_shaded_.index() :
                                      face_texture_.index();
        Model::DrawCube(cube_size, index);
        if (i % width_ == width_ - 1)
            glTranslatef(-shift.x * width_, shift.y, 0);
    }
    glPopMatrix();
}

void BoggleGraph::PositionLabel(float cube_size, int which) {
 
    // Use gluProject to get the current window space coordinates.
    GLdouble model_view[16];
    GLdouble projection[16];
    GLint viewport[4];
    glGetDoublev(GL_MODELVIEW_MATRIX, model_view);    
    glGetDoublev(GL_PROJECTION_MATRIX, projection);
    glGetIntegerv(GL_VIEWPORT, viewport);
    
    GLdouble lower[3], upper[3];
    vec3 pt(-cube_size / 2, -cube_size / 2, cube_size / 2);
    string str = dice_text_[which];
    int x_shifter = str.size() == 1 ? 9 : 5;
    float larger_dimension = width_ > height_ ? width_ : height_;    
    pt.x += x_shifter * (15 / 960.f) * (5.f / larger_dimension);
    pt.y += 5 * (10 / 540.f) * (5.f / larger_dimension);
    gluProject(pt.x, pt.y, pt.z,
               model_view, projection, viewport,
               &lower[0], &lower[1], &lower[2]);
    
    pt.set(cube_size / 2, cube_size / 2, cube_size / 2);
    pt.x -= 4 * (15 / 960.f) * (5.f / larger_dimension);
    pt.y -= 5 * (10 / 540.f) * (5.f / larger_dimension);
    gluProject(pt.x, pt.y, pt.z,
               model_view, projection, viewport,
               &upper[0], &upper[1], &upper[2]);
    
    float x1 = (lower[0] / BoggleGUI::window_width()) * 100;
    float y1 = (lower[1] / BoggleGUI::window_height()) * 100;
    float x2 = (upper[0] / BoggleGUI::window_width()) * 100;
    float y2 = (upper[1] / BoggleGUI::window_height()) * 100;
    
    float thick = 2.75f;
    if (width_ > 7)
        thick = 2.f;
    Label label(str, vec3(0.25, 0.25, 0.15), false, true, thick,
                vec4(x1, y1, x2, y2));
    dice_labels_.push_back(label);
}

void BoggleGraph::DrawLabels() const {
    for (size_t i = 0; i < dice_labels_.size(); ++i)
        dice_labels_.at(i).Draw();
}

