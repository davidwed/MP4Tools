/////////////////////////////////////////////////////////////////////////////
// Name:      AvConvProcess.h
// Author:    Alex Thuering
// Created:   20.06.2015
// Copyright: (c) Alex Thuering
// Licence:   GPL
/////////////////////////////////////////////////////////////////////////////

#ifndef AVCONVPROCESS_H_
#define AVCONVPROCESS_H_

#include "ProgramProcess.h"
#include <wx/progdlg.h>
#include <wx/regex.h>

/**
 * Processes output of avconv tool
 */
class AvConvProcess: public ProgramProcess {
public:
	AvConvProcess(wxProgressDialog* parent, int step, int subSteps, long totalFrames) : ProgramProcess(parent) {
		this->totalFrames = totalFrames;
		this->step = step;
		this->subSteps = subSteps;
		pattern.Compile(wxT("frame=[[:space:]]*([0-9]+).*"));
	}

	virtual void ProcessOutput(const wxString& line, bool errorStream) {
		if (line.Find(wxT("buffer underflow i=1")) >= 0
				|| line.Find(wxT("packet too large, ignoring buffer limits")) >= 0
				|| line.Find(wxT("Last message repeated 1 times")) >= 0)
			return;
		cerr << line << endl;
    	if (line.StartsWith("Error") || line.StartsWith("Unknown")) {
			wxLogError(line);
    	} else if (pattern.Matches(line)) {
			long frame = 0;
			pattern.GetMatch(line, 1).ToLong(&frame);
			int subStep = (totalFrames > 0) ? (frame * subSteps) / totalFrames : 0;
			Update(step*subSteps + subStep);
		}
    }

private:
	long totalFrames;
	int step;
	int subSteps;
	wxRegEx pattern;
};

#endif
