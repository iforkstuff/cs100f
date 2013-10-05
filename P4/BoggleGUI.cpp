#include "BoggleGUI.h"
#include "BogglePlayer.h"
#include "MyBogglePlayer.h"
#include "Geometry.h"
#include <sstream>
#include <cstdio>
#include <cstring>

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

using std::stringstream;

int BoggleGUI::window_w_ = 0;
int BoggleGUI::window_h_ = 0;

bool Label::ToPixelValues(const vec4& coords,
                          int* x1, int* y1, int* x2, int* y2) {
    if (x1 == NULL || y1 == NULL || x2 == NULL || y2 == NULL)
        return false;
    *x1 = (int)(coords.x * BoggleGUI::window_width() / 100.f);
    *y1 = (int)(coords.y * BoggleGUI::window_height() / 100.f);
    *x2 = (int)(coords.z * BoggleGUI::window_width() / 100.f);
    *y2 = (int)(coords.w * BoggleGUI::window_height() / 100.f);
    return true;
}

Label::Label(const string& text, const vec3& color, bool shadowed,
             bool centered, float thickness, const vec4& coords) :
             text_(text), color_(color), thickness_(thickness),
             shadowed_(shadowed), centered_(centered), coords_(coords) {
}

void Label::UpdateText(const string& text) { text_ = text; }

int Label::pad_horiz() { return 15 * BoggleGUI::window_width() / 960; }
int Label::pad_vert() { return 10 * BoggleGUI::window_height() / 540; }

void Label::Draw() const {
    int window_width = BoggleGUI::window_width();
    int window_height = BoggleGUI::window_height();

    // Determine the dimensions of the text to render.
    int text_width = 0;
    int max_width = 0;
    for (const char* letter = text_.c_str(); *letter; ++letter) {
        if (*letter == '\n') {
            text_width = 0;
        } else {
            text_width += glutStrokeWidth(GLUT_STROKE_ROMAN, *letter);
            if (max_width < text_width)
                max_width = text_width;
        }
    }
    text_width = max_width;
    int text_height = 120;
    int text_descender = 34;
    int x1, y1, x2, y2;
    ToPixelValues(coords_, &x1, &y1, &x2, &y2);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glPushAttrib(GL_ENABLE_BIT);
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glLoadIdentity();
    gluOrtho2D(0, window_width, 0, window_height);

    float mult = (window_width / 960.f) * (window_height / 540.f);
    glLineWidth(thickness_ * pow(mult, 0.5f));
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

    glMatrixMode(GL_MODELVIEW);
    glDisable(GL_DEPTH_TEST);
    glPushMatrix();
    glLoadIdentity();

    // Draw a translucent gray quad covering the label's area.
    if (shadowed_) {
        const vec4 LABEL_BKND_COL(0.0, 0.0, 0.0, 0.5);
        glColor4f(LABEL_BKND_COL.x, LABEL_BKND_COL.y,
                  LABEL_BKND_COL.z, LABEL_BKND_COL.w); 
        glBegin(GL_QUADS);
        vec4 quad_coords(x1 - pad_horiz(), y1 - pad_vert(),
                         x2 + pad_horiz(), y2 + pad_vert());
        glVertex3f(quad_coords.x, quad_coords.y, 0);
        glVertex3f(quad_coords.z, quad_coords.y, 0);
        glVertex3f(quad_coords.z, quad_coords.w, 0);
        glVertex3f(quad_coords.x, quad_coords.w, 0);
        glEnd();
    }

    glLoadIdentity();
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_TEXTURE_2D);

    float scale_x = (x2 - x1) / (float)text_width;
    float scale_y = (y2 - y1) / (float)text_height;
    float scale = (scale_x < scale_y) ? scale_x : scale_y;
    float shift_down = ((y2 - y1) - (text_height + text_descender) * scale);
    if (centered_)
        shift_down /= 2;

    // Render the text, skipping to a newline for every '\n' character.
    glTranslatef(x1, (y1 + text_descender * scale) + shift_down, 0.1);
    glScalef(scale, scale, 1.0);
    glColor4f(color_.x, color_.y, color_.z, 1);
    glPushMatrix();
    glPushMatrix();
    for (const char* letter = text_.c_str(); *letter; ++letter) {
        if (*letter == '\n') {
            glPopMatrix();
            glTranslatef(0, -text_height, 0.0);
            glPushMatrix(); 

            // Automatically cut off drawing the label if we're past the limit.
            GLdouble model_view[16];
            GLdouble projection[16];
            GLint viewport[4];
            glGetDoublev(GL_MODELVIEW_MATRIX, model_view);    
            glGetDoublev(GL_PROJECTION_MATRIX, projection);
            glGetIntegerv(GL_VIEWPORT, viewport);
            GLdouble pos[3];
            gluProject(0, -text_height, 0, model_view, projection, viewport,
                       &pos[0], &pos[1], &pos[2]);
            const char* next = letter + 1;
            if (pos[1] < y1 && *next != '\0') {
                for (int i = 0; i < 3; ++i)
                    glutStrokeCharacter(GLUT_STROKE_ROMAN, '.');
                break;
            }
            
        } else {
            glutStrokeCharacter(GLUT_STROKE_ROMAN, *letter);
        }
    }
    glPopMatrix();
    glPopMatrix();
   
    glColor4f(1, 1, 1, 1);
    glPopAttrib();
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

