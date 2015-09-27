/////////////////////////////////////////////////////////////////////////////
// Name:      utils.h
// Author:    Alex Thuering
// Created:   7.07.2012
// Copyright: (c) Alex Thuering
// Licence:   GPL
/////////////////////////////////////////////////////////////////////////////

#ifndef UTILS_H_
#define UTILS_H_

#include <wx/wx.h>
#include <wx/log.h>
#include <wx/mstream.h>
#include <wx/regex.h>
#include <vector>

using namespace std;

#define VECTOR_CLEAR(obj, elem_type) \
	for (vector<elem_type*>::iterator vectorIt = obj.begin(); vectorIt != obj.end(); vectorIt++) \
		delete *vectorIt; \
	obj.clear();

#define VECTOR_COPY(src, dst, elem_type) \
	for (vector<elem_type*>::const_iterator vectorIt = src.begin(); vectorIt != src.end(); vectorIt++) \
		dst.push_back(new elem_type(**vectorIt));

wxString wxGetAppPath();
void wxSetAppPath(wxString value);

wxString wxFindDataDirectory(wxString dir);
wxString wxFindDataFile(wxString filename);

#define wxBITMAP_FROM_MEMORY(name) wxGetBitmapFromMemory(name##_png, sizeof(name##_png))

inline wxBitmap wxGetBitmapFromMemory(const unsigned char *data, int length) {
   wxMemoryInputStream is(data, length);
   return wxBitmap(wxImage(is, wxBITMAP_TYPE_ANY, -1), -1);
}

#define wxIMAGE_FROM_MEMORY(name) wxGetImageFromMemory(name##_png, sizeof(name##_png))

inline wxImage wxGetImageFromMemory(const unsigned char *data, int length) {
   wxMemoryInputStream is(data, length);
   return wxImage(is, wxBITMAP_TYPE_ANY, -1);
}

#define wxICON_FROM_MEMORY(name) wxGetIconFromMemory(name##_png, sizeof(name##_png))

inline wxIcon wxGetIconFromMemory(const unsigned char *data, int length) {
   wxIcon icon;
   icon.CopyFromBitmap(wxGetBitmapFromMemory(data, length));
   return icon;
}

/** Regular expression for time values */
extern wxRegEx s_timeRE;

/**
 * Converts time span value (milliseconds) in string
 */
wxString Time2String(long value, bool full = false);
/**
 * Converts string in time span value (milliseconds)
 */
long String2Time(const wxString& value, float fps = 25);
/**
 * Converts string in time span value (milliseconds)
 */
double TimeToDouble(wxString timeStr);

#endif // UTILS_H_
