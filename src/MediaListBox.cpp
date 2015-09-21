/////////////////////////////////////////////////////////////////////////////
// Name:      MediaListBox.cpp
// Author:    Alex Thuering
// Created:   7.07.2012
// Copyright: (c) Alex Thuering
// Licence:   GPL
/////////////////////////////////////////////////////////////////////////////

#include "MediaListBox.h"
#include "MainWin.h"
#include "utils.h"
#include "../resources/video.png.h"
#include <wx/dc.h>
#include <wx/dnd.h>

#define ITEM_FONT wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT)

class MediaDnDFile : public wxFileDropTarget {
public:
	MediaDnDFile(MediaListBox* mediaListBox) {
		m_mediaListBox = mediaListBox;
	}

	bool OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& filenames) {
		bool res = false;
		for (int i = 0; i<(int)filenames.Count(); i++) {
			res = AddFile(x, y, filenames[i]) || res;
		}
		m_mediaListBox->RefreshAll();
		m_mediaListBox->SetFocus();
		return res;
	}

	bool AddFile(wxCoord x, wxCoord y, wxString filename) {
		MediaFile* mediaFile = new MediaFile;
		if (!mediaFile->Init(filename)) {
			delete mediaFile;
			return false;
		}
		m_mediaListBox->GetFiles().push_back(mediaFile);
		return true;
	}

private:
	MediaListBox* m_mediaListBox;
};


MediaListBox::MediaListBox(wxWindow *parent, wxWindowID id, const wxPoint& pos,
		const wxSize& size, long style, int i1, int i2, const wxValidator& val, const wxString& name) :
	wxVListBox(parent, id, pos, size, style, name), files(((MainWin*) parent)->GetFiles()) {
	SetDropTarget(new MediaDnDFile(this));
	Connect(wxEVT_RIGHT_DOWN, wxMouseEventHandler(MediaListBox::OnRightDown));
}

void MediaListBox::RefreshAll() {
	SetItemCount(files.size());
	wxVListBox::RefreshAll();
}

void MediaListBox::OnDrawItem(wxDC& dc, const wxRect& rect, size_t idx) const {
	if (idx >= files.size() || files[idx] == NULL)
		return;
	MediaFile& mediaFile = *files[idx];
	
	// image
	wxImage image = mediaFile.GetImage();
	int h = 46;
	int w = image.GetWidth()*h/image.GetHeight();
	wxBitmap bitmap = wxBitmap(image.Scale(w, h));
	dc.DrawBitmap(bitmap, rect.x + 2, rect.y + 2);
	int x = rect.x + bitmap.GetWidth() + 6;
	int y = rect.y + 4;

	// text
	dc.SetFont(ITEM_FONT);
	dc.SetTextForeground((int)idx == GetSelection() ? wxSystemSettings::GetColour(wxSYS_COLOUR_LISTBOXHIGHLIGHTTEXT)
			: wxSystemSettings::GetColour(wxSYS_COLOUR_LISTBOXTEXT));
	dc.DrawText(mediaFile.GetFileName(), x, y);
	y += dc.GetCharHeight() + 2;
	
	double duration = mediaFile.GetDuration();
	wxString s = _("Duration") + wxString(wxT(": "));
	if (duration > 0) {
		int secs = (int) duration;
		int mins = secs / 60;
		secs %= 60;
		int hours = mins / 60;
		mins %= 60;
		s += wxString::Format(wxT("%02d:%02d:%02d"), hours, mins, secs);
	} else
		s += wxT("N/A");
	
	s += " / ";
	s += _("Size");
	long size = mediaFile.GetSize();
	if (size > 1024*1024)
		s += wxString::Format(wxT(": %0.1f "), (double) size / 1024 / 1024) + _("GB");
	else if (size > 1024)
		s += wxString::Format(wxT(": %ld "), size / 1024) + _("MB");
	else
		s += wxString::Format(wxT(": %ld "), size) + _("KB");
	dc.DrawText(s, x, y);
	y += dc.GetCharHeight() + 2;
	
	for (vector<MediaStream*>::const_iterator st = mediaFile.GetStreams().begin();
			st != mediaFile.GetStreams().end(); st++) {
		MediaStream* mediaStream = *st;
		wxString format = mediaStream->GetFormat();
		switch (mediaStream->GetType()) {
		case stVIDEO: {
			dc.DrawText(_("Video:") + wxString(wxT(" ")) + format, x, y);
			y += dc.GetCharHeight() + 2;
			break;
		}
		case stAUDIO: {
			dc.DrawText(_("Audio:") + wxString(wxT(" ")) + format, x, y);
			y += dc.GetCharHeight() + 2;
			break;
		}
		case stSUBTITLE: {
			dc.DrawText(_("Subtitle:") + wxString(wxT(" ")) + format, x, y);
			y += dc.GetCharHeight() + 2;
			break;
		}
		default:
			break;
		}
	}
}

wxCoord MediaListBox::OnMeasureItem(size_t idx) const {
	if (idx >= files.size() || files[idx] == NULL)
		return 0;
	MediaFile& mediaFile = *files[idx];
	
	wxMemoryDC dc;
	dc.SetFont(ITEM_FONT);
	return 4 + (mediaFile.GetStreams().size() + 2)*(dc.GetCharHeight() + 2);
}

void MediaListBox::OnRightDown(wxMouseEvent& event) {
	OnLeftDown(event);
}
