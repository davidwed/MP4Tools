/////////////////////////////////////////////////////////////////////////////
// Name:      ProgramProcess.cpp
// Author:    Alex Thuering
// Created:   14.02.2013
// Copyright: (c) Alex Thuering
// Licence:   GPL
/////////////////////////////////////////////////////////////////////////////

#include "ProgramProcess.h"
#include <wx/txtstrm.h>

/** Constructor */
ProgramProcess::ProgramProcess(wxProgressDialog* parent) : wxProcess(parent) {
	progDlg = parent;
	terminated = false;
	pid = 0;
	status = 0;
	Redirect();
}

/** Notifies about process termination */
void ProgramProcess::OnTerminate(int pid, int status) {
	this->status = status;
	// read the rest of the output
	while (HasInput())
		;
	if (status != 0)
		Update(-1, _("Failed"));
	terminated = true;
}

bool ProgramProcess::HasInput() {
	bool hasInput = false;
	
	if (IsInputAvailable()) {
		DoGetFromStream(*GetInputStream(), lineOut, false);
		hasInput = true;
	}

	if (IsErrorAvailable()) {
		DoGetFromStream(*GetErrorStream(), lineErr, true);
		hasInput = true;
	}
	
	return hasInput;
}

/**
 * Reads data from given stream
 */
void ProgramProcess::DoGetFromStream(wxInputStream& in, wxString& line, bool errorStream) {
	wxTextInputStream tis(in);
	while (in.CanRead()) {
		wxYieldIfNeeded();
		while (in.CanRead()) {
			wxChar c = tis.GetChar();
			if (c == wxT('\n') || c == wxT('\r')) {
				if (line.length()) {
					ProcessOutput(line, errorStream);
				}
				line = wxT("");
				break;
			}
			line += c;
		}
	}
}

/** Executes a given command */
bool ProgramProcess::Execute(const wxString& command) {
	pid = wxExecute(command, wxEXEC_ASYNC, this);
	if (pid == 0) {
		wxLogError(_("Execution of '%s' failed."), command.c_str());
		return false;
	}
	
	while (!terminated && progDlg->IsVisible()) {
#if wxCHECK_VERSION(2, 9, 0)
		if (progDlg->WasCancelled() && wxProcess::Exists(GetPid())) {
			wxProcess::Kill(GetPid(), wxSIGTERM);
			wxMilliSleep(500);
			if (wxProcess::Exists(GetPid()))
				wxProcess::Kill(GetPid(), wxSIGKILL);
		} else
#endif
		if (!HasInput())
			wxMilliSleep(100);
		wxYield();
	}
	return status == 0;
}

/** Updates progress message */
bool ProgramProcess::Update(const wxString& msg) {
	if (msg.length())
		cerr << msg << endl;
	return progDlg->Update(progDlg->GetValue(), msg);
}

/** Updates progress value and message */
bool ProgramProcess::Update(int value, const wxString& msg) {
	if (value == -1)
		value = progDlg->GetRange();
	if (msg.length())
		cerr << msg << endl;
	return progDlg->Update(value, msg);
}
