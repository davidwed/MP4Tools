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
#include <wx/checkbox.h>
#include <wx/dialog.h>
#include <wx/combobox.h>
//*)

#include <wx/bmpcbox.h>

/**
 * Options dialog
 */
class OptionsDlg: public wxDialog {
public:
	OptionsDlg(wxWindow* parent);
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
	
private:
	//(*Declarations(OptionsDlg)
	wxCheckBox* audioReencode;
	wxCheckBox* videoReencode;
	wxBitmapComboBox* langaugeCombo;
	//*)

	//(*Identifiers(OptionsDlg)
	static const long ID_STATICTEXT1;
	static const long ID_LANG_COMBO;
	static const long ID_VIDEO_CHECK;
	static const long ID_AUDIO_CHECK;
	//*)

	//(*Handlers(OptionsDlg)
	//*)

	DECLARE_EVENT_TABLE()
};

#endif
