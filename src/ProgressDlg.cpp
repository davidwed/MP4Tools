/////////////////////////////////////////////////////////////////////////////
// Name:      ProgressDlg.cpp
// Author:    Alex Thuering
// Created:   30.07.2016
// Copyright: (c) Alex Thuering
// Licence:   GPL
/////////////////////////////////////////////////////////////////////////////
#include <src/ProgressDlg.h>
#include <iostream>
using namespace std;

ProgressDlg::ProgressDlg(wxWindow *parent, const wxString& title, const wxString& message,
        int maximum, const wxString& logFileName): wxProgressDialog(title, message, maximum, parent,
        		wxPD_APP_MODAL | wxPD_CAN_ABORT | wxPD_ELAPSED_TIME | wxPD_ESTIMATED_TIME) {
	// create log file
	if (logFileName.length()) {
		logFile.Open(logFileName, wxT("w"));
	}
}

ProgressDlg::~ProgressDlg() {
	if (logFile.IsOpened()) {
		logFile.Close();
	}
}

/* Updates progress dialog */
bool ProgressDlg::Update(int value, const wxString& msg, bool *skip) {
	if (msg.length())
		DoLogMessage(msg);
	return wxProgressDialog::Update(value, msg, skip);
}

/** Writes text to log */
void ProgressDlg::DoLog(const wxString& text) {
	if (logFile.IsOpened()) {
		logFile.Write(text);
		logFile.Flush();
	} else {
		cerr << text;
	}
}

/** Writes message to log */
void ProgressDlg::DoLogMessage(const wxString& message) {
	DoLog(message + "\n");
}

