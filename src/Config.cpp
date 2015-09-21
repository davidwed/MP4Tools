/////////////////////////////////////////////////////////////////////////////
// Name:      Config.cpp
// Author:    Alex Thuering
// Created:   13.07.2012
// Copyright: (c) Alex Thuering
// Licence:   GPL
/////////////////////////////////////////////////////////////////////////////

#include "Config.h"
#include "utils.h"
#ifdef __WXMSW__
#include <wx/fileconf.h>
#endif

Config s_config;

void Config::Init() {
#ifdef __WXMSW__
	// check if INI file exists
	wxString fileName = wxGetAppPath() + wxT("..") + wxFILE_SEP_PATH + wxT("MP4Tools.ini");
	if (wxFileExists(fileName))
		wxConfig::Set(new wxFileConfig(wxT(""), wxT(""), fileName));
#endif
	cfg = wxConfig::Get();
}

bool Config::IsMainWinMaximized() {
	bool ret = false;
	cfg->Read(wxT("MainWin/maximized"), &ret);
	return ret;
}

wxRect Config::GetMainWinLocation() {
	wxRect rect;
	rect.x = cfg->Read(wxT("MainWin/x"), -1);
	rect.y = cfg->Read(wxT("MainWin/y"), -1);
	rect.width = cfg->Read(wxT("MainWin/width"), -1);
	rect.height = cfg->Read(wxT("MainWin/height"), -1);
	return rect;
}

void Config::SetMainWinLocation(wxRect rect, bool isMaximized) {
	cfg->Write(wxT("MainWin/maximized"), isMaximized);
	if (rect.width > 50 && rect.height > 50) {
		cfg->Write(wxT("MainWin/x"), rect.x);
		cfg->Write(wxT("MainWin/y"), rect.y);
		cfg->Write(wxT("MainWin/width"), rect.width);
		cfg->Write(wxT("MainWin/height"), rect.height);
	}
}

bool Config::IsSplitterWinMaximized() {
	bool ret = false;
	cfg->Read(wxT("SplitterWin/maximized"), &ret);
	return ret;
}

wxRect Config::GetSplitterWinLocation() {
	wxRect rect;
	rect.x = cfg->Read(wxT("SplitterWin/x"), -1);
	rect.y = cfg->Read(wxT("SplitterWin/y"), -1);
	rect.width = cfg->Read(wxT("SplitterWin/width"), -1);
	rect.height = cfg->Read(wxT("SplitterWin/height"), -1);
	return rect;
}

void Config::SetSplitterWinLocation(wxRect rect, bool isMaximized) {
	cfg->Write(wxT("SplitterWin/maximized"), isMaximized);
	if (rect.width > 50 && rect.height > 50) {
		cfg->Write(wxT("SplitterWin/x"), rect.x);
		cfg->Write(wxT("SplitterWin/y"), rect.y);
		cfg->Write(wxT("SplitterWin/width"), rect.width);
		cfg->Write(wxT("SplitterWin/height"), rect.height);
	}
}
