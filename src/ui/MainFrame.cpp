#include "ui/MainFrame.h"
#include "ui/InputBar.h"
#include <wx/sizer.h>

MainFrame::MainFrame()
    : wxFrame(nullptr, wxID_ANY, "Grocery Shopping List", wxDefaultPosition,
              wxSize(MainFrame::DEFAULT_WIDTH, MainFrame::DEFAULT_HEIGHT)) {
  SetMinSize(wxSize(MainFrame::DEFAULT_WIDTH, MainFrame::DEFAULT_HEIGHT));
  SetInitialSize(wxSize(1200, 720));

  wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);

  m_groceryList = new GroceryList();

  InputBar *inputBar = new InputBar(this, m_groceryList);
  inputBar->Show(true);

  sizer->Add(inputBar, wxSizerFlags().Center().Border(wxALL, 10).Proportion(0));
  SetSizer(sizer);
}
