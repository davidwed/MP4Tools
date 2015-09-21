/////////////////////////////////////////////////////////////////////////////
// Name:      CheckedListCtrl.h
// Author:    Alex Thuering
// Created:   21.06.2015
// Copyright: (c) Alex Thuering
// Licence:   GPL
/////////////////////////////////////////////////////////////////////////////

#ifndef CHECKED_LISTCTRL_H_
#define CHECKED_LISTCTRL_H_

#include <wx/listctrl.h>

/**
 * wxListCtrl with Checkboxes
 */
class CheckedListCtrl: public wxListCtrl {
public:
	CheckedListCtrl(wxWindow* parent, wxWindowID id, const wxPoint& pos = wxDefaultPosition,
			const wxSize& size = wxDefaultSize, long style = wxLC_ICON,
			const wxValidator& validator = wxDefaultValidator, const wxString& name = wxListCtrlNameStr);
	~CheckedListCtrl();
	
	bool Create(wxWindow *parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition,
			const wxSize& size = wxDefaultSize, long style = wxLC_ICON,
			const wxValidator& validator = wxDefaultValidator, const wxString& name = wxListCtrlNameStr);
	void OnMouseEvent(wxMouseEvent& event);
	
	bool IsChecked(long item) const;
	void SetChecked(long item, bool checked);
	
private:
	wxImageList* imageList;
	
	DECLARE_CLASS(CheckedListCtrl)
	DECLARE_EVENT_TABLE()
};


#endif
