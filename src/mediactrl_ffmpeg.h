/////////////////////////////////////////////////////////////////////////////
// Name:        mediactrl_ffmpeg.h
// Purpose:     Displays video
// Author:      Alex Thuering
// Created:     21.01.2011
// RCS-ID:      $Id: mediactrl_ffmpeg.h,v 1.5 2014/09/23 05:16:15 ntalex Exp $
// Copyright:   (c) Alex Thuering
// Licence:     GPL
/////////////////////////////////////////////////////////////////////////////

#ifndef MEDIA_CTRL_FF_H
#define MEDIA_CTRL_FF_H

#include <wx/mediactrl.h>
#include <vector>
class wxFfmpegMediaDecoder;

class MediaCtrlFF: public wxMediaCtrl {
public:
	MediaCtrlFF(wxWindow* parent, wxWindowID winid, const wxString& fileName = wxEmptyString,
            const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = 0,
            const wxValidator& validator = wxDefaultValidator, const wxString& name = wxT("mediaCtrl"));
	virtual ~MediaCtrlFF();
	double GetFps();
	wxFfmpegMediaDecoder& GetDecoder();
	wxImage GetImage();
};

#endif // MEDIA_CTRL_FF_H
