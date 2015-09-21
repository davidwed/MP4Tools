/////////////////////////////////////////////////////////////////////////////
// Name:      Languages.h
// Author:    Alex Thuering
// Created:   14.07.2012
// Copyright: (c) Alex Thuering
// Licence:   GPL
/////////////////////////////////////////////////////////////////////////////

#include "Languages.h"
#include "utils.h"
#include <wx/defs.h>
#include <wx/button.h>
#include <wx/choice.h>
#include <wx/dialog.h>
#include <wx/intl.h>
#include <wx/sizer.h>
#include <wx/stattext.h>

#include <wx/arrimpl.cpp>
WX_DEFINE_OBJARRAY(BitmapArray);

// languages supported by MP4Joiner
#include "langs.inc"

wxArrayString s_langNamesArr;
const wxArrayString& GetLangNames() {
	if (s_langNamesArr.size() == 0)
		for (int i = 0; i < (int) WXSIZEOF(s_langs); i++)
			s_langNamesArr.Add(wxString(s_langs[i].name, wxConvUTF8));
	return s_langNamesArr;
}

wxString GetLangCode(int index) {
	return wxLocale::GetLanguageInfo(s_langs[index].id)->CanonicalName;
}

wxString GetLangName(wxString langCode) {
	int langId = wxLANGUAGE_ENGLISH;
	if (wxLocale::FindLanguageInfo(langCode))
		langId = wxLocale::FindLanguageInfo(langCode)->Language;
	for (unsigned int i = 0; i < GetLangNames().size(); i++)
		if (langId == s_langs[i].id)
			return GetLangNames()[i];
	return GetLangName(wxT("EN"));
}

int ChooseLanguage(int defLanguage) {
	wxSingleChoiceDialog dlg(NULL, _T("Choose language to use:"),
			_T("Language selection"), GetLangNames());
	for (int i = 0; i < (int) WXSIZEOF(s_langs); i++)
		if (s_langs[i].id == defLanguage)
			dlg.SetSelection(i);
	if (dlg.ShowModal() == wxID_OK)
		return s_langs[dlg.GetSelection()].id;
	return wxLANGUAGE_UNKNOWN;
}

BitmapArray s_langBitmapArray;

const BitmapArray& GetLangBitmaps() {
	if (s_langBitmapArray.size() == 0)
		for (int i = 0; i < (int) WXSIZEOF(s_langs); i++)
			s_langBitmapArray.Add(wxGetBitmapFromMemory(s_langs[i].bitmap, s_langs[i].bmpLength));
	return s_langBitmapArray;
}
