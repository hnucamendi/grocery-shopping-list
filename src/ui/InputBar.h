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

#ifdef TESTING
  void setNameForTests(const wxString &name) { m_nameField->SetValue(name); };
  void setQuantityForTests(const wxString &quantity) {
    m_quantityField->SetValue(quantity);
  };
  void setSectionForTests(const int &index) {
    m_sectionField->SetSelection(index);
  };
  wxString getNameForTests() { return m_nameField->GetValue(); };
  wxString getQuantityForTests() { return m_quantityField->GetValue(); };
  int getSectionIndexForTests() { return m_sectionField->GetSelection(); };
  void clickAddForTests() {
    wxCommandEvent event(wxEVT_BUTTON);
    onAddClicked(event);
  };
  bool isAddButtonEnabledForTests() { return m_addButton->IsEnabled(); }
  void triggerUpdateButtonEventForTests() { updateAddButtonState(); };
#endif

private:
  GroceryList *m_groceryList;
  wxTextCtrl *m_nameField;
  wxTextCtrl *m_quantityField;
  wxChoice *m_sectionField;
  wxButton *m_addButton;

  void onAddClicked(wxCommandEvent &event);
  void onNameTextUpdate(wxCommandEvent &event);
  void onQuantityTextUpdate(wxCommandEvent &event);
  void onSectionSelectionUpdate(wxCommandEvent &event);
  void updateAddButtonState();
};

#endif
