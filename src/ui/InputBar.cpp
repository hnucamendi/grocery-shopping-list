#include "ui/InputBar.h"
#include "domain/GroceryItem.h"
#include "domain/GroceryList.h"
#include "domain/StoreSection.h"
#include <wx/log.h>
#include <wx/sizer.h>

InputBar::InputBar(wxWindow *parent, GroceryList *groceryList)
    : wxPanel(parent), m_groceryList(groceryList) {
  wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);

  m_nameField = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition,
                               wxDefaultSize, wxTE_PROCESS_ENTER);
  m_nameField->SetHint("Item name");
  m_nameField->SetMinSize(wxSize(200, -1));

  m_quantityField = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition,
                                   wxDefaultSize, wxTE_PROCESS_ENTER);
  // wxSize(250, 20), wxTE_PROCESS_ENTER);
  m_quantityField->SetHint("Qty");
  m_quantityField->SetMinSize(wxSize(80, -1));

  wxArrayString storeSectionStrings;
  storeSectionStrings.Add("Please select store section:");
  for (const auto &section : getAllSections()) {
    storeSectionStrings.Add(toString(section));
  }

  m_sectionField = new wxChoice(this, wxID_ANY, wxDefaultPosition,
                                wxDefaultSize, storeSectionStrings);

  m_addButton =
      new wxButton(this, wxID_ANY, "Add", wxDefaultPosition, wxDefaultSize);

  sizer->Add(
      m_nameField,
      wxSizerFlags().Border(wxALL, 10).Expand().Align(wxTOP).Proportion(2));
  sizer->Add(
      m_quantityField,
      wxSizerFlags().Border(wxALL, 10).Expand().Align(wxTOP).Proportion(1));
  sizer->Add(
      m_sectionField,
      wxSizerFlags().Border(wxALL, 10).Expand().Align(wxTOP).Proportion(0));
  sizer->Add(
      m_addButton,
      wxSizerFlags().Border(wxALL, 10).Expand().Align(wxTOP).Proportion(0));

  SetSizer(sizer);

  m_addButton->Disable();

  m_addButton->Bind(wxEVT_BUTTON, &InputBar::onAddClicked, this);
  m_nameField->Bind(wxEVT_TEXT_ENTER, &InputBar::onAddClicked, this);
  m_quantityField->Bind(wxEVT_TEXT_ENTER, &InputBar::onAddClicked, this);
  m_nameField->Bind(wxEVT_TEXT, &InputBar::onNameTextUpdate, this);
  m_quantityField->Bind(wxEVT_TEXT, &InputBar::onQuantityTextUpdate, this);
}

void InputBar::onAddClicked(wxCommandEvent &event) {
  wxString name = m_nameField->GetValue();
  wxString quantity = m_quantityField->GetValue();

  if (name.IsEmpty()) {
    return;
  }

  if (quantity.IsEmpty()) {
    quantity = "1";
  }

  int sectionIndex = m_sectionField->GetSelection();
  if (sectionIndex <= 0) {
    return;
  }

  std::string selectionValue =
      m_sectionField->GetStringSelection().ToStdString();
  StoreSection chosenSection = fromString(selectionValue);

  m_groceryList->addItem(
      GroceryItem(name.ToStdString(), quantity.ToStdString(), chosenSection));

  wxLogMessage("Add clicked: %s", name);

  m_nameField->Clear();
  m_quantityField->Clear();
  m_sectionField->SetSelection(0);

  m_nameField->SetFocus();
}

void InputBar::onUpdateAddButtonField() {
  bool nameFieldValid = !m_nameField->GetValue().IsEmpty();
  bool quantityFieldValid = !m_quantityField->GetValue().IsEmpty();
  m_addButton->Enable(nameFieldValid && quantityFieldValid);
}

void InputBar::onNameTextUpdate(wxCommandEvent &event) {
  onUpdateAddButtonField();
}

void InputBar::onQuantityTextUpdate(wxCommandEvent &event) {
  onUpdateAddButtonField();
}
