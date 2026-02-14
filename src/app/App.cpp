#include "app/App.h"
#include "ui/MainFrame.h"

wxIMPLEMENT_APP(App);

bool App::OnInit() {
  MainFrame *frame = new MainFrame();
  frame->Show(true);
  return true;
}
