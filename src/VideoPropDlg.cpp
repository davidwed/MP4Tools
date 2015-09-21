/////////////////////////////////////////////////////////////////////////////
// Name:        VideoPropDlg.cpp
// Purpose:     The video properties dialog
// Author:      Alex Thuering
// Created:     31.05.2015
// RCS-ID:      $Id: $
// Copyright:   (c) Alex Thuering
// Licence:     GPL
/////////////////////////////////////////////////////////////////////////////

#include "VideoPropDlg.h"
#include "utils.h"
#include <wx/artprov.h>
#include <wx/regex.h>

//(*InternalHeaders(VideoPropDlg)
#include <wx/artprov.h>
#include <wx/bitmap.h>
#include <wx/intl.h>
#include <wx/button.h>
#include <wx/image.h>
#include <wx/string.h>
//*)

//(*IdInit(VideoPropDlg)
const long VideoPropDlg::ID_CUSTOM1 = wxNewId();
const long VideoPropDlg::ID_SLIDER = wxNewId();
const long VideoPropDlg::ID_TIME_CTRL = wxNewId();
const long VideoPropDlg::ID_TIME_SPINB = wxNewId();
const long VideoPropDlg::ID_FRAME_SPINBT = wxNewId();
const long VideoPropDlg::ID_TEXTCTRL2 = wxNewId();
const long VideoPropDlg::ID_SPINBUTTON1 = wxNewId();
const long VideoPropDlg::ID_SPINBUTTON2 = wxNewId();
const long VideoPropDlg::START_BT_ID = wxNewId();
const long VideoPropDlg::ID_TEXTCTRL3 = wxNewId();
const long VideoPropDlg::ID_SPINBUTTON3 = wxNewId();
const long VideoPropDlg::ID_SPINBUTTON4 = wxNewId();
const long VideoPropDlg::ID_RESET_TIME_BT = wxNewId();
//*)

BEGIN_EVENT_TABLE(VideoPropDlg,wxDialog)
	//(*EventTable(VideoPropDlg)
	//*)
	EVT_BUTTON(wxID_OK, VideoPropDlg::OnOkBt)
	EVT_COMMAND_SCROLL(ID_SLIDER, VideoPropDlg::OnSliderScroll)
END_EVENT_TABLE()

