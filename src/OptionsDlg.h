/////////////////////////////////////////////////////////////////////////////
// Name:        OptionsDlg.h
// Author:      Alex Thuering
// Created:     20.03.2016
// RCS-ID:      $Id: $
// Copyright:   (c) Alex Thuering
// Licence:     GPL
/////////////////////////////////////////////////////////////////////////////

#ifndef OPTIONS_DIALOG_H
#define OPTIONS_DIALOG_H

//(*Headers(OptionsDlg)
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/checkbox.h>
#include <wx/choice.h>
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/combobox.h>
//*)

#include <wx/bmpcbox.h>

const int MIN_CRF = 18;
const int MAX_CRF = 28;
const int DEF_CRF = 23;

/**
 * Options dialog
 */
class OptionsDlg: public wxDialog {
public:
	OptionsDlg(wxWindow* parent, bool disableEncodingOptions);
	virtual ~OptionsDlg();

    /** Displays dialog */
	virtual int ShowModal();
	/** Returns true, if option to force reencoding of audio streams is enabled */
	bool IsForceReencodeAudio() { return audioReencode->IsChecked(); }
	/** Set the option to force reencoding of audio streams */
	void SetForceReencodeAudio(bool value) { audioReencode->SetValue(value); }
	
	/** Returns true, if option to force reencoding of video streams is enabled */
	bool IsForceReencodeVideo() { return videoReencode->IsChecked(); }
	/** Set the option to force reencoding of video streams */
	void SetForceReencodeVideo(bool value) { videoReencode->SetValue(value); }
	
	/** Returns Constant Rate Factor (quality of video) */
	int GetCrf() { return qualityChoice->GetSelection() + MIN_CRF; }
	/** Set Constant Rate Factor (quality of video) */
	void SetCrf(int value) { qualityChoice->SetSelection(value - MIN_CRF); }
	
	/** Returns preset for video encoding */
	int GetPreset() { return presetChoice->GetSelection(); }
	/** Set Constant Rate Factor (quality of video) */
	void SetPreset(int value) { presetChoice->SetSelection(value); }
	
	/** Returns log file name */
	wxString GetLogFile() { return logFileCtrl->GetValue(); }
	/** Set Constant Rate Factor (quality of video) */
	void SetLogFile(const wxString& value) { logFileCtrl->SetValue(value); }
	
private:
	//(*Declarations(OptionsDlg)
	wxTextCtrl* logFileCtrl;
	wxChoice* presetChoice;
	wxChoice* qualityChoice;
	wxCheckBox* audioReencode;
	wxCheckBox* videoReencode;
	wxBitmapComboBox* langaugeCombo;
	//*)

	//(*Identifiers(OptionsDlg)
	static const long ID_LANG_COMBO;
	static const long ID_LOGFILE_TEXTCTRL;
	static const long ID_LOGFILE_BT;
	static const long ID_VIDEO_CHECK;
	static const long ID_CHOICE1;
	static const long ID_CHOICE2;
	static const long ID_AUDIO_CHECK;
	//*)

	//(*Handlers(OptionsDlg)
	void OnSelectFile(wxCommandEvent& event);
	//*)

	DECLARE_EVENT_TABLE()
};

#endif
