#include "ui/InputBar.h"
#include "domain/GroceryItem.h"
#include "domain/GroceryList.h"
#include "domain/StoreSection.h"
#include <wx/log.h>
#include <wx/sizer.h>

InputBar::InputBar(wxWindow *parent, GroceryList *groceryList)
    : wxPanel(parent), m_groceryList(groceryList) {
  auto *sizer = new wxBoxSizer(wxHORIZONTAL);

  m_nameField = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition,
                               wxDefaultSize, wxTE_PROCESS_ENTER);
  m_nameField->SetHint("Item name");
  m_nameField->SetMinSize(wxSize(200, -1));

  m_quantityField = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition,
                                   wxDefaultSize, wxTE_PROCESS_ENTER);
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
  m_sectionField->Bind(wxEVT_CHOICE, &InputBar::onSectionSelectionUpdate, this);
}

void InputBar::onAddClicked(wxCommandEvent &event) {
  const wxString name = m_nameField->GetValue().Trim();
  const wxString quantity = m_quantityField->GetValue().Trim();
  const int sectionIndex = m_sectionField->GetSelection();

  if (name.IsEmpty()) {
    return;
  }
  if (quantity.IsEmpty()) {
    return;
  }
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

void InputBar::onNameTextUpdate(wxCommandEvent &event) {
  updateAddButtonState();
}

void InputBar::onQuantityTextUpdate(wxCommandEvent &event) {
  updateAddButtonState();
}

void InputBar::onSectionSelectionUpdate(wxCommandEvent &event) {
  updateAddButtonState();
}

void InputBar::updateAddButtonState() {
    const bool nameFieldValid = !m_nameField->GetValue().IsEmpty();
    const bool quantityFieldValid = !m_quantityField->GetValue().IsEmpty();
    const bool sectionFieldValid = m_sectionField->GetSelection() > 0;
    m_addButton->Enable(nameFieldValid && quantityFieldValid &&
                        sectionFieldValid);
}