Button::Button(const string& text, const vec3& color, bool shadowed,
               bool centered, float thickness, const vec4& coords,
               bool invert_when_clicked) :
               Label(text, color, shadowed, centered, thickness, coords), 
               invert_when_clicked_(invert_when_clicked),
               mouse_was_down_(false) {
}

void Button::Draw(const MouseInfo& mouse_info) const {
    Label::Draw();

    // TODO
}

static bool InRange(const MouseInfo& mouse, const vec4& button) {
    int mouse_x = mouse.x, mouse_y = BoggleGUI::window_height() - mouse.y;
    int btn_x1, btn_y1, btn_x2, btn_y2;
    Label::ToPixelValues(button, &btn_x1, &btn_y1, &btn_x2, &btn_y2);
    return mouse_x >= btn_x1 && mouse_x <= btn_x2 &&
    mouse_y >= btn_y1 && mouse_y <= btn_y2;
}

BoggleGUI::BoggleGUI() :    
    new_random_("Random", vec3(1), true, true, 3.f, vec4(5, 90, 15, 95), true),
    new_custom_("Custom", vec3(1), true, true, 3.f, vec4(20, 90, 30, 95), true),
    comp_play_("Let Computer Play", vec3(1), true, true, 3.f,
               vec4(70, 90, 95, 95), true),
    human_score_label_("Human: 0", vec3(1), true, true, 3.f,
                       vec4(5, 80, 25, 85)),
    human_word_list_label_("Word List (0)   ", vec3(1), true, false, 3.f,
                           vec4(5, 20, 25, 75)),
    computer_score_label_("Computer: 0", vec3(1), true, true, 3.f,
                          vec4(75, 80, 95, 85)),
    computer_word_list_label_("Word List (0)   ", vec3(1), true, false, 3.f,
                              vec4(75, 20, 95, 75)),
    enter_word_header_("Enter word:", vec3(1), false, true, 3.f,
                       vec4(22, 5, 42, 15)),
    enter_word_("", vec3(1), true, true, 3.f, vec4(45, 7, 95, 13)),
    time_left_("3:00", vec3(1), false, true, 4.f, vec4(5, 5, 15, 15)),
    seconds_left_(180),
    zoom_board_(ANIMATE ? 2.65f : 0.f),
    ready_for_comp_(false),
    mouse_info_(GLUT_LEFT_BUTTON, GLUT_UP, 0, 0) {
  board_.SetPlayer(new MyBogglePlayer());
  board_.Random(4, 4);
}

void BoggleGUI::ResetBoard(int width) {
    if (width > 0)
        board_.Random(width, width);
    human_word_list_.clear();
    computer_word_list_.clear();
    UpdateWordList(false);
    UpdateWordList(true);
    seconds_left_ = 180;
    if (ANIMATE)
        zoom_board_ = 2.65f;
}

void BoggleGUI::SetMouseState(const MouseInfo& mouse_info) {
    mouse_info_ = mouse_info;
    
    if (mouse_info.button == GLUT_LEFT_BUTTON &&
        mouse_info.state == GLUT_DOWN) {
        
        if (InRange(mouse_info, new_random_.coords())) {
            ResetBoard(board_.width());
        } else if (InRange(mouse_info, new_custom_.coords())) {
            board_.Custom();
            ResetBoard(0);            
        } else if (InRange(mouse_info, comp_play_.coords())) {
            comp_play_.UpdateText(" Please Wait...");
            ready_for_comp_ = true;
            glutPostRedisplay();
        }
    }
}

void BoggleGUI::ErrorMsg(const string& msg) {
    enter_word_.UpdateText(msg.c_str());
}

void BoggleGUI::AddQueuedComputerWords() {
    if (ready_for_comp_) {
        ready_for_comp_ = false;
        computer_word_list_.clear();
        board_.GetAllValidWords(3, &add_to_comp_list_);
    } else if (add_to_comp_list_.size() == 0) {
        return;
    }
    string str;
    int reps = (add_to_comp_list_.size() > 20) ? 10 : 1;
    for (int i = 0; i < reps; ++i) {
        str = add_to_comp_list_.back();
        add_to_comp_list_.pop_back();
        computer_word_list_.push_back(str);
    }
    if (add_to_comp_list_.size() == 0)
        comp_play_.UpdateText("Let Computer Play");
    vector<int> locations;
    board_.IsOnBoard(str, &locations);
    board_.HighlightLocations(locations);
    UpdateWordList(false);
}

