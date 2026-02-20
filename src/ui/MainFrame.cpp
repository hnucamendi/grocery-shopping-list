#include "ui/MainFrame.h"
#include "domain/GroceryList.h"
#include "ui/InputBar.h"
#include <wx/log.h>

MainFrame::MainFrame()
    : wxFrame(nullptr, wxID_ANY, "Grocery Shopping List", wxDefaultPosition,
              wxSize(MainFrame::DEFAULT_WIDTH, MainFrame::DEFAULT_HEIGHT)) {
  SetMinSize(wxSize(MainFrame::DEFAULT_WIDTH, MainFrame::DEFAULT_HEIGHT));

  GroceryList *groceryList = new GroceryList();
  InputBar *inputBar = new InputBar(this, groceryList);
  inputBar->Show(true);
}
