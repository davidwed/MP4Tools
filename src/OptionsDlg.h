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
#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/choice.h>
#include <wx/combobox.h>
#include <wx/dialog.h>
#include <wx/sizer.h>
#include <wx/spinctrl.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
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
	OptionsDlg(wxWindow* parent, bool disableEncodingOptions, bool disableTempDir = true);
	virtual ~OptionsDlg();

    /** Displays dialog */
	virtual int ShowModal();
	
	/** Returns true, if splitting only on key frames is allowed */
	bool IsSplitOnKeyFrame() { return keyFrameCtrl->IsChecked(); }
	/** Sets, if splitting only on key frames is allowed */
	void SetSplitOnKeyFrame(bool value) { keyFrameCtrl->SetValue(value); }
	
	/** Returns true, if option to force reencoding of audio streams is enabled */
	bool IsForceReencodeAudio() { return audioReencodeCtrl->IsChecked(); }
	/** Sets the option to force reencoding of audio streams */
	void SetForceReencodeAudio(bool value) { audioReencodeCtrl->SetValue(value); }
	
	/** Returns true, if option to force reencoding of video streams is enabled */
	bool IsForceReencodeVideo() { return videoReencodeCtrl->IsChecked(); }
	/** Sets the option to force reencoding of video streams */
	void SetForceReencodeVideo(bool value) { videoReencodeCtrl->SetValue(value); }
	
	/** Returns Constant Rate Factor (quality of video) */
	int GetCrf() { return qualityChoice->GetSelection() + MIN_CRF; }
	/** Sets Constant Rate Factor (quality of video) */
	void SetCrf(int value) { qualityChoice->SetSelection(value - MIN_CRF); }
	
	/** Returns preset for video encoding */
	int GetPreset() { return presetChoice->GetSelection(); }
	/** Sets Constant Rate Factor (quality of video) */
	void SetPreset(int value) { presetChoice->SetSelection(value); }
	
	/** Returns audio sample rate */
	int GetSampleRate() { return sampleRateChoice->GetSelection(); }
	/** Sets audop sample rate  */
	void SetSampleRate(int value) { sampleRateChoice->SetSelection(value); }
	
	/** Returns audio bitrate */
	int GetAudioBitrate() { return audioBitrateCtrl->GetValue(); }
	/** Sets audio bitrate  */
	void SetAudioBitrate(int value) { audioBitrateCtrl->SetValue(value); }
	
	/** Returns log file name */
	wxString GetLogFile() { return logFileCtrl->GetValue(); }
	/** Set log file name */
	void SetLogFile(const wxString& value) { logFileCtrl->SetValue(value); }
	
private:
	//(*Declarations(OptionsDlg)
	wxBitmapComboBox* langaugeCombo;
	wxCheckBox* audioReencodeCtrl;
	wxCheckBox* keyFrameCtrl;
	wxCheckBox* videoReencodeCtrl;
	wxChoice* presetChoice;
	wxChoice* qualityChoice;
	wxChoice* sampleRateChoice;
	wxSpinCtrl* audioBitrateCtrl;
	wxStaticText* mp4boxParamsLabel;
	wxStaticText* tempDirLabel;
	wxTextCtrl* logFileCtrl;
	wxTextCtrl* mp4boxParamsCtrl;
	wxTextCtrl* tempDirCtrl;
	//*)

	//(*Identifiers(OptionsDlg)
	static const long ID_LANG_COMBO;
	static const long ID_STATICTEXT1;
	static const long ID_TEXTCTRL1;
	static const long ID_TMPDIR_TEXTCTRL;
	static const long ID_TEMPDIR_BT;
	static const long ID_LOGFILE_TEXTCTRL;
	static const long ID_LOGFILE_BT;
	static const long ID_CHECKBOX1;
	static const long ID_VIDEO_CHECK;
	static const long ID_CHOICE1;
	static const long ID_CHOICE2;
	static const long ID_AUDIO_CHECK;
	static const long ID_CHOICE3;
	static const long ID_SPINCTRL1;
	//*)

	//(*Handlers(OptionsDlg)
	void OnSelectFile(wxCommandEvent& event);
	void OnSelectTempDir(wxCommandEvent& event);
	//*)

	DECLARE_EVENT_TABLE()
};

#endif
