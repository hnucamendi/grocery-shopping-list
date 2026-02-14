#ifndef GROCERY_SHOPPING_LIST_UI_MAINFRAME_H
#define GROCERY_SHOPPING_LIST_UI_MAINFRAME_H

#include <wx/frame.h>

class MainFrame : public wxFrame {
public:
  MainFrame();

private:
  static constexpr int DEFAULT_WIDTH = 600;
  static constexpr int DEFAULT_HEIGHT = 400;
};

#endif
