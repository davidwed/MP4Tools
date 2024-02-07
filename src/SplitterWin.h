/////////////////////////////////////////////////////////////////////////////
// Name:      SplitterWin.h
// Author:    Alex Thuering
// Created:   10.06.2015
// Copyright: (c) Alex Thuering
// Licence:   GPL
/////////////////////////////////////////////////////////////////////////////
#ifndef SPLITTERWIN_H
#define SPLITTERWIN_H

//(*Headers(SplitterWin)
#include <wx/bmpbuttn.h>
#include <wx/button.h>
#include <wx/frame.h>
#include <wx/listctrl.h>
#include <wx/sizer.h>
#include <wx/slider.h>
#include <wx/spinbutt.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
//*)
#include "CheckedListCtrl.h"
#include "MediaFile.h"
#include "mediactrl_ffmpeg.h"

/**
 * Splitter window
 */
class SplitterWin: public wxFrame {
public:
	SplitterWin();
	virtual ~SplitterWin();
	
	bool Open(wxString fileName);

private:
	//(*Declarations(SplitterWin)
	CheckedListCtrl* pointListCtrl;
	MediaCtrlFF* mediaCtrl;
	wxBitmapButton* aboutBt;
	wxBitmapButton* playBt;
	wxBitmapButton* settingsBt;
	wxButton* addSplitPointBt;
	wxButton* openBt;
	wxButton* startBt;
	wxSlider* mediaSlider;
	wxSpinButton* frameSpinBt;
	wxSpinButton* timeSpinBt;
	wxTextCtrl* timeCtrl;
	//*)

	//(*Identifiers(SplitterWin)
	static const long ID_MEDIA_CTRL;
	static const long ID_SLIDER;
	static const long ID_PLAY_BT;
	static const long ID_TIME_CTRL;
	static const long ID_TIME_SPIN;
	static const long ID_FRAME_SPIN;
	static const long ID_ADD_SPLIT_POINT_BT;
	static const long ID_OPEN_BT;
	static const long ID_SETTINGS_BT;
	static const long ID_ABOUT_BT;
	static const long ID_LISTCTRL1;
	static const long ID_START_BT;
	//*)

	//(*Handlers(SplitterWin)
	void OnChangeTime(wxCommandEvent& event);
	void OnTimeSpin(wxSpinEvent& event);
	void OnFrameSpin(wxSpinEvent& event);
	void OnFrameSpinDown(wxSpinEvent& event);
	void OnSliderScroll(wxScrollEvent& event);
	void OnAddCutPoint(wxCommandEvent& event);
	void OnCutPointRClick(wxListEvent& event);
	void OnSettingsBt(wxCommandEvent& event);
	void OnOpenFile(wxCommandEvent& event);
	void OnAboutBt(wxCommandEvent& event);
	void OnRunBt(wxCommandEvent& event);
	void OnPlay(wxCommandEvent& event);
	//*)
	void OnMediaStop(const wxMediaEvent& evt);
	void OnMediaUpdate(const wxMediaEvent& evt);
	void OnRemovePoint(wxCommandEvent& event);
	void OnRemoveAll(wxCommandEvent& event);
	void OnClose(wxCloseEvent &event);
	
	void UpdateControls();
	
	MediaFile mediaFile;
	long videoPos;
	bool splitOnKeyFrame;
	void SeekVideo(long pos, bool updateTimeCtrl = true);
	vector<long> cutPoints;
	wxString logFileName;

	DECLARE_EVENT_TABLE()
};

#endif
