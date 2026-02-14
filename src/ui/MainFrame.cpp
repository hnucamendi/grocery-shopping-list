#include "ui/MainFrame.h"

MainFrame::MainFrame()
    : wxFrame(nullptr, wxID_ANY, "Grocery Shopping List", wxDefaultPosition,
              wxSize(MainFrame::DEFAULT_WIDTH, MainFrame::DEFAULT_HEIGHT)) {
  SetMinSize(wxSize(MainFrame::DEFAULT_WIDTH, MainFrame::DEFAULT_HEIGHT));
}
