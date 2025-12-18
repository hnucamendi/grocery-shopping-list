#include <wx/wx.h>

class GroceryApp : public wxApp {
public:
  bool OnInit() override;
};

wxIMPLEMENT_APP(GroceryApp);

class GroceryFrame : public wxFrame {
public:
  GroceryFrame();

private:
  void OnHello(wxCommandEvent &event);
  void OnExit(wxCommandEvent &event);
  void OnAbout(wxCommandEvent &event);
};

enum { ID_Hello = 1 };

bool GroceryApp::OnInit() {
  GroceryFrame *frame = new GroceryFrame();
  frame->Show();
  return true;
}

GroceryFrame::GroceryFrame() : wxFrame(nullptr, wxID_ANY, "Hello World") {

  wxMenu *menuFile = new wxMenu;
  menuFile->Append(ID_Hello, "&Hello...\tCtrl-H",
                   "Help string shown in status bar for this menu item.");

  menuFile->AppendSeparator();
  menuFile->Append(wxID_EXIT);

  wxMenu *menuHelp = new wxMenu;
  menuHelp->Append(wxID_ABOUT);

  wxMenuBar *menuBar = new wxMenuBar;
  menuBar->Append(menuFile, "&File");
  menuBar->Append(menuHelp, "&Help");

  SetMenuBar(menuBar);

  CreateStatusBar();
  SetStatusText("Testing out wxWidgets");

  Bind(wxEVT_MENU, &GroceryFrame::OnHello, this, ID_Hello);
  Bind(wxEVT_MENU, &GroceryFrame::OnAbout, this, wxID_ABOUT);
  Bind(wxEVT_MENU, &GroceryFrame::OnExit, this, wxID_EXIT);
}

void GroceryFrame::OnExit(wxCommandEvent &event) { Close(true); }
void GroceryFrame::OnAbout(wxCommandEvent &event) {
  wxMessageBox("This is just a test message from GroceryApp", "GroceryApp",
               wxOK | wxICON_INFORMATION);
}
void GroceryFrame::OnHello(wxCommandEvent &event) {
  wxLogMessage("Hello from wxEvent");
}
