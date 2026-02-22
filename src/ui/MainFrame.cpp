#include "ui/MainFrame.h"
#include "ui/InputBar.h"
#include <wx/sizer.h>

MainFrame::MainFrame()
    : wxFrame(nullptr, wxID_ANY, "Grocery Shopping List", wxDefaultPosition,
              wxSize(MainFrame::DEFAULT_WIDTH, MainFrame::DEFAULT_HEIGHT)) {
    wxTopLevelWindowBase::SetMinSize(wxSize(MainFrame::DEFAULT_WIDTH, MainFrame::DEFAULT_HEIGHT));
  SetInitialSize(wxSize(1200, 720));

  auto *sizer = new wxBoxSizer(wxVERTICAL);
  m_groceryList = std::make_unique<GroceryList>();
  auto *inputBar = new InputBar(this, m_groceryList.get());

  sizer->Add(inputBar, wxSizerFlags().Center().Border(wxALL, 10).Proportion(0));
  SetSizer(sizer);
}