VideoPropDlg::VideoPropDlg(wxWindow* parent, MediaFile* mediaFile) {
	this->mediaFile = mediaFile;
	m_videoPos = 0;

	//(*Initialize(VideoPropDlg)
	wxBoxSizer* mediaSizer;
	wxStdDialogButtonSizer* stdDialogButtonSizer;
	wxStaticText* staticText1;
	wxStaticText* startLabel;
	wxStaticText* endLabel;
	wxBoxSizer* timeSizer;
	wxBoxSizer* mainSizer;
	wxBoxSizer* hSizer;

	Create(parent, wxID_ANY, _("Cut video"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER, _T("wxID_ANY"));
	SetClientSize(wxSize(750,500));
	mainSizer = new wxBoxSizer(wxVERTICAL);
	hSizer = new wxBoxSizer(wxHORIZONTAL);
	mediaSizer = new wxBoxSizer(wxVERTICAL);
	m_mediaCtrl = new MediaCtrlFF(this, ID_CUSTOM1, wxT(""), wxDefaultPosition,wxDefaultSize, 0, wxDefaultValidator, _T("ID_CUSTOM1"));
	m_mediaCtrl->SetMinSize(wxSize(300, 200));
	m_mediaCtrl->SetWindowStyle(wxBORDER_NONE);
	mediaSizer->Add(m_mediaCtrl, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 4);
	m_slider = new wxSlider(this, ID_SLIDER, 0, 0, 100, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_SLIDER"));
	mediaSizer->Add(m_slider, 0, wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	timeSizer = new wxBoxSizer(wxHORIZONTAL);
	staticText1 = new wxStaticText(this, wxID_ANY, _("Time:"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	timeSizer->Add(staticText1, 0, wxLEFT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 4);
	m_timeCtrl = new wxTextCtrl(this, ID_TIME_CTRL, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TIME_CTRL"));
	timeSizer->Add(m_timeCtrl, 0, wxLEFT|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	m_timeSpinBt = new wxSpinButton(this, ID_TIME_SPINB, wxDefaultPosition, wxDefaultSize, wxSP_VERTICAL|wxSP_ARROW_KEYS, _T("ID_TIME_SPINB"));
	m_timeSpinBt->SetRange(0, 100);
	m_timeSpinBt->SetMinSize(wxSize(16,12));
	timeSizer->Add(m_timeSpinBt, 0, wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	m_frameSpinBt = new wxSpinButton(this, ID_FRAME_SPINBT, wxDefaultPosition, wxDefaultSize, wxSP_VERTICAL|wxSP_ARROW_KEYS|wxSP_WRAP, _T("ID_FRAME_SPINBT"));
	m_frameSpinBt->SetRange(-9999, 9999);
	m_frameSpinBt->SetMinSize(wxSize(16,12));
	timeSizer->Add(m_frameSpinBt, 0, wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	startLabel = new wxStaticText(this, wxID_ANY, _("Start:"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	timeSizer->Add(startLabel, 0, wxLEFT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 8);
	m_startCtrl = new wxTextCtrl(this, ID_TEXTCTRL2, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL2"));
	timeSizer->Add(m_startCtrl, 0, wxLEFT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	m_startSpin = new wxSpinButton(this, ID_SPINBUTTON1, wxDefaultPosition, wxDefaultSize, wxSP_VERTICAL|wxSP_ARROW_KEYS|wxSP_WRAP, _T("ID_SPINBUTTON1"));
	m_startSpin->SetRange(-9999, 9999);
	m_startSpin->SetMinSize(wxSize(16,12));
	timeSizer->Add(m_startSpin, 0, wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	m_startFrameSpin = new wxSpinButton(this, ID_SPINBUTTON2, wxDefaultPosition, wxDefaultSize, wxSP_VERTICAL|wxSP_ARROW_KEYS|wxSP_WRAP, _T("ID_SPINBUTTON2"));
	m_startFrameSpin->SetRange(-9999, 9999);
	m_startFrameSpin->SetMinSize(wxSize(16,12));
	timeSizer->Add(m_startFrameSpin, 0, wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	m_startBt = new wxBitmapButton(this, START_BT_ID, wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_CUT")),wxART_MENU), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW, wxDefaultValidator, _T("START_BT_ID"));
	m_startBt->SetDefault();
	m_startBt->SetToolTip(_("Cut beginning"));
	timeSizer->Add(m_startBt, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	endLabel = new wxStaticText(this, wxID_ANY, _("End:"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	timeSizer->Add(endLabel, 0, wxLEFT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	m_endCtrl = new wxTextCtrl(this, ID_TEXTCTRL3, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL3"));
	timeSizer->Add(m_endCtrl, 0, wxLEFT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	m_endSpin = new wxSpinButton(this, ID_SPINBUTTON3, wxDefaultPosition, wxDefaultSize, wxSP_VERTICAL|wxSP_ARROW_KEYS|wxSP_WRAP, _T("ID_SPINBUTTON3"));
	m_endSpin->SetRange(-9999, 9999);
	m_endSpin->SetMinSize(wxSize(16,12));
	timeSizer->Add(m_endSpin, 0, wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	m_endFrameSpin = new wxSpinButton(this, ID_SPINBUTTON4, wxDefaultPosition, wxDefaultSize, wxSP_VERTICAL|wxSP_ARROW_KEYS|wxSP_WRAP, _T("ID_SPINBUTTON4"));
	m_endFrameSpin->SetRange(-9999, 9999);
	m_endFrameSpin->SetMinSize(wxSize(16,12));
	timeSizer->Add(m_endFrameSpin, 0, wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	m_endBt = new wxBitmapButton(this, ID_RESET_TIME_BT, wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_CUT")),wxART_MENU), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW, wxDefaultValidator, _T("ID_RESET_TIME_BT"));
	m_endBt->SetDefault();
	m_endBt->SetToolTip(_("Cut ending"));
	timeSizer->Add(m_endBt, 0, wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	mediaSizer->Add(timeSizer, 0, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	hSizer->Add(mediaSizer, 1, wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	mainSizer->Add(hSizer, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	stdDialogButtonSizer = new wxStdDialogButtonSizer();
	stdDialogButtonSizer->AddButton(new wxButton(this, wxID_OK, wxEmptyString));
	stdDialogButtonSizer->AddButton(new wxButton(this, wxID_CANCEL, wxEmptyString));
	stdDialogButtonSizer->Realize();
	mainSizer->Add(stdDialogButtonSizer, 0, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 4);
	SetSizer(mainSizer);
	SetSizer(mainSizer);
	Layout();
	Center();

	Connect(ID_TIME_CTRL,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&VideoPropDlg::OnChangeTime);
	Connect(ID_TIME_SPINB,wxEVT_SCROLL_THUMBTRACK,(wxObjectEventFunction)&VideoPropDlg::OnTimeSpin);
	Connect(ID_FRAME_SPINBT,wxEVT_SCROLL_LINEUP,(wxObjectEventFunction)&VideoPropDlg::OnFrameSpin);
	Connect(ID_FRAME_SPINBT,wxEVT_SCROLL_LINEDOWN,(wxObjectEventFunction)&VideoPropDlg::OnFrameSpinDown);
	Connect(ID_SPINBUTTON1,wxEVT_SCROLL_LINEUP,(wxObjectEventFunction)&VideoPropDlg::OnStartSpin);
	Connect(ID_SPINBUTTON1,wxEVT_SCROLL_LINEDOWN,(wxObjectEventFunction)&VideoPropDlg::OnStartSpinDown);
	Connect(ID_SPINBUTTON2,wxEVT_SCROLL_LINEUP,(wxObjectEventFunction)&VideoPropDlg::OnStartFrameSpin);
	Connect(ID_SPINBUTTON2,wxEVT_SCROLL_LINEDOWN,(wxObjectEventFunction)&VideoPropDlg::OnStartFrameSpinDown);
	Connect(START_BT_ID,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&VideoPropDlg::OnStartBt);
	Connect(ID_SPINBUTTON3,wxEVT_SCROLL_LINEUP,(wxObjectEventFunction)&VideoPropDlg::OnEndSpin);
	Connect(ID_SPINBUTTON3,wxEVT_SCROLL_LINEDOWN,(wxObjectEventFunction)&VideoPropDlg::OnEndSpinDown);
	Connect(ID_SPINBUTTON4,wxEVT_SCROLL_LINEUP,(wxObjectEventFunction)&VideoPropDlg::OnEndFrameSpin);
	Connect(ID_SPINBUTTON4,wxEVT_SCROLL_LINEDOWN,(wxObjectEventFunction)&VideoPropDlg::OnEndFrameSpinDown);
	Connect(ID_RESET_TIME_BT,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&VideoPropDlg::OnEndBt);
	//*)

    int x = 0;
	m_timeCtrl->GetTextExtent(wxT("00:00:00.000"), &x, NULL);
	m_timeCtrl->SetMinSize(wxSize(x + 10, -1));
	m_startCtrl->SetMinSize(wxSize(x + 10, -1));
	m_endCtrl->SetMinSize(wxSize(x + 10, -1));

    // start time
    int ms = lround(mediaFile->GetStartTime() * 1000);
    ms = ms % 1000;
	long t = (long) mediaFile->GetStartTime();
	m_startCtrl->SetValue(wxString::Format(wxT("%d:%02d:%02d.%03d"), t/3600, (t/60)%60, t%60, ms));

	// end time
	if (mediaFile->GetRecordingTime() > 0) {
		ms = lround((mediaFile->GetStartTime() + mediaFile->GetRecordingTime()) * 1000);
		ms = ms % 1000;
		t = (long) (mediaFile->GetStartTime() + mediaFile->GetRecordingTime());
		m_endCtrl->SetValue(wxString::Format(wxT("%d:%02d:%02d.%03d"), t/3600, (t/60) % 60, t%60, ms));
	}
}

VideoPropDlg::~VideoPropDlg() {
	//(*Destroy(VideoPropDlg)
	//*)
}

/**
 * Displays dialog
 */
int VideoPropDlg::ShowModal() {
	if (!m_mediaCtrl->Load(mediaFile->GetFileName()))
		return wxID_CANCEL;
	m_slider->SetMax(m_mediaCtrl->Length()/1000);
	m_timeSpinBt->SetMax(m_mediaCtrl->Length()/1000);
	wxScrollEvent scrlEvt;
	OnSliderScroll(scrlEvt);
	int res = wxDialog::ShowModal();
	if (res == wxID_OK) {
		double time = TimeToDouble(m_startCtrl->GetValue());
		mediaFile->SetStartTime(time >= 0 ? time : 0);
		time = TimeToDouble(m_endCtrl->GetValue());
		mediaFile->SetRecordingTime(time > mediaFile->GetStartTime() ? time - mediaFile->GetStartTime() : -1);
	}
	return res;
}

void VideoPropDlg::SeekVideo(long pos, bool updateTimeCtrl) {
	m_videoPos = pos;
	m_slider->SetValue(lround(pos / 1000));
	m_timeSpinBt->SetValue(lround(pos / 1000));
	m_mediaCtrl->Seek((wxFileOffset) pos);
	if (updateTimeCtrl)
		m_timeCtrl->ChangeValue(Time2String(pos, true));
}

void VideoPropDlg::OnSliderScroll(wxScrollEvent& event) {
	SeekVideo(((long)m_slider->GetValue())*1000);
}

void VideoPropDlg::OnTimeSpin(wxSpinEvent& event) {
	SeekVideo(((long)m_timeSpinBt->GetValue())*1000);
}

void VideoPropDlg::OnChangeTime(wxCommandEvent& event) {
	if (s_timeRE.Matches(m_timeCtrl->GetValue()))
		SeekVideo(String2Time(m_timeCtrl->GetValue()), false);
}

void VideoPropDlg::OnFrameSpin(wxSpinEvent& event) {
    SeekVideo(String2Time(m_timeCtrl->GetValue()) + 1000 / m_mediaCtrl->GetFps());
}

void VideoPropDlg::OnFrameSpinDown(wxSpinEvent& event) {
    long pos = String2Time(m_timeCtrl->GetValue()) - 1000 / m_mediaCtrl->GetFps();
    SeekVideo(pos >= 0 ? pos : 0);
}

void VideoPropDlg::OnStartSpin(wxSpinEvent& event) {
   	long pos = String2Time(m_startCtrl->GetValue());
   	pos = pos - pos % 1000 + 1000;
	m_startCtrl->ChangeValue(Time2String(pos, true));
}

void VideoPropDlg::OnStartSpinDown(wxSpinEvent& event) {
    long pos = String2Time(m_startCtrl->GetValue());
    pos = pos - pos % 1000 - (pos % 1000 > 0 ? 0 : 1000);
    m_startCtrl->ChangeValue(Time2String(pos >= 0 ? pos : 0, true));
}

void VideoPropDlg::OnStartFrameSpin(wxSpinEvent& event) {
	m_startCtrl->ChangeValue(Time2String(String2Time(m_startCtrl->GetValue()) + 1000 / m_mediaCtrl->GetFps(), true));
}

void VideoPropDlg::OnStartFrameSpinDown(wxSpinEvent& event) {
	long pos = String2Time(m_startCtrl->GetValue()) - 1000 / m_mediaCtrl->GetFps();
	m_startCtrl->ChangeValue(Time2String(pos >= 0 ? pos : 0, true));
}

void VideoPropDlg::OnEndSpin(wxSpinEvent& event) {
   	long pos = String2Time(m_endCtrl->GetValue());
   	pos = pos - pos % 1000 + 1000;
	m_endCtrl->ChangeValue(Time2String(pos, true));
}

void VideoPropDlg::OnEndSpinDown(wxSpinEvent& event) {
	long pos = String2Time(m_endCtrl->GetValue());
	pos = pos - pos % 1000 - (pos % 1000 > 0 ? 0 : 1000);
	m_endCtrl->ChangeValue(Time2String(pos >= 0 ? pos : 0, true));
}

void VideoPropDlg::OnEndFrameSpin(wxSpinEvent& event) {
	m_endCtrl->ChangeValue(Time2String(String2Time(m_endCtrl->GetValue()) + 1000 / m_mediaCtrl->GetFps(), true));
}

void VideoPropDlg::OnEndFrameSpinDown(wxSpinEvent& event) {
	long pos = String2Time(m_endCtrl->GetValue()) - 1000 / m_mediaCtrl->GetFps();
	m_endCtrl->ChangeValue(Time2String(pos >= 0 ? pos : 0, true));
}

void VideoPropDlg::OnStartBt(wxCommandEvent& event) {
	m_startCtrl->SetValue(Time2String(m_videoPos, true));
}

void VideoPropDlg::OnEndBt(wxCommandEvent& event) {
	if (m_slider->GetValue() < m_slider->GetMax()) {
		m_endCtrl->SetValue(Time2String(m_videoPos, true));
	} else
		m_endCtrl->SetValue(wxT(""));
}

void VideoPropDlg::OnOkBt(wxCommandEvent& event) {
	if (m_startCtrl->GetValue().length() > 0 && TimeToDouble(m_startCtrl->GetValue()) == -1) {
		wxLogError(_("Invalid time") + wxString(wxT(": ")) + m_startCtrl->GetValue());
		event.Skip();
		return;
	}
	if (m_endCtrl->GetValue().length() > 0 && TimeToDouble(m_endCtrl->GetValue()) == -1) {
		wxLogError(_("Invalid time") + wxString(wxT(": ")) + m_startCtrl->GetValue());
		event.Skip();
		return;
	}
	this->EndModal(wxID_OK);
}
