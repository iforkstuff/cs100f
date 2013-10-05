#ifndef BOGGLE_GUI_H
#define BOGGLE_GUI_H

#include "BoggleGraph.h"
#include "Texture.h"
#include "vec3.h"
#include "vec4.h"
#include <vector>
#include <string>
using std::vector;
using std::string;

// Disable stupid Windows warnings.
#pragma warning( disable : 4996)

// Holds info about the current state of the mouse.
struct MouseInfo {
    int button;
    int state;
    int x, y;
    MouseInfo(int btn, int st, int mx, int my) :
        button(btn), state(st), x(mx), y(my) { }
};

// A text label. This UI element passively sits in one location doing nothing.
class Label {
public:

    // Convert the coordinates from percentage of the window size to pixels.
    static bool ToPixelValues(const vec4& coords,
                              int* x1, int* y1, int* x2, int* y2);

    // Construct a new Label with the given text.
    // Any newline characters in the text are treated as newlines in the label.
    // The color is an RGB vector where each element ranges from 0 to 1.
    // If outline is true, draw an outline around the Label's occupying space.
    // The coords are where to draw the label, in percentage of the window size. 
    Label(const string& text, const vec3& color, bool shadowed, bool centered,
          float thickness, const vec4& coords);
    
    // Replace the label's text with new text.
    void UpdateText(const string& text);

    // Issue OpenGL commands to draw the label.
    virtual void Draw() const;
    
    // Get the padding around a label draw area.
    static int pad_horiz();
    static int pad_vert();
    
    const vec4& coords() const { return coords_; } 

protected:
    const static int TEXT_PADDING = 5;
    string text_;
    vec3 color_;
    vec4 coords_;
    bool shadowed_;
    bool centered_;
    float thickness_;
};

// A clickable area in the window. The Button calls a callback function when
// the user clicks in the area that a Button occupies. The Button may have text
// an, outline, and other visible elements, so it is a subclass of Label.
class Button : public Label {
public:

    // Construct a new Button. All the parameters that are shared with Label
    // are sent to Label's constructor. The callback funtion is called when the
    // user clicks on the button. If invert_when_clicked is true, the button's
    // colors are inverted while the user holds the mouse down on the button.
    Button(const string& text, const vec3& color, bool shadowed,
           bool centered, float thickness, const vec4& coords,
           bool invert_when_clicked);

    // Issue OpenGL commands to draw the button. The caller of this method must
    // pass in the current mouse state so the Button can decide whether to draw
    // the button in a different state when the mouse button is down, and so
    // the Button can call the callback when the user releases the mouse button.
    void Draw(const MouseInfo& mouse_info) const;

private:
    bool invert_when_clicked_;
    bool mouse_was_down_;
};

// Contains user interface elements for the 3D scene containing the Boggle
// board. The OpenGL redraw method uses this to draw the scene.
class BoggleGUI {
public:

    // Remember the window dimensions for proportional rendering later.
    static void SaveWindowSize(int w, int h) { window_w_ = w; window_h_ = h; }
    static int window_width() { return window_w_; }
    static int window_height() { return window_h_; }

    void SetFilenames(const string& word_list, const string& bknd_img,
                             const string& face_bmp,
                             const string& face_shaded_bmp) {
        word_list_ = word_list;
        bknd_img_ = bknd_img;
        face_bmp_ = face_bmp;
        face_shaded_bmp_ = face_shaded_bmp;
    }

    // Construct a new BoggleGUI with the given board. All scores are reset.
    BoggleGUI();
    void ResetBoard(int width);
    
    // Add the last one of any recently-found computer words to the list.
    void AddQueuedComputerWords();

    // Set variables in the UI.
    void DecreaseSeconds();
    void UpdateWordList(bool human);
    
    // Call this to update the current word when the user types a letter key.
    void AddHumanLetter(unsigned char letter);

    // The glutMouseFunc just calls this, updating the GUI's stored mouse state.
    // Mouse clicks are checked against button positions, possibly caling the
    // button's callback method.
    void SetMouseState(const MouseInfo& mouse_info);
    
    // Display an error message at the label at the bottom of the screen.
    void ErrorMsg(const string& msg);

    // Issue OpenGL commands to draw the entire scene.
    void Draw();
    
    // Load texture images and other data from their files.
    bool LoadData();

    // Returns whether there are queued words to add to the computer's list.
    bool HasCompListWords() { return !add_to_comp_list_.empty(); }

private:

    // These are static parameters of the UI that should be set before init.
    static int window_w_, window_h_;
    string word_list_;
    string bknd_img_;
    string face_bmp_;
    string face_shaded_bmp_;
    
    BoggleGraph board_;
    Button new_random_;
    Button new_custom_;
    Button comp_play_;
    Label human_score_label_;
    Label human_word_list_label_;
    Label computer_score_label_;
    Label computer_word_list_label_;
    Label enter_word_header_;
    Label enter_word_;
    Label time_left_;
    int seconds_left_;
    float zoom_board_;
    bool ready_for_comp_;
  
    vector<string> human_word_list_;
    vector<string> computer_word_list_;
    vector<string> add_to_comp_list_;
    string cur_human_word_;

    MouseInfo mouse_info_;
    Texture bknd_texture_;
    
    const static bool ANIMATE = false;

    void DrawBackgroundImage() const;

    static int CalculateScore(const vector<string>& words);
};


#endif
