/////////////////////////////////////////////////////////////////////////////
// Name:      Languages.h
// Author:    Alex Thuering
// Created:   14.07.2012
// Copyright: (c) Alex Thuering
// Licence:   GPL
/////////////////////////////////////////////////////////////////////////////

#ifndef LANGUAGES_H_
#define LANGUAGES_H_

#include <wx/wx.h>
#include <wx/dynarray.h>
#include <wx/bitmap.h>
WX_DECLARE_OBJARRAY(wxBitmap, BitmapArray);

int ChooseLanguage(int defLanguage);
const wxArrayString& GetLangNames();
wxString GetLangName(wxString langCode);
wxString GetLangCode(int index);
const BitmapArray& GetLangBitmaps();

#endif // LANGUAGES_H_
