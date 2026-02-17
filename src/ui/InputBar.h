#ifndef GROCERY_SHOPPING_LIST_UI_INPUTBAR_H
#define GROCERY_SHOPPING_LIST_UI_INPUTBAR_H

#include "domain/GroceryList.h"
#include <wx/button.h>
#include <wx/choice.h>
#include <wx/panel.h>
#include <wx/textctrl.h>

class InputBar : public wxPanel {
public:
  InputBar(wxWindow *parent, GroceryList *groceryList);
  void onAddClicked(wxCommandEvent &event);

private:
  GroceryList *m_groceryList;
  wxTextCtrl *m_nameField;
  wxTextCtrl *m_quantityField;
  wxChoice *m_sectionField;
  wxButton *m_addButton;
};

#endif
