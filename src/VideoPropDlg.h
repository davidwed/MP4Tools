/////////////////////////////////////////////////////////////////////////////
// Name:        VideoPropDlg.h
// Purpose:     The video properties dialog
// Author:      Alex Thuering
// Created:     31.05.2015
// RCS-ID:      $Id: $
// Copyright:   (c) Alex Thuering
// Licence:     GPL
/////////////////////////////////////////////////////////////////////////////
#ifndef VIDEO_PROP_DLG_H
#define VIDEO_PROP_DLG_H

#include "MediaFile.h"
#include "mediactrl_ffmpeg.h"

//(*Headers(VideoPropDlg)
#include <wx/bmpbuttn.h>
#include <wx/dialog.h>
#include <wx/sizer.h>
#include <wx/slider.h>
#include <wx/spinbutt.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
//*)

class VideoPropDlg: public wxDialog {
public:
	/** Constructor */
	VideoPropDlg(wxWindow* parent, MediaFile* mediaFile);
	virtual ~VideoPropDlg();

    /** Displays dialog */
	virtual int ShowModal();

private:
	//(*Declarations(VideoPropDlg)
	MediaCtrlFF* m_mediaCtrl;
	wxBitmapButton* m_endBt;
	wxBitmapButton* m_startBt;
	wxSlider* m_slider;
	wxSpinButton* m_endFrameSpin;
	wxSpinButton* m_endSpin;
	wxSpinButton* m_frameSpinBt;
	wxSpinButton* m_startFrameSpin;
	wxSpinButton* m_startSpin;
	wxSpinButton* m_timeSpinBt;
	wxTextCtrl* m_endCtrl;
	wxTextCtrl* m_startCtrl;
	wxTextCtrl* m_timeCtrl;
	//*)

	//(*Identifiers(VideoPropDlg)
	static const long ID_CUSTOM1;
	static const long ID_SLIDER;
	static const long ID_TIME_CTRL;
	static const long ID_TIME_SPINB;
	static const long ID_FRAME_SPINBT;
	static const long ID_TEXTCTRL2;
	static const long ID_SPINBUTTON1;
	static const long ID_SPINBUTTON2;
	static const long START_BT_ID;
	static const long ID_TEXTCTRL3;
	static const long ID_SPINBUTTON3;
	static const long ID_SPINBUTTON4;
	static const long ID_RESET_TIME_BT;
	//*)

	MediaFile* mediaFile;
	long m_videoPos;
	void SeekVideo(long pos, bool updateTimeCtrl = true);
	//(*Handlers(VideoPropDlg)
	void OnChangeTime(wxCommandEvent& event);
	void OnStartBt(wxCommandEvent& event);
	void OnEndBt(wxCommandEvent& event);
	void OnSliderScroll(wxScrollEvent& event);
	void OnTimeSpin(wxSpinEvent& event);
	void OnOkBt(wxCommandEvent& event);
	void OnFrameSpin(wxSpinEvent& event);
	void OnStartSpin(wxSpinEvent& event);
	void OnStartFrameSpin(wxSpinEvent& event);
	void OnEndSpin(wxSpinEvent& event);
	void OnEndFrameSpin(wxSpinEvent& event);
	void OnFrameSpinDown(wxSpinEvent& event);
	void OnStartFrameSpinDown(wxSpinEvent& event);
	void OnEndFrameSpinDown(wxSpinEvent& event);
	void OnStartSpinDown(wxSpinEvent& event);
	void OnEndSpinDown(wxSpinEvent& event);
	//*)
	DECLARE_EVENT_TABLE()
};

#endif // VIDEO_PROP_DLG_H
