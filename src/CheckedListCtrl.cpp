/////////////////////////////////////////////////////////////////////////////
// Name:      CheckedListCtrl.cpp
// Author:    Alex Thuering
// Created:   21.06.2015
// Copyright: (c) Alex Thuering
// Licence:   GPL
/////////////////////////////////////////////////////////////////////////////
#include "CheckedListCtrl.h"
#include <wx/renderer.h>
#include <wx/dcmemory.h>
#include <wx/imaglist.h>
#include <wx/log.h>

IMPLEMENT_CLASS(CheckedListCtrl, wxListCtrl)

BEGIN_EVENT_TABLE(CheckedListCtrl, wxListCtrl)
	EVT_LEFT_DOWN(CheckedListCtrl::OnMouseEvent)
END_EVENT_TABLE()

CheckedListCtrl::CheckedListCtrl(wxWindow* parent, wxWindowID id, const wxPoint& pt, const wxSize& sz, long style,
		const wxValidator &validator, const wxString &name) : imageList(NULL) {
	Create(parent, id, pt, sz, style, validator, name);
}

CheckedListCtrl::~CheckedListCtrl() {
	delete imageList;
}

bool CheckedListCtrl::Create(wxWindow* parent, wxWindowID id, const wxPoint& pt, const wxSize& sz, long style,
		const wxValidator& validator, const wxString& name) {
	if (!wxListCtrl::Create(parent, id, pt, sz, style, validator, name))
		return FALSE;

	// Get the native size of the checkbox
	int width = wxRendererNative::Get().GetCheckBoxSize(this).GetWidth();
	int height = wxRendererNative::Get().GetCheckBoxSize(this).GetHeight();

	imageList = new wxImageList(width, height, TRUE);

	wxBitmap unchecked_bmp(width, height), checked_bmp(width, height),
			unchecked_disabled_bmp(width, height), checked_disabled_bmp(width,
					height);

	wxMemoryDC renderer_dc;

	// Unchecked
	renderer_dc.SelectObject(unchecked_bmp);
	renderer_dc.SetBackground(*wxTheBrushList->FindOrCreateBrush(GetBackgroundColour(), wxSOLID));
	renderer_dc.Clear();
	wxRendererNative::Get().DrawCheckBox(this, renderer_dc, wxRect(0, 0, width, height), 0);

	// Checked
	renderer_dc.SelectObject(checked_bmp);
	renderer_dc.SetBackground(*wxTheBrushList->FindOrCreateBrush(GetBackgroundColour(), wxSOLID));
	renderer_dc.Clear();
	wxRendererNative::Get().DrawCheckBox(this, renderer_dc, wxRect(0, 0, width, height), wxCONTROL_CHECKED);

	// Unchecked and Disabled
	renderer_dc.SelectObject(unchecked_disabled_bmp);
	renderer_dc.SetBackground(*wxTheBrushList->FindOrCreateBrush(GetBackgroundColour(), wxSOLID));
	renderer_dc.Clear();
	wxRendererNative::Get().DrawCheckBox(this, renderer_dc, wxRect(0, 0, width, height), 0 | wxCONTROL_DISABLED);

	// Checked and Disabled
	renderer_dc.SelectObject(checked_disabled_bmp);
	renderer_dc.SetBackground(*wxTheBrushList->FindOrCreateBrush(GetBackgroundColour(), wxSOLID));
	renderer_dc.Clear();
	wxRendererNative::Get().DrawCheckBox(this, renderer_dc,	wxRect(0, 0, width, height),
			wxCONTROL_CHECKED | wxCONTROL_DISABLED);

	// Deselect the renderers Object
	renderer_dc.SelectObject(wxNullBitmap);

	// the add order must respect the wxCLC_XXX_IMGIDX defines in the headers !
	imageList->Add(unchecked_bmp);
	imageList->Add(checked_bmp);
	imageList->Add(unchecked_disabled_bmp);
	imageList->Add(checked_disabled_bmp);
	SetImageList(imageList, wxIMAGE_LIST_SMALL);

	return TRUE;
}

void CheckedListCtrl::OnMouseEvent(wxMouseEvent& event) {
	if (event.LeftDown()) {
		int flags;
		long item = HitTest(event.GetPosition(), flags);
		if (item > -1 && (flags & wxLIST_HITTEST_ONITEMICON)) {
			SetChecked(item, !IsChecked(item));
		} else
			event.Skip();
	} else {
		event.Skip();
	}
}

bool CheckedListCtrl::IsChecked(long item) const {
	wxListItem info;
	info.m_mask = wxLIST_MASK_IMAGE;
	info.m_itemId = item;

	if (GetItem(info)) {
		return (info.m_image == 1);
	} else
		return FALSE;
}

void CheckedListCtrl::SetChecked(long item, bool checked) {
	SetItemImage(item, (checked ? 1 : 0), -1);
}
