/////////////////////////////////////////////////////////////////////////////
// Name:      ProgressDlg.h
// Author:    Alex Thuering
// Created:   30.07.2016
// Copyright: (c) Alex Thuering
// Licence:   GPL
/////////////////////////////////////////////////////////////////////////////

#ifndef PROGRESSDLG_H
#define PROGRESSDLG_H

#include <wx/progdlg.h>
#include <wx/ffile.h>

class ProgressDlg: public wxProgressDialog {
public:
	ProgressDlg(wxWindow *parent, const wxString& title, const wxString& message, int maximum,
			const wxString& logFileName);
	~ProgressDlg();
	
	/** Updates progress dialog */
	virtual bool Update(int value, const wxString& msg = wxEmptyString, bool *skip = NULL);
	
	/** Writes text to log */
	void DoLog(const wxString& text);
	/** Writes message to log */
	void DoLogMessage(const wxString& message);

private:
	wxFFile logFile;
};

#endif // PROGRESSDLG_H
