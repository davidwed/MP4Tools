/////////////////////////////////////////////////////////////////////////////
// Name:      ProgramProcess.h
// Author:    Alex Thuering
// Created:   14.02.2013
// Copyright: (c) Alex Thuering
// Licence:   GPL
/////////////////////////////////////////////////////////////////////////////

#ifndef PROGRAMPROCESS_H_
#define PROGRAMPROCESS_H_

#include <wx/wx.h>
#include <wx/process.h>
#include <wx/progdlg.h>

using namespace std;

/**
 * Processes output of program
 */
class ProgramProcess: public wxProcess {
public:
	/** Constructor */
	ProgramProcess(wxProgressDialog* parent);
	
	/** Notifies about process termination */
	virtual void OnTerminate(int pid, int status);
    
	/** Reads data from given stream */
    void DoGetFromStream(wxInputStream& in, wxString& line, bool errorStream);
    
    /** Processes output line */
    virtual void ProcessOutput(const wxString& line, bool errorStream) = 0;
    
    /** Executes a given command */
    bool Execute(const wxString& command);
    
    /** Updates progress message */
    bool Update(const wxString& msg);
    
    /** Updates progress value and message */
    bool Update(int value, const wxString& msg = wxEmptyString);

private:
	wxProgressDialog* progDlg;
	bool terminated;
	wxString lineOut;
	wxString lineErr;
	long pid;
	int status;
	bool HasInput();
};

#endif /* PROGRAMPROCESS_H_ */
