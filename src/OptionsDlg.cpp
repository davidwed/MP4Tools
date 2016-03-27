/////////////////////////////////////////////////////////////////////////////
// Name:        OptionsDlg.cpp
// Author:      Alex Thuering
// Created:     20.03.2016
// RCS-ID:      $Id: $
// Copyright:   (c) Alex Thuering
// Licence:     GPL
/////////////////////////////////////////////////////////////////////////////

#include <src/OptionsDlg.h>
#include "Languages.h"
#include "Config.h"

//(*InternalHeaders(OptionsDlg)
#include <wx/intl.h>
#include <wx/button.h>
#include <wx/string.h>
//*)

//(*IdInit(OptionsDlg)
const long OptionsDlg::ID_STATICTEXT1 = wxNewId();
const long OptionsDlg::ID_LANG_COMBO = wxNewId();
const long OptionsDlg::ID_VIDEO_CHECK = wxNewId();
const long OptionsDlg::ID_AUDIO_CHECK = wxNewId();
//*)

BEGIN_EVENT_TABLE(OptionsDlg,wxDialog)
	//(*EventTable(OptionsDlg)
	//*)
END_EVENT_TABLE()

OptionsDlg::OptionsDlg(wxWindow* parent) {
	//(*Initialize(OptionsDlg)
	wxFlexGridSizer* gridSizer;
	wxStaticText* label1;
	wxBoxSizer* mainSizer;
	wxStdDialogButtonSizer* StdDialogButtonSizer1;

	Create(parent, wxID_ANY, _("Options"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("wxID_ANY"));
	mainSizer = new wxBoxSizer(wxVERTICAL);
	gridSizer = new wxFlexGridSizer(0, 2, 0, 0);
	label1 = new wxStaticText(this, ID_STATICTEXT1, _("Language:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT1"));
	gridSizer->Add(label1, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	langaugeCombo = new wxBitmapComboBox(this, ID_LANG_COMBO, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, 0, wxCB_READONLY, wxDefaultValidator, _T("ID_LANG_COMBO"));
	gridSizer->Add(langaugeCombo, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	gridSizer->Add(8,8,1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	videoReencode = new wxCheckBox(this, ID_VIDEO_CHECK, _("force reencode video"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_VIDEO_CHECK"));
	videoReencode->SetValue(false);
	gridSizer->Add(videoReencode, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	gridSizer->Add(8,8,1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	audioReencode = new wxCheckBox(this, ID_AUDIO_CHECK, _("force reencode audio"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_AUDIO_CHECK"));
	audioReencode->SetValue(false);
	gridSizer->Add(audioReencode, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	mainSizer->Add(gridSizer, 1, wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StdDialogButtonSizer1 = new wxStdDialogButtonSizer();
	StdDialogButtonSizer1->AddButton(new wxButton(this, wxID_OK, wxEmptyString));
	StdDialogButtonSizer1->AddButton(new wxButton(this, wxID_CANCEL, wxEmptyString));
	StdDialogButtonSizer1->Realize();
	mainSizer->Add(StdDialogButtonSizer1, 0, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	SetSizer(mainSizer);
	mainSizer->Fit(this);
	mainSizer->SetSizeHints(this);
	Center();
	//*)
	const wxArrayString& langNames = GetLangNames();
	for (unsigned int i = 0; i < langNames.size(); i++)
		langaugeCombo->Append(langNames[i], GetLangBitmaps()[i]);
}

OptionsDlg::~OptionsDlg() {
	//(*Destroy(OptionsDlg)
	//*)
}

/**
 * Displays dialog
 */
int OptionsDlg::ShowModal() {
	wxString lang = GetLangName(s_config.GetLanguageCode());
	langaugeCombo->SetStringSelection(lang);
	int res = wxDialog::ShowModal();
	if (res == wxID_OK) {
		if (GetLangCode(langaugeCombo->GetSelection()) != s_config.GetLanguageCode()) {
			s_config.SetLanguageCode(GetLangCode(langaugeCombo->GetSelection()));
			wxMessageBox(_("Language change will not take effect until MP4Joiner is restarted"),
					GetTitle(), wxOK|wxICON_INFORMATION, this);
		}
	}
	return res;
}
