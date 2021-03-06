/////////////////////////////////////////////////////////////////////////////
// Name:        OptionsDlg.cpp
// Author:      Alex Thuering
// Created:     20.03.2016
// RCS-ID:      $Id: $
// Copyright:   (c) Alex Thuering
// Licence:     GPL
/////////////////////////////////////////////////////////////////////////////

#include "OptionsDlg.h"
#include "Languages.h"
#include "Config.h"

//(*InternalHeaders(OptionsDlg)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(OptionsDlg)
const long OptionsDlg::ID_LANG_COMBO = wxNewId();
const long OptionsDlg::ID_LOGFILE_TEXTCTRL = wxNewId();
const long OptionsDlg::ID_LOGFILE_BT = wxNewId();
const long OptionsDlg::ID_VIDEO_CHECK = wxNewId();
const long OptionsDlg::ID_CHOICE1 = wxNewId();
const long OptionsDlg::ID_CHOICE2 = wxNewId();
const long OptionsDlg::ID_AUDIO_CHECK = wxNewId();
//*)

BEGIN_EVENT_TABLE(OptionsDlg,wxDialog)
	//(*EventTable(OptionsDlg)
	//*)
END_EVENT_TABLE()

OptionsDlg::OptionsDlg(wxWindow* parent, bool disableEncodingOptions) {
	//(*Initialize(OptionsDlg)
	wxStaticText* presetLabel;
	wxStaticText* qualityLabel;
	wxFlexGridSizer* gridSizer;
	wxStaticText* StaticText1;
	wxBoxSizer* BoxSizer2;
	wxStaticText* label1;
	wxBoxSizer* BoxSizer1;
	wxButton* logFileBt;
	wxBoxSizer* mainSizer;
	wxStdDialogButtonSizer* StdDialogButtonSizer1;

	Create(parent, wxID_ANY, _("Options"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("wxID_ANY"));
	mainSizer = new wxBoxSizer(wxVERTICAL);
	gridSizer = new wxFlexGridSizer(0, 2, 0, 0);
	label1 = new wxStaticText(this, wxID_ANY, _("Language:"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	gridSizer->Add(label1, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	langaugeCombo = new wxBitmapComboBox(this, ID_LANG_COMBO, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, 0, wxCB_READONLY, wxDefaultValidator, _T("ID_LANG_COMBO"));
	gridSizer->Add(langaugeCombo, 1, wxALL|wxEXPAND, 5);
	StaticText1 = new wxStaticText(this, wxID_ANY, _("Log file:"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	gridSizer->Add(StaticText1, 0, wxBOTTOM|wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
	logFileCtrl = new wxTextCtrl(this, ID_LOGFILE_TEXTCTRL, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_LOGFILE_TEXTCTRL"));
	BoxSizer2->Add(logFileCtrl, 1, wxRIGHT|wxEXPAND, 2);
	logFileBt = new wxButton(this, ID_LOGFILE_BT, _("..."), wxDefaultPosition, wxSize(20,20), 0, wxDefaultValidator, _T("ID_LOGFILE_BT"));
	BoxSizer2->Add(logFileBt, 0, wxEXPAND, 0);
	gridSizer->Add(BoxSizer2, 1, wxBOTTOM|wxLEFT|wxRIGHT|wxEXPAND, 5);
	gridSizer->Add(8,8,1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	videoReencode = new wxCheckBox(this, ID_VIDEO_CHECK, _("force reencode video"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_VIDEO_CHECK"));
	videoReencode->SetValue(false);
	gridSizer->Add(videoReencode, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	gridSizer->Add(8,8,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer1->Add(8,8,0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	qualityLabel = new wxStaticText(this, wxID_ANY, _("CRF (quality):"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	BoxSizer1->Add(qualityLabel, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	qualityChoice = new wxChoice(this, ID_CHOICE1, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE1"));
	BoxSizer1->Add(qualityChoice, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	presetLabel = new wxStaticText(this, wxID_ANY, _("Preset:"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	BoxSizer1->Add(presetLabel, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	presetChoice = new wxChoice(this, ID_CHOICE2, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE2"));
	presetChoice->Append(_("veryslow"));
	presetChoice->Append(_("slower"));
	presetChoice->Append(_("slow"));
	presetChoice->SetSelection( presetChoice->Append(_("medium")) );
	presetChoice->Append(_("fast"));
	presetChoice->Append(_("faster"));
	presetChoice->Append(_("veryfast"));
	presetChoice->Append(_("superfast"));
	presetChoice->Append(_("ultrafast"));
	BoxSizer1->Add(presetChoice, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	gridSizer->Add(BoxSizer1, 1, wxRIGHT|wxEXPAND, 5);
	gridSizer->Add(8,8,1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	audioReencode = new wxCheckBox(this, ID_AUDIO_CHECK, _("force reencode audio"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_AUDIO_CHECK"));
	audioReencode->SetValue(false);
	gridSizer->Add(audioReencode, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	mainSizer->Add(gridSizer, 1, wxEXPAND, 5);
	StdDialogButtonSizer1 = new wxStdDialogButtonSizer();
	StdDialogButtonSizer1->AddButton(new wxButton(this, wxID_OK, wxEmptyString));
	StdDialogButtonSizer1->AddButton(new wxButton(this, wxID_CANCEL, wxEmptyString));
	StdDialogButtonSizer1->Realize();
	mainSizer->Add(StdDialogButtonSizer1, 0, wxALL|wxEXPAND, 5);
	SetSizer(mainSizer);
	mainSizer->Fit(this);
	mainSizer->SetSizeHints(this);
	Center();

	Connect(ID_LOGFILE_BT,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&OptionsDlg::OnSelectFile);
	//*)
	const wxArrayString& langNames = GetLangNames();
	for (unsigned int i = 0; i < langNames.size(); i++) {
		langaugeCombo->Append(langNames[i], GetLangBitmaps()[i]);
	}
	for (unsigned int i = MIN_CRF; i <= MAX_CRF; i++) {
		wxString label = wxString::Format("%d", i);
		if (i == MIN_CRF)
			label += " (" + _("best") + ")";
		else if (i == MAX_CRF)
			label += " (" + _("worst") + ")";
		else if (i == DEF_CRF)
			label += " (" + _("default") + ")";
		qualityChoice->Append(label);
	}
	if (disableEncodingOptions) {
		videoReencode->Enable(false);
		qualityLabel->Enable(false);
		qualityChoice->Enable(false);
		presetLabel->Enable(false);
		presetChoice->Enable(false);
		audioReencode->Enable(false);
	}
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
			wxMessageBox(_("Language change will not take effect until the application is restarted"),
					GetTitle(), wxOK|wxICON_INFORMATION, this);
		}
	}
	return res;
}

void OptionsDlg::OnSelectFile(wxCommandEvent& event) {
	wxFileDialog dialog(this, _("Save a log file"), "", "mp4tools.log", "Log files (*.log)|*.log", wxFD_SAVE);
	if (dialog.ShowModal() == wxID_OK)
		logFileCtrl->SetValue(dialog.GetPath());
}
