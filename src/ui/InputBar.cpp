#include "ui/InputBar.h"
#include "domain/GroceryItem.h"
#include "domain/GroceryList.h"
#include "domain/StoreSection.h"
#include <stdexcept>
#include <wx/log.h>
#include <wx/sizer.h>

InputBar::InputBar(wxWindow *parent, GroceryList *groceryList)
    : wxPanel(parent), m_groceryList(groceryList) {
  auto *sizer = new wxBoxSizer(wxHORIZONTAL);

  m_nameField = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition,
                               wxDefaultSize, wxTE_PROCESS_ENTER);
  m_nameField->SetHint("Item name");

  m_quantityField = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition,
                                   wxDefaultSize, wxTE_PROCESS_ENTER);
  m_quantityField->SetHint("Qty");

  m_sectionField = new wxChoice(this, wxID_ANY);
  for (auto section : getAllSections()) {
    m_sectionField->Append(toString(section));
  }

  m_sectionField->Create(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);

  m_addButton = new wxButton(this, wxID_ANY, "Add");

  sizer->Add(m_nameField, 1, wxALL | wxEXPAND, 5);
  sizer->Add(m_quantityField, 0, wxALL | wxEXPAND, 5);
  sizer->Add(m_sectionField, 0, wxALL | wxEXPAND, 5);
  sizer->Add(m_addButton, 0, wxALL | wxEXPAND, 5);

  SetSizer(sizer);

  m_addButton->Bind(wxEVT_BUTTON, &InputBar::onAddClicked, this);
}

void InputBar::onAddClicked(wxCommandEvent &event) {
  wxString name = m_nameField->GetValue();
  wxString quantity = m_quantityField->GetValue();
  int sectionIndex = m_sectionField->GetSelection();
  StoreSection chosenSection = getAllSections()[sectionIndex];
  if (name == "") {
    wxLogMessage("Name field was not properly entered");
    throw std::invalid_argument("Name field was not properly entered");
  }
  if (quantity == "") {
    quantity = "1";
    wxLogMessage("Quantity field was not properly entered - defaultin to '1'");
  }

  if (sectionIndex == wxNOT_FOUND) {
    wxLogMessage("Section field was not properly entered");
    throw std::invalid_argument("Section field was not properly entered");
  }

  m_groceryList->addItem(
      GroceryItem(name.ToStdString(), quantity.ToStdString(), chosenSection));

  name = "";
  quantity = "";
  sectionIndex = 0;

  wxLogMessage("Add clicked: %s", name);
}
