/////////////////////////////////////////////////////////////////////////////
// Name:      mp4joiner.cpp
// Author:    Alex Thuering
// Created:   7.07.2012
// Copyright: (c) Alex Thuering
// Licence:   GPL
/////////////////////////////////////////////////////////////////////////////
#include <wx/wxprec.h>

#ifndef WX_PRECOMP
    #include <wx/app.h>
#endif

//(*AppHeaders
#include "MainWin.h"
#include <wx/image.h>
//*)
#include "Config.h"
#include "Languages.h"
#include "utils.h"
#include <wxSVG/mediadec_ffmpeg.h>
#include <wx/cmdline.h>
#include <wx/config.h>
#include <wx/filesys.h>
#include <wx/fs_zip.h>
#include <locale.h>
#include "../resources/mp4joiner.png.h"

#define LOCALE_DIR wxFindDataDirectory(_T("..") + wxString(wxFILE_SEP_PATH) + _T("locale"))

/**
 * Applicatin main class
 */
class MP4Joiner : public wxApp {
public:
	virtual bool OnInit();

private:
	wxLocale m_locale;
};

IMPLEMENT_APP(MP4Joiner);

bool MP4Joiner::OnInit() {
	wxGetApp();
	s_config.Init();
	
	wxFileSystem::AddHandler(new wxZipFSHandler);
    wxInitAllImageHandlers();
    
#if wxDEBUG_LEVEL > 0
	wxDisableAsserts();
#endif


    // locale
	int lang = wxLANGUAGE_ENGLISH;
	wxString languageCode = s_config.GetLanguageCode();
	if (languageCode.length() > 0 && wxLocale::FindLanguageInfo(languageCode)) {
		lang = wxLocale::FindLanguageInfo(languageCode)->Language;
	} else {
		lang = ChooseLanguage(wxLocale::GetSystemLanguage());
		if (lang == wxLANGUAGE_UNKNOWN)
			lang = wxLANGUAGE_ENGLISH;
		languageCode = wxLocale::GetLanguageInfo(lang)->CanonicalName;
	}
	if (lang != wxLANGUAGE_ENGLISH) {
		m_locale.Init(lang);
		m_locale.AddCatalogLookupPathPrefix(LOCALE_DIR);
		m_locale.AddCatalog(wxT("mp4tools"));
		m_locale.AddCatalog(wxT("wxstd"));
	}
	
	setlocale(LC_NUMERIC, "C");
	wxFfmpegMediaDecoder::Init();

    MainWin* mainWin = new MainWin();
#ifndef __WXMSW__
	mainWin->SetIcon(wxICON_FROM_MEMORY(mp4joiner));
#else
	mainWin->SetIcon(wxICON(mp4joiner));
#endif
    mainWin->Show();
    SetTopWindow(mainWin);
    
    s_config.SetLanguageCode(languageCode); // save choosed languages if all ok
    return true;
}

