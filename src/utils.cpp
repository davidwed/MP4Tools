/////////////////////////////////////////////////////////////////////////////
// Name:      utils.cpp
// Author:    Alex Thuering
// Created:   7.07.2012
// Copyright: (c) Alex Thuering
// Licence:   GPL
/////////////////////////////////////////////////////////////////////////////

#include "utils.h"
#include <wx/dir.h>
#include <wx/log.h>
#include <wx/filename.h>

#ifdef __UNIX_LIKE__
#include <stdlib.h>
#include <sys/param.h>
#endif

wxString appPath;

wxRegEx s_timeRE(wxT("^(([[:digit:]]+:)?[[:digit:]][[:digit:]]?:)?[[:digit:]][[:digit:]]?(\\.[[:digit:]]+)?$"));

wxString wxGetAppPath()
{
  if (appPath.length() == 0)
#if defined(__WXMAC__) && !defined(__DARWIN__)
    // On Mac, the current directory is the relevant one when
    // the application starts.
    appPath = wxGetWorkingDirectory();
#else
  {
    if (wxIsAbsolutePath(wxTheApp->argv[0]))
      appPath = wxTheApp->argv[0];
    else
    {
	  // Is it a relative path?
	  wxString currentDir(wxGetCwd());
	  if (currentDir.Last() != wxFILE_SEP_PATH)
		currentDir += wxFILE_SEP_PATH;
	  wxString str = currentDir + wxTheApp->argv[0];
	  if (wxFileExists(str))
		appPath = str;
	  else
	  {
		// OK, it's neither an absolute path nor a relative path.
		// Search PATH.
		wxPathList pathList;
		pathList.AddEnvList(wxT("PATH"));
		str = pathList.FindAbsoluteValidPath(wxTheApp->argv[0]);
		if (!str.IsEmpty())
		  appPath = str;
	  }
    }
#ifdef __UNIX_LIKE__
	// realfullname
	char realnameBuf[MAXPATHLEN];
	char* realname = realpath(appPath.mb_str(), (char*)realnameBuf);
	appPath = wxString(realname, *wxConvCurrent);
#endif
	appPath = wxPathOnly(appPath);
  }
#endif
  if (appPath.Last() != wxFILE_SEP_PATH)
    appPath += wxFILE_SEP_PATH;
  return appPath;
}

void wxSetAppPath(wxString value)
{
  appPath = value;
}

wxString wxFindDataDirectory(wxString dir)
{
  wxString d = wxGetAppPath() + dir;
  if (wxDir::Exists(d))
	return d;
  wxFileName dname(wxGetAppPath() + wxT("..") +
     wxFILE_SEP_PATH + dir + wxFILE_SEP_PATH);
  dname.Normalize();
  if (wxDir::Exists(dname.GetFullPath()))
	return dname.GetFullPath();
#ifdef DATADIR
  return wxString(DATADIR,wxConvLocal) + wxFILE_SEP_PATH + dir + wxFILE_SEP_PATH;
#else
  return wxGetAppPath() + dir + wxFILE_SEP_PATH;
#endif
}

wxString wxFindDataFile(wxString filename)
{
  wxString d = wxGetAppPath() + filename;
  if (wxFileExists(d))
	return d;
  wxFileName fname(wxGetAppPath() + wxT("..") + wxFILE_SEP_PATH + filename);
  fname.Normalize();
  if (wxFileExists(fname.GetFullPath()))
	return fname.GetFullPath();
#ifdef DATADIR
  return wxString(DATADIR,wxConvLocal) + wxFILE_SEP_PATH + filename;
#else
  return wxGetAppPath() + filename;
#endif
}

/**
 * Converts time span value (milliseconds) in string
 */
wxString Time2String(long value, bool full) {
	int t = value / 1000;
	int ms = value % 1000;
	if (full || (t >= 3600 && ms > 0))
		return wxString::Format(wxT("%d:%02d:%02d.%03d"), t/3600, (t/60) % 60, t % 60, ms);
	else if (t >= 3600)
		return wxString::Format(wxT("%d:%02d:%02d"), t/3600, (t/60) % 60, t % 60);
	else if (ms > 0)
		return wxString::Format(wxT("%d:%02d.%03d"), (t/60) % 60, t % 60, ms);
	return wxString::Format(wxT("%d:%02d"), (t/60) % 60, t % 60);
}

/**
 * Converts string in time span value (milliseconds)
 */
long String2Time(const wxString& value, float fps) {
	long result = 0;
	wxString val = value;
	for (int i = 0; i <= 3; i++) {
		if (i < 3)
			result *= 60;
		if (val.Find(wxT(':')) == wxNOT_FOUND) {
			result *= 1000;
			if (i == 3) { // SMPTE format 00:00:00:00
				long t = 0;
				if (val.ToLong(&t))
					result += lround(t*1000/fps);
			} else {
				double t = 0;
				if (val.ToDouble(&t))
					result += lround(t*1000);
			}
			break;
		} else {
			long t = 0;
			if (val.BeforeFirst(wxT(':')).ToLong(&t))
				result += t;
			val = val.AfterFirst(wxT(':'));
		}
	}
	return result;
}

/**
 * Converts string in time span value (milliseconds)
 */
double TimeToDouble(wxString timeStr) {
	wxRegEx re(wxT("^(([[:digit:]]+:)?([[:digit:]]?[[:digit:]]):)?([[:digit:]]?[[:digit:]])(\\.[[:digit:]]+)?$"));
	if (!re.Matches(timeStr))
		return -1;
	double result = 0;
	long lval;
	wxString val = re.GetMatch(timeStr, 2).BeforeFirst(wxT(':'));
	if (val.ToLong(&lval))
		result += lval*3600;
	val = re.GetMatch(timeStr, 3).BeforeFirst(wxT(':'));
	if (val.ToLong(&lval))
		result += lval*60;
	val = re.GetMatch(timeStr, 4);
	if (val.ToLong(&lval))
		result += lval;
	val = re.GetMatch(timeStr, 5);
	double dval;
	if (val.ToDouble(&dval))
		result += dval;
	return result;
}
