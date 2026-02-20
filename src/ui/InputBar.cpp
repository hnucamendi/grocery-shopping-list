#include "ui/InputBar.h"
#include "domain/GroceryItem.h"
#include "domain/GroceryList.h"
#include "domain/StoreSection.h"
#include <stdio.h>
#include <wx/log.h>
#include <wx/sizer.h>

InputBar::InputBar(wxWindow *parent, GroceryList *groceryList)
    : wxPanel(parent), m_groceryList(groceryList) {
  auto *sizer = new wxBoxSizer(wxHORIZONTAL);

  m_nameField = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition,
                               wxSize(250, 20), wxTE_PROCESS_ENTER);
  m_nameField->SetHint("Item name");

  m_quantityField = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition,
                                   wxSize(250, 20), wxTE_PROCESS_ENTER);
  m_quantityField->SetHint("Qty");

  std::vector<std::string> storeSectionStrings = {
      "Please select store section:"};
  for (auto section : getAllSections()) {
    storeSectionStrings.push_back(toString(section));
  }

  m_sectionField = new wxChoice(this, wxID_ANY, wxDefaultPosition,
                                wxDefaultSize, storeSectionStrings);

  m_addButton =
      new wxButton(this, wxID_ANY, "Add", wxDefaultPosition, wxDefaultSize);

  sizer->Add(m_nameField,
             wxSizerFlags().Border(wxALL, 10).Center().Align(wxTOP));
  sizer->Add(m_quantityField,
             wxSizerFlags().Border(wxALL, 10).Center().Align(wxTOP));
  sizer->Add(m_sectionField,
             wxSizerFlags().Border(wxALL, 10).Center().Align(wxTOP));
  sizer->Add(m_addButton,
             wxSizerFlags().Border(wxALL, 10).Center().Align(wxTOP));

  SetSizer(sizer);

  m_addButton->Bind(wxEVT_BUTTON, &InputBar::onAddClicked, this);
}

void InputBar::onAddClicked(wxCommandEvent &event) {
  wxString name = m_nameField->GetValue();
  wxString quantity = m_quantityField->GetValue();

  StoreSection chosenSection =
      fromString(m_sectionField->GetStringSelection().ToStdString());

  if (name == "") {
    wxLogMessage("Name field was not properly entered");
    m_nameField->SetFocus();
  }

  if (quantity == "") {
    wxLogMessage("Quantity field was not properly entered - defaulting to '1'");
    quantity = "1";
  }

  if (m_sectionField->GetSelection() == wxNOT_FOUND) {
    wxLogMessage("Section field was not properly entered");
    m_sectionField->SetFocus();
  }

  m_groceryList->addItem(
      GroceryItem(name.ToStdString(), quantity.ToStdString(), chosenSection));

  wxLogMessage("Add clicked: %s, quantity: %s section: %s", name, quantity,
               toString(chosenSection));

  m_nameField->Clear();
  m_quantityField->Clear();
  m_sectionField->SetSelection(0);

  m_nameField->SetFocus();
}
