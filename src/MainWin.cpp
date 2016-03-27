/////////////////////////////////////////////////////////////////////////////
// Name:      MainWin.cpp
// Author:    Alex Thuering
// Created:   7.07.2012
// Copyright: (c) Alex Thuering
// Licence:   GPL
/////////////////////////////////////////////////////////////////////////////
#include "MainWin.h"

//(*InternalHeaders(MainWin)
#include <wx/bitmap.h>
#include <wx/settings.h>
#include <wx/intl.h>
#include <wx/image.h>
#include <wx/string.h>
//*)
#include <wx/toolbar.h>
#include <wx/file.h>
#include <wx/artprov.h>
#include <wx/aboutdlg.h>
#include "VideoPropDlg.h"
#include "OptionsDlg.h"
#include "AvConvProcess.h"
#include "Config.h"
#include "Version.h"
#include "utils.h"
#include "../resources/mp4joiner.png.h"
#include "../resources/add.png.h"
#include "../resources/remove.png.h"
#include "../resources/trash.png.h"
#include "../resources/run.png.h"
#include "../resources/preferences.png.h"
#include "../resources/info.png.h"
#include "../resources/up.png.h"
#include "../resources/down.png.h"
#include <map>
#include <algorithm>
#include <utility>

/**
 * Processes output of MP4Box tool
 */
class MP4Process: public ProgramProcess {
public:
	MP4Process(wxProgressDialog* parent, int step, int stepCount, vector<MediaFile*> mediaFiles):
			ProgramProcess(parent), files(mediaFiles) {
		this->step = step;
		this->stepCount = stepCount;
		pattern.Compile(".*\\(([0-9]+)/100\\).*");
	}

	virtual void OnTerminate(int pid, int status) {
		ProgramProcess::OnTerminate(pid, status); // read the rest of the output
		if (status == 0)
			Update(-1, _("Successful"));
	}

	virtual void ProcessOutput(const wxString& line, bool errorStream) {
    	if (line.StartsWith("Error") || line.StartsWith("Unknown")
    			|| line.StartsWith("No suitable media tracks to cat")) {
    		cerr << line << endl;
			wxLogError(line);
		} else if (line.StartsWith("Appending file") || line.StartsWith("IsoMedia import")
				|| line.Find("video import") >= 0 || line.Find("audio import") >= 0 || line.Find("Audio import") >= 0) {
			if (step < stepCount - 1)
				step++;
			Update(step*100, line);
		} else if (line.StartsWith("Saving to")) {
			if (step < stepCount - 1)
				step++;
			Update(step*100, line.Index(':') > 0 ? line.BeforeLast(':') : line);
		} else if (pattern.Matches(line)) {
			cerr << line << endl;
			long percent = 0;
			if (pattern.GetMatch(line, 1).ToLong(&percent)) {
				Update(step*100 + percent);
			}
		} else
			cerr << line << endl;
    }

private:
	vector<MediaFile*>& files;
	int step;
	int stepCount;
	wxRegEx pattern;
};

//(*IdInit(MainWin)
const long MainWin::ID_MEDIA_LISTBOX = wxNewId();
const long MainWin::ID_BITMAPBUTTON1 = wxNewId();
const long MainWin::ID_BITMAPBUTTON2 = wxNewId();
const long MainWin::ID_TOOLBAR = wxNewId();
const long MainWin::ID_STATUSBAR = wxNewId();
//*)
const long ADD_FILE_ID = wxNewId();
const long REMOVE_FILE_ID = wxNewId();
const long REMOVE_ALL_ID = wxNewId();
const long SORT_ID = wxNewId();
const long PROPERTIES_ID = wxNewId();
const long RUN_ID = wxNewId();
const long SETTINGS_ID = wxNewId();
const long ABOUT_ID = wxNewId();

BEGIN_EVENT_TABLE(MainWin,wxFrame)
	//(*EventTable(MainWin)
	//*)
END_EVENT_TABLE()

