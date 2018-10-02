/////////////////////////////////////////////////////////////////////////////
// Name:      MainWin.h
// Author:    Alex Thuering
// Created:   7.07.2012
// Copyright: (c) Alex Thuering
// Licence:   GPL
/////////////////////////////////////////////////////////////////////////////
#ifndef MAINWIN_H
#define MAINWIN_H

//(*Headers(MainWin)
#include <wx/bmpbuttn.h>
#include <wx/frame.h>
#include <wx/listbox.h>
#include <wx/sizer.h>
#include <wx/statusbr.h>
#include <wx/toolbar.h>
//*)
#include "MediaListBox.h"
#include <wx/ffile.h>

/**
 * Main window
 */
class MainWin: public wxFrame {
public:
	MainWin();
	virtual ~MainWin();
	
	vector<MediaFile*>& GetFiles() { return files; }

private:

	//(*Declarations(MainWin)
	MediaListBox* mediaListBox;
	wxBitmapButton* downButton;
	wxBitmapButton* upButton;
	wxStatusBar* statusBar;
	wxToolBar* toolbar;
	//*)

	//(*Identifiers(MainWin)
	static const long ID_MEDIA_LISTBOX;
	static const long ID_BITMAPBUTTON1;
	static const long ID_BITMAPBUTTON2;
	static const long ID_TOOLBAR;
	static const long ID_STATUSBAR;
	//*)

	//(*Handlers(MainWin)
	void OnMediaSelect(wxCommandEvent& event);
	void OnUpBt(wxCommandEvent& event);
	void OnDownBt(wxCommandEvent& event);
	void OnMediaDClick(wxCommandEvent& event);
	//*)
	void OnAddFileBt(wxCommandEvent& event);
	void OnRemoveFileBt(wxCommandEvent& event);
	void OnRemoveAllBt(wxCommandEvent& event);
	void OnRunBt(wxCommandEvent& event);
	void OnSettingsBt(wxCommandEvent& event);
	void OnAboutBt(wxCommandEvent& event);
	void OnClose(wxCloseEvent &event);
	void OnRightClick(wxContextMenuEvent &event);
	void OnProperties(wxCommandEvent& event);
	void OnSort(wxCommandEvent& event);
	
	void UpdateToolBar();

	vector<MediaFile*> files;
	bool forceReencodeAudio;
	bool forceReencodeVideo;
	int crf; // Constant Rate Factor (quality of video)
	int preset; // preset is a collection of options that will provide a certain encoding speed to compression ratio
	wxString logFileName;
	
	DECLARE_EVENT_TABLE()
};

#endif
