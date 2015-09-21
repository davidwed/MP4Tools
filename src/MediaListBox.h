/////////////////////////////////////////////////////////////////////////////
// Name:      MediaListBox.h
// Author:    Alex Thuering
// Created:   7.07.2012
// Copyright: (c) Alex Thuering
// Licence:   GPL
/////////////////////////////////////////////////////////////////////////////

#ifndef MEDIALISTBOX_H_
#define MEDIALISTBOX_H_

#include "MediaFile.h"
#include <wx/vlbox.h>

/**
 * Displays list of media files
 */
class MediaListBox: public wxVListBox {
public:
	MediaListBox(wxWindow *parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition,
			const wxSize& size = wxDefaultSize, long style = 0, int i1 = 0, int i2 = 0,
			const wxValidator& val = wxDefaultValidator, const wxString& name = wxVListBoxNameStr);
	
	void RefreshAll();
	
	void OnDrawItem(wxDC& dc, const wxRect& rect, size_t idx) const;

	wxCoord OnMeasureItem(size_t idx) const;
	
	vector<MediaFile*>& GetFiles() { return files; }

private:
	vector<MediaFile*>& files;
    void OnRightDown(wxMouseEvent& event);
};

#endif /* MEDIALISTBOX_H_ */