MainWin::MainWin(): forceReencodeAudio(false), forceReencodeVideo(false) {
	//(*Initialize(MainWin)
	wxBoxSizer* BoxSizer2;
	wxBoxSizer* BoxSizer1;

	Create(0, wxID_ANY, _("MP4 Joiner"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL, _T("wxID_ANY"));
	SetFocus();
	SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE));
	BoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
	mediaListBox = new MediaListBox(this, ID_MEDIA_LISTBOX, wxPoint(328,48), wxSize(300,400), 0, 0, 0, wxDefaultValidator, _T("ID_MEDIA_LISTBOX"));
	BoxSizer1->Add(mediaListBox, 1, wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 4);
	BoxSizer2 = new wxBoxSizer(wxVERTICAL);
	upButton = new wxBitmapButton(this, ID_BITMAPBUTTON1, wxBITMAP_FROM_MEMORY(up), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|wxTAB_TRAVERSAL, wxDefaultValidator, _T("ID_BITMAPBUTTON1"));
	BoxSizer2->Add(upButton, 0, wxBOTTOM|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	downButton = new wxBitmapButton(this, ID_BITMAPBUTTON2, wxBITMAP_FROM_MEMORY(down), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|wxSTATIC_BORDER, wxDefaultValidator, _T("ID_BITMAPBUTTON2"));
	BoxSizer2->Add(downButton, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer1->Add(BoxSizer2, 0, wxLEFT|wxRIGHT|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	SetSizer(BoxSizer1);
	statusBar = new wxStatusBar(this, ID_STATUSBAR, 0, _T("ID_STATUSBAR"));
	int __wxStatusBarWidths_1[1] = { -10 };
	int __wxStatusBarStyles_1[1] = { wxSB_NORMAL };
	statusBar->SetFieldsCount(1,__wxStatusBarWidths_1);
	statusBar->SetStatusStyles(1,__wxStatusBarStyles_1);
	SetStatusBar(statusBar);
	BoxSizer1->Fit(this);
	BoxSizer1->SetSizeHints(this);
	Center();

	Connect(ID_MEDIA_LISTBOX,wxEVT_COMMAND_LISTBOX_SELECTED,(wxObjectEventFunction)&MainWin::OnMediaSelect);
	Connect(ID_MEDIA_LISTBOX,wxEVT_COMMAND_LISTBOX_DOUBLECLICKED,(wxObjectEventFunction)&MainWin::OnProperties);
	Connect(ID_BITMAPBUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MainWin::OnUpBt);
	Connect(ID_BITMAPBUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MainWin::OnDownBt);
	//*)

	toolbar = new wxToolBar(this, ID_TOOLBAR, wxDefaultPosition, wxDefaultSize, wxTB_TEXT|wxTB_FLAT);
	SetToolBar(toolbar);
	toolbar->AddTool(ADD_FILE_ID, _("Add Video"), wxBITMAP_FROM_MEMORY(add), wxNullBitmap, wxITEM_NORMAL,
			_("Add video file"), wxT(""));
#if wxCHECK_VERSION(2,9,0)
	toolbar->AddTool(REMOVE_FILE_ID,  _("Remove"), wxBITMAP_FROM_MEMORY(remove), wxNullBitmap, wxITEM_DROPDOWN,
			_("Remove selected video"), wxT(""));
	wxMenu* delMenu = new wxMenu;
	delMenu->Append(REMOVE_FILE_ID, _("&Remove selected"));
	wxMenuItem* mitem =  new wxMenuItem(delMenu, REMOVE_ALL_ID, _("Remove a&ll"));
	mitem->SetBitmap(wxBitmap(wxIMAGE_FROM_MEMORY(trash).Rescale(16, 16)));
	delMenu->Append(mitem);
	toolbar->SetDropdownMenu(REMOVE_FILE_ID, delMenu);
#else
	toolbar->AddTool(REMOVE_FILE_ID,  _("Remove"), wxBITMAP_FROM_MEMORY(remove), wxNullBitmap, wxITEM_NORMAL,
			_("Remove selected video"), wxT(""));
#endif
#ifdef __WXMAC__
	toolbar->AddTool(REMOVE_ALL_ID,  _("Remove all"), wxBITMAP_FROM_MEMORY(trash), wxNullBitmap, wxITEM_NORMAL,
				_("Remove all videos"), wxT(""));
#endif 
	toolbar->AddTool(RUN_ID,  _("Join"), wxBITMAP_FROM_MEMORY(run), wxNullBitmap, wxITEM_NORMAL,
			_("Select output file and start join"), wxT(""));
#if wxCHECK_VERSION(2, 9, 0)
	toolbar->AddStretchableSpace();
#endif
	toolbar->AddTool(SETTINGS_ID, _("Options"), wxBITMAP_FROM_MEMORY(preferences),
			wxNullBitmap, wxITEM_NORMAL, _("Options"), wxT(""));
	toolbar->AddTool(ABOUT_ID, _("About"),  wxBITMAP_FROM_MEMORY(info),
			wxNullBitmap, wxITEM_NORMAL, _("About the program"), wxT(""));
	toolbar->Realize();
	UpdateToolBar();

	Connect(ADD_FILE_ID, wxEVT_COMMAND_TOOL_CLICKED, (wxObjectEventFunction)&MainWin::OnAddFileBt);
	Connect(REMOVE_FILE_ID, wxEVT_COMMAND_TOOL_CLICKED, (wxObjectEventFunction)&MainWin::OnRemoveFileBt);
	Connect(REMOVE_ALL_ID, wxEVT_COMMAND_TOOL_CLICKED, (wxObjectEventFunction)&MainWin::OnRemoveAllBt);
	Connect(RUN_ID, wxEVT_COMMAND_TOOL_CLICKED, (wxObjectEventFunction)&MainWin::OnRunBt);
	Connect(SETTINGS_ID, wxEVT_COMMAND_TOOL_CLICKED, (wxObjectEventFunction)&MainWin::OnSettingsBt);
	Connect(ABOUT_ID, wxEVT_COMMAND_TOOL_CLICKED, (wxObjectEventFunction)&MainWin::OnAboutBt);
	Connect(wxEVT_CLOSE_WINDOW, (wxObjectEventFunction)&MainWin::OnClose);
	mediaListBox->Connect(wxEVT_CONTEXT_MENU, wxContextMenuEventHandler(MainWin::OnRightClick));
	Connect(SORT_ID, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&MainWin::OnSort);
	Connect(PROPERTIES_ID, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&MainWin::OnProperties);

	// Restore frame size/position
	if (s_config.IsMainWinMaximized()) {
		Maximize(true);
	} else {
		wxRect rect = s_config.GetMainWinLocation();
		if (rect.width > 0 && rect.height > 0) {
			SetSize(rect);
			Center();
		} else {
			SetSize(wxSize(500,550));
			Center();
		}
	}
	mediaListBox->SetFocus();
}

MainWin::~MainWin() {
	//(*Destroy(MainWin)
	//*)
	VECTOR_CLEAR(files, MediaFile)
}

void MainWin::UpdateToolBar() {
	toolbar->EnableTool(REMOVE_FILE_ID, mediaListBox->GetSelection() >= 0);
	toolbar->EnableTool(REMOVE_ALL_ID, mediaListBox->GetItemCount() > 0);
	toolbar->EnableTool(RUN_ID, files.size() > 1);
	upButton->Enable(mediaListBox->GetSelection() > 0);
	downButton->Enable(mediaListBox->GetSelection() >= 0 && mediaListBox->GetSelection() < (int) files.size() - 1);
	long totalDuration = 0;
	long totalSize = 0;
	for (vector<MediaFile*>::iterator it = files.begin(); it != files.end(); it++) {
		if ((*it)->GetDuration() > 0)
			totalDuration += (int) (*it)->GetResultDuration();
		totalSize += (*it)->GetResultSize() / 1024;
	}
	wxString statusStr = _("Total duration");
	int mins = totalDuration / 60;
	int secs = totalDuration % 60;
	int hours = mins / 60;
	mins %= 60;
	statusStr += wxString::Format(" %02d:%02d:%02d", hours, mins, secs);
	statusStr += " / ";
	statusStr += _("total size");
	if (totalSize > 1024)
		statusStr += wxString::Format(": %0.1f ", (double) totalSize / 1024) + _("GB");
	else
		statusStr += wxString::Format(": %ld ", totalSize) + _("MB");
	statusBar->SetStatusText(statusStr, 0);
}

void MainWin::OnAddFileBt(wxCommandEvent& event) {
	wxFileDialog fileDlg(this, _("Choose a file"), wxT(""), wxT(""),
			wxString(_("MP4 Files")) + wxT(" (*.mp4;*.ts)|*.mp4;*ts|")
			+ wxString(_("AVI Files")) + wxT(" (*.avi;*.mov)|*.avi;*.mov|")
			+ wxString(_("All Files")) + wxT(" (*.*)|*.*"), wxFD_OPEN | wxFD_MULTIPLE);
	fileDlg.SetDirectory(s_config.GetLastAddDir() + wxFILE_SEP_PATH);
	if (fileDlg.ShowModal() != wxID_OK)
		return;
	s_config.SetLastAddDir(fileDlg.GetDirectory());
	wxArrayString paths;
	fileDlg.GetPaths(paths);
	for (unsigned int i = 0; i < paths.GetCount(); i++) {
		MediaFile* mediaFile = new MediaFile;
		if (mediaFile->Init(paths[i]))
			files.push_back(mediaFile);
		else
			delete mediaFile;
	}
	mediaListBox->RefreshAll();
	if (mediaListBox->GetItemCount() > 0 && mediaListBox->GetSelection() == -1)
		mediaListBox->SetSelection(0);
	UpdateToolBar();
}

void MainWin::OnRemoveFileBt(wxCommandEvent& event) {
	if (mediaListBox->GetSelection() == -1)
		return;
	delete files[mediaListBox->GetSelection()];
	files.erase(files.begin() + mediaListBox->GetSelection());
	mediaListBox->RefreshAll();
	UpdateToolBar();
}

void MainWin::OnRemoveAllBt(wxCommandEvent& event) {
	VECTOR_CLEAR(files, MediaFile)
	mediaListBox->RefreshAll();
	UpdateToolBar();
}

void MainWin::OnUpBt(wxCommandEvent& event) {
	std::swap(files[mediaListBox->GetSelection()], files[mediaListBox->GetSelection() - 1]);
	mediaListBox->SetSelection(mediaListBox->GetSelection() - 1);
	mediaListBox->SetFocus();
	UpdateToolBar();
	mediaListBox->RefreshAll();
}

void MainWin::OnDownBt(wxCommandEvent& event) {
	std::swap(files[mediaListBox->GetSelection()], files[mediaListBox->GetSelection() + 1]);
	mediaListBox->SetSelection(mediaListBox->GetSelection() + 1);
	mediaListBox->SetFocus();
	UpdateToolBar();
	mediaListBox->RefreshAll();
}

bool compareMediaFile(MediaFile* i, MediaFile* j) { return i->GetFileName() < j->GetFileName(); }

void MainWin::OnSort(wxCommandEvent& event) {
	std::sort(files.begin(), files.end(), compareMediaFile);
	mediaListBox->RefreshAll();
}

void MainWin::OnProperties(wxCommandEvent& event) {
	if (mediaListBox->GetSelection() == -1)
		return;
	VideoPropDlg dialog(this, files[mediaListBox->GetSelection()]);
	if (dialog.ShowModal() == wxID_OK) {
		UpdateToolBar();
	}
}

void RemoveTempFiles(const map<int, wxString>& tempFiles) {
	for (map<int, wxString>::const_iterator it = tempFiles.begin(); it != tempFiles.end(); it++) {
		if (wxFileExists(it->second)) {
			wxRemoveFile(it->second);
		}
	}
}

void MainWin::OnRunBt(wxCommandEvent& event) {
	// choose a file to save
	wxFileDialog fileDlg(this, _("Choose a file to save"), wxT(""), _("Output.mp4"),
		wxString(_("MP4 Files")) + wxT(" (*.mp4)|*.mp4|")
		+ wxString(_("All Files")) + wxT(" (*.*)|*.*"), wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
	if (fileDlg.ShowModal() != wxID_OK)
		return;
	wxString fileName = fileDlg.GetPath();
	if (wxFile::Exists(fileName) && !wxRemoveFile(fileName))
		return;

	// check if reencoding is needed
	map<int, wxString> tempFiles;
	MediaFile* mediaFile1 = files[0];
	for (unsigned int idx = 0; idx < files.size(); idx++) {
		MediaFile* mediaFile = files[idx];
		mediaFile->SetReencodeAudio(!mediaFile->HasCompatibleAudioStreams(mediaFile1) || forceReencodeAudio);
		mediaFile->SetReencodeVideo(!mediaFile->HasCompatibleVideoStreams(mediaFile1) || forceReencodeVideo);
		if (!mediaFile->IsReencodeVideo() && !mediaFile->IsReencodeAudio() && !mediaFile->IsCutVideo())
			continue;
		tempFiles[idx] = wxFileName::CreateTempFileName(wxFileName::GetTempDir() + wxFILE_SEP_PATH)
				+ "." + mediaFile1->GetFormatName();
	}

	// show progress dialog
	int streamCount = mediaFile1->GetStreams().size();
	int stepCount = tempFiles.size()*10 + files.size()*(streamCount + 1) + 1;
	wxProgressDialog progDlg(_("MP4 Joiner"), _("Joning the files"), stepCount*100, this,
			wxPD_APP_MODAL | wxPD_CAN_ABORT | wxPD_ELAPSED_TIME | wxPD_ESTIMATED_TIME);
	progDlg.Show();
	int step = 0;

	// reencode
	for (map<int, wxString>::iterator it = tempFiles.begin(); it != tempFiles.end(); it++) {
		int fileIdx = it->first;
		wxString tempFileName = it->second;

		MediaFile* mediaFile = files[fileIdx];
		wxString message = "%s";
		if (mediaFile->IsReencodeVideo())
			message = _("Reencode video from file %s");
		else if (mediaFile->IsReencodeAudio())
			message = _("Reencode audio from file %s");
		else if (mediaFile->IsCutVideo())
			message = _("Cut video from file %s");
			
		progDlg.Update(step * 100, wxString::Format(message, mediaFile->GetFileName().c_str()));

		// build avconv/ffmpeg command
		wxString cmd = "ffmpeg";
#if defined(__WXMSW__) || defined(__WXMAC__)
		cmd = '"' + wxGetAppPath() + cmd + '"';
#endif
		cmd += " -i \"" + mediaFile->GetFileName() + '"';
		
		// reencode video
		if (mediaFile->IsReencodeVideo()) {
			MediaStream* video = mediaFile->GetVideoStream();
			MediaStream* video1 = mediaFile1->GetVideoStream();
			cmd += " -c:v " + video1->GetCodecName();
			if (mediaFile1->GetFormatName() == "avi" && video1->GetCodecTag().length())
				cmd += " -vtag " + video1->GetCodecTag().Upper();
			if (video->GetVideoSize() != video1->GetVideoSize()
					|| lround(video->GetAspectRatio()*10) != lround(video1->GetAspectRatio()*10)) {
				cmd += wxString::Format(" -s %dx%d", video1->GetVideoSize().x, video1->GetVideoSize().y);
				cmd += wxString::Format(" -aspect %g", video1->GetAspectRatio());
				wxSize pad = mediaFile->CalcPad(video1->GetVideoSize(), video1->GetAspectRatio());
				if (pad.x > 0 || pad.y > 0) {
					int w = video1->GetVideoSize().x;
					int h = video1->GetVideoSize().y;
					cmd += " -vf " + wxString::Format(wxT("scale=%d:%d"), w - pad.x*2, h - pad.y*2)
							+ wxString::Format(wxT(",pad=%d:%d:%d:%d"), w, h, pad.x, pad.y);
				}
			}
			if (lround(mediaFile->GetVideoStream()->GetFps()*10) != lround(video1->GetFps()*10)
					|| lround(mediaFile->GetVideoStream()->GetTimebase()*10) != lround(video1->GetTimebase()*10))
				cmd += wxString::Format(" -r %g", video1->GetFps());

		} else {
			cmd += " -c:v copy";
		}
		
		// reencode audio
		if (mediaFile->IsReencodeAudio()) {
			unsigned int audioIdx = 0;
			unsigned int streamIdx1 = 0;
			for (unsigned int i = 0; i < mediaFile->GetStreams().size(); i++) {
				if (mediaFile->GetStreams()[i]->GetType() == stAUDIO) {
					// find corresponding audio stream in mediaFile1
					while (streamIdx1 < mediaFile1->GetStreams().size()
							&& mediaFile1->GetStreams()[streamIdx1]->GetType() != stAUDIO)
						streamIdx1++;
					if (streamIdx1 >= mediaFile1->GetStreams().size())
						break;
					// set audio codec
					wxString codec = mediaFile1->GetStreams()[streamIdx1]->GetCodecName();
					cmd += wxString::Format(" -c:a:%d ", audioIdx) + codec;
					cmd += wxString::Format(" -ac:a:%d %d", audioIdx, mediaFile1->GetStreams()[streamIdx1]->GetChannelNumber());
					cmd += wxString::Format(" -ar:a:%d %d", audioIdx, mediaFile1->GetStreams()[streamIdx1]->GetSampleRate());
					audioIdx++;
					streamIdx1++;
				}
			}
		} else {
			unsigned int audioIdx = 0;
			for (unsigned int i = 0; i < mediaFile->GetStreams().size(); i++) {
				if (mediaFile->GetStreams()[i]->GetType() == stAUDIO
						&& mediaFile->GetStreams()[i]->GetCodecName() == "aac") {
					cmd += wxString::Format(" -bsf:a:%d aac_adtstoasc", audioIdx);
					audioIdx++;
				}
			}
			cmd += " -c:a copy";
		}
		
		// cut video
		if (mediaFile->GetStartTime() != 0)
			cmd += wxString::Format(wxT(" -ss %f"), mediaFile->GetStartTime());
		if (mediaFile->GetRecordingTime() > 0)
			cmd += wxString::Format(wxT(" -t %f"), mediaFile->GetRecordingTime());
		
		// output file
		cmd += " \"" + tempFileName + '"';
		cerr << cmd << endl;

		// execute avconv/ffmpeg
		AvConvProcess proc(&progDlg, step, 1000, lround(mediaFile->GetDuration() * mediaFile->GetVideoStream()->GetFps()));
		if (!proc.Execute(cmd)) {
			RemoveTempFiles(tempFiles);
			return;
		}
		cerr << endl;
		step += 10;
	}

	// build MP4Box command
	wxString cmd = "MP4Box";
#if defined(__WXMSW__) || defined(__WXMAC__)
	cmd = '"' + wxGetAppPath() + cmd + '"';
#endif
	for (unsigned int idx = 0; idx < files.size(); idx++) {
		wxString fileName = tempFiles.find(idx) != tempFiles.end() ? tempFiles[idx] : files[idx]->GetFileName();
		cmd += " -cat \"" + fileName + '"';
	}
	cmd += " \"" + fileName + '"';
	cerr << cmd << endl;

	// execute MP4Box
	step--;
	MP4Process proc(&progDlg, step, stepCount, files);
	proc.Execute(cmd);
	RemoveTempFiles(tempFiles);
}

void MainWin::OnSettingsBt(wxCommandEvent& event) {
	OptionsDlg dlg(this);
	dlg.SetForceReencodeAudio(forceReencodeAudio);
	dlg.SetForceReencodeVideo(forceReencodeVideo);
	if (dlg.ShowModal() == wxID_OK) {
		forceReencodeAudio = dlg.IsForceReencodeAudio();
		forceReencodeVideo = dlg.IsForceReencodeVideo();
	}
}

wxString FixEmail(const wxString& str) {
	wxString result = str;
	result.Replace(" at ", "@");
	return result;
}

void MainWin::OnAboutBt(wxCommandEvent& event) {
	wxAboutDialogInfo info;
	info.SetName("MP4Joiner (MP4Tools)");
	info.SetVersion(APP_VERSION);
	info.SetDescription(_("Simple tool to join multiple MP4 files into one."));
	info.SetCopyright(APP_COPYRIGHT);
	info.SetWebSite(APP_COMPANYWEB);
#ifdef __WXMSW__
	info.SetIcon(wxICON(mp4joiner));
#else
	info.SetIcon(wxICON_FROM_MEMORY(mp4joiner));
#endif
	wxAboutBox(info);
}

void MainWin::OnMediaSelect(wxCommandEvent& event) {
	UpdateToolBar();
}

void MainWin::OnRightClick(wxContextMenuEvent &event) {
	wxMenu ctxMenu;
	ctxMenu.Append(REMOVE_FILE_ID, _("&Remove")); //->Enable(mediaListBox->GetSelection() >= 0);
	ctxMenu.Append(PROPERTIES_ID, _("&Cut video..."));
	ctxMenu.AppendSeparator();
	ctxMenu.Append(SORT_ID, _("&Sort"));
	PopupMenu(&ctxMenu);
}

void MainWin::OnClose(wxCloseEvent &event) {
	if (!IsIconized())
		s_config.SetMainWinLocation(GetRect(), IsMaximized());
	Destroy();
}
