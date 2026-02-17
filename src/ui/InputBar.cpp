#include "ui/InputBar.h"
#include "domain/GroceryList.h"
#include <wx/sizer.h>

InputBar::InputBar(wxWindow *parent, GroceryList *groceryList)
    : wxPanel(parent), m_groceryList(groceryList) {
  auto *sizer = new wxBoxSizer(wxHORIZONTAL);

  m_nameField = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition,
                               wxDefaultSize, wxTE_PROCESS_ENTER);
  m_nameField->SetHint("Item name");

  m_quantityField = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition,
                                   wxDefaultSize, wxTE_PROCESS_ENTER);
  m_quantityField->SetHint("Item quantity");

  m_sectionField = new wxChoice(this, wxID_ANY);

  m_addButton = new wxButton(this, wxID_ANY, "Add");

  sizer->Add(m_nameField, 1, wxALL | wxEXPAND, 5);
  sizer->Add(m_quantityField, 0, wxALL | wxEXPAND, 5);
  sizer->Add(m_sectionField, 0, wxALL | wxEXPAND, 5);
  sizer->Add(m_addButton, 0, wxALL | wxEXPAND, 5);

  SetSizer(sizer);

  m_addButton->Bind(wxEVT_BUTTON, &InputBar::onAddClicked, this);
}