void BoggleGUI::UpdateWordList(bool human) {
    vector<string>& list = human ? human_word_list_ : computer_word_list_;
    
    char header[100];
    sprintf(header, "Word List (%d)", (int)list.size());
    string label(header);
    if (list.size() == 0) {
        label += "   ";
    } else  {
        label += ":";
        for (size_t i = 0; i < 16 - strlen(header); ++i)
            label += ' ';
    }
    label += "\n\n";
    
    // Don't waste time putting invisible words in the list
    for (int i = list.size() - 1; i >= 0; --i) {
        label += list.at(i);
        label += '\n';
    }
    char score_label[100];
    if (human) {
        human_word_list_label_.UpdateText(label);
        int score = CalculateScore(human_word_list_);
        sprintf(score_label, "Human: %d", score);
        human_score_label_.UpdateText(string(score_label));
    } else {
        computer_word_list_label_.UpdateText(label);
        int score = CalculateScore(computer_word_list_);   
        sprintf(score_label, "Computer: %d", score);
        computer_score_label_.UpdateText(string(score_label));
    }
}

int BoggleGUI::CalculateScore(const vector<string>& words) {
    int score = 0;
    for (vector<string>::const_iterator it = words.begin(); it != words.end();
         ++it) {
        switch ((*it).length()) {
            case 0: case 1: case 2: break;
            case 3: case 4: score++; break;
            case 5: score += 2; break;
            case 6: score += 3; break;
            case 7: score += 5; break;
            default: score += 11; break;
        }
    }
    return score;
}

void BoggleGUI::DecreaseSeconds() {
    if (seconds_left_ == 0)
        return;
    --seconds_left_;
    if (seconds_left_ == 0) {
        int human_score = CalculateScore(human_word_list_);
        int computer_score = CalculateScore(computer_word_list_);
        if (human_score > computer_score)
            enter_word_.UpdateText("Time's Up!");
        else if (human_score == computer_score)
            enter_word_.UpdateText("Time's Up!");
        else
            enter_word_.UpdateText("Time's Up!");
    }
    
    int minutes = 0, seconds = seconds_left_;
    while (seconds >= 60) {
        seconds -= 60;
        ++minutes;
    }
    stringstream stream;
    stream << minutes << ":";
    if (seconds < 10) stream << "0";
    stream << seconds;
    time_left_.UpdateText(stream.str());
}

void BoggleGUI::AddHumanLetter(unsigned char letter) {
    if (seconds_left_ == 0) return;
    if ((letter >= 'a' && letter <= 'z') || (letter >= 'A' && letter <= 'Z')) {
        if (cur_human_word_.length() < size_t(board_.width() * board_.width()))
            cur_human_word_ += letter;
        
    } else if ((letter == 8 || letter == 127) && cur_human_word_.length() > 0) { 
        cur_human_word_ = cur_human_word_.substr // Backspace
            (0, cur_human_word_.length() - 1);
        
    } else if (letter == 13) { // Enter
        string word = BoggleGraph::ToLower(cur_human_word_);
        for (vector<string>::iterator it = human_word_list_.begin();
             it != human_word_list_.end(); ++it)
            if (word.compare(*it) == 0)
                return;
        
        vector<int> locations;
        if (seconds_left_ > 0 && word.length() >= 3) {
            if (!board_.IsInLexicon(word)) {
                ErrorMsg("Not In Lexicon!");
                cur_human_word_.clear();
                return;
            } else if (!board_.IsOnBoard(word, &locations)) {
                ErrorMsg("Not On Board!");
                cur_human_word_.clear();
                return;
            } else {
                human_word_list_.push_back(cur_human_word_);
                UpdateWordList(true);
                board_.HighlightLocations(locations);
                cur_human_word_.clear();
            }
        }
    }
    enter_word_.UpdateText(cur_human_word_);
}

void BoggleGUI::DrawBackgroundImage() const {
    glPushMatrix();
    float ratio = float(window_w_) / window_h_;
    vec3 shift(0.5, 0.5, 0.1);
    vec3 coords(shift.x * 5 * ratio, shift.y * 5, -1.75);
    glBindTexture(GL_TEXTURE_2D, bknd_texture_.index());
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-coords.x, -coords.y, coords.z);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(coords.x, -coords.y, coords.z);    
    glTexCoord2f(1.0f, 1.0f); glVertex3f(coords.x, coords.y, coords.z);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-coords.x, coords.y, coords.z);
    glEnd ();
    glPopMatrix ();
}

bool BoggleGUI::LoadData() {
    if (!bknd_texture_.Load(bknd_img_.c_str()))
        return false;
    if (!board_.LoadTextures(face_bmp_, face_shaded_bmp_))
        return false;
    if (!board_.LoadWords(word_list_))
        return false;
    return true;
}

void BoggleGUI::Draw() {
    board_.DrawDice(zoom_board_);
    board_.DrawLabels();
    DrawBackgroundImage();
    new_random_.Draw(mouse_info_);
    new_custom_.Draw(mouse_info_);
    comp_play_.Draw(mouse_info_);
    human_score_label_.Draw();
    human_word_list_label_.Draw();
    computer_score_label_.Draw();
    computer_word_list_label_.Draw();
    enter_word_header_.Draw();
    enter_word_.Draw();
    time_left_.Draw();
    
    zoom_board_ -= 0.075;
    if (zoom_board_ < 0)
        zoom_board_ = 0;
}
