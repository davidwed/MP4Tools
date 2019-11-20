/////////////////////////////////////////////////////////////////////////////
// Name:      SplitterWin.cpp
// Author:    Alex Thuering
// Created:   7.07.2012
// Copyright: (c) Alex Thuering
// Licence:   GPL
/////////////////////////////////////////////////////////////////////////////
#include "SplitterWin.h"

//(*InternalHeaders(SplitterWin)
#include <wx/artprov.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/intl.h>
#include <wx/settings.h>
#include <wx/string.h>
//*)
#include <wx/toolbar.h>
#include <wx/file.h>
#include <wx/artprov.h>
#include <wx/aboutdlg.h>
#include "VideoPropDlg.h"
#include "ProgressDlg.h"
#include "AvConvProcess.h"
#include "Languages.h"
#include "Config.h"
#include "Version.h"
#include "utils.h"
#include "OptionsDlg.h"
#include "../resources/mp4splitter.png.h"
#include "../resources/add.png.h"
#include "../resources/remove.png.h"
#include "../resources/run.png.h"
#include "../resources/preferences.png.h"
#include "../resources/info.png.h"
#include "../resources/up.png.h"
#include "../resources/down.png.h"
#include "../resources/play.png.h"
#include "../resources/pause.png.h"
#include <map>
#include <algorithm>
#include <utility>
#include <wx/dnd.h>

class SplitterDnDFile : public wxFileDropTarget {
public:
	SplitterDnDFile(SplitterWin* mediaListBox) {
		m_mediaListBox = mediaListBox;
	}

	bool OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& filenames) {
		if (filenames.size() > 0) {
			return m_mediaListBox->Open(filenames[0]);
		}
		return false;
	}
	
private:
	SplitterWin* m_mediaListBox;
};


//(*IdInit(SplitterWin)
const long SplitterWin::ID_MEDIA_CTRL = wxNewId();
const long SplitterWin::ID_SLIDER = wxNewId();
const long SplitterWin::ID_PLAY_BT = wxNewId();
const long SplitterWin::ID_TIME_CTRL = wxNewId();
const long SplitterWin::ID_TIME_SPIN = wxNewId();
const long SplitterWin::ID_FRAME_SPIN = wxNewId();
const long SplitterWin::ID_ADD_SPLIT_POINT_BT = wxNewId();
const long SplitterWin::ID_OPEN_BT = wxNewId();
const long SplitterWin::ID_SETTINGS_BT = wxNewId();
const long SplitterWin::ID_ABOUT_BT = wxNewId();
const long SplitterWin::ID_LISTCTRL1 = wxNewId();
const long SplitterWin::ID_START_BT = wxNewId();
//*)
const long REMOVE_POINT_ID = wxNewId();
const long REMOVE_ALL_ID = wxNewId();

BEGIN_EVENT_TABLE(SplitterWin,wxFrame)
	//(*EventTable(SplitterWin)
	//*)
END_EVENT_TABLE()

SplitterWin::SplitterWin() {
	//(*Initialize(SplitterWin)
	wxBoxSizer* BoxSizer1;
	wxBoxSizer* BoxSizer2;
	wxBoxSizer* BoxSizer4;
	wxBoxSizer* mainHSizer;
	wxBoxSizer* mediaSizer;
	wxBoxSizer* rightSizer;
	wxBoxSizer* timeSizer;
	wxStaticText* label1;
	wxStaticText* staticText1;

	Create(0, wxID_ANY, _("MP4 Splitter"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL, _T("wxID_ANY"));
	SetFocus();
	SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE));
	mainHSizer = new wxBoxSizer(wxHORIZONTAL);
	mediaSizer = new wxBoxSizer(wxVERTICAL);
	mediaCtrl = new MediaCtrlFF(this, ID_MEDIA_CTRL, wxT(""), wxDefaultPosition,wxDefaultSize, 0, wxDefaultValidator, _T("ID_MEDIA_CTRL"));
	mediaCtrl->SetMinSize(wxSize(300, 200));
	mediaCtrl->SetWindowStyle(wxBORDER_NONE);
	mediaSizer->Add(mediaCtrl, 1, wxALL|wxEXPAND, 4);
	mediaSlider = new wxSlider(this, ID_SLIDER, 0, 0, 100, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_SLIDER"));
	mediaSizer->Add(mediaSlider, 0, wxEXPAND, 2);
	timeSizer = new wxBoxSizer(wxHORIZONTAL);
	playBt = new wxBitmapButton(this, ID_PLAY_BT, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW, wxDefaultValidator, _T("ID_PLAY_BT"));
	timeSizer->Add(playBt, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	staticText1 = new wxStaticText(this, wxID_ANY, _("Time:"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	timeSizer->Add(staticText1, 0, wxLEFT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 4);
	BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
	timeCtrl = new wxTextCtrl(this, ID_TIME_CTRL, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TIME_CTRL"));
	BoxSizer2->Add(timeCtrl, 0, wxLEFT|wxALIGN_CENTER_VERTICAL, 2);
	timeSpinBt = new wxSpinButton(this, ID_TIME_SPIN, wxDefaultPosition, wxDefaultSize, wxSP_VERTICAL|wxSP_ARROW_KEYS, _T("ID_TIME_SPIN"));
	timeSpinBt->SetRange(0, 100);
	timeSpinBt->SetMinSize(wxSize(16,12));
	BoxSizer2->Add(timeSpinBt, 0, wxEXPAND, 5);
	frameSpinBt = new wxSpinButton(this, ID_FRAME_SPIN, wxDefaultPosition, wxDefaultSize, wxSP_VERTICAL|wxSP_ARROW_KEYS|wxSP_WRAP, _T("ID_FRAME_SPIN"));
	frameSpinBt->SetRange(-9999, 9999);
	frameSpinBt->SetMinSize(wxSize(16,12));
	BoxSizer2->Add(frameSpinBt, 0, wxEXPAND, 5);
	timeSizer->Add(BoxSizer2, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	addSplitPointBt = new wxButton(this, ID_ADD_SPLIT_POINT_BT, _("Add split point"), wxDefaultPosition, wxSize(-1,24), 0, wxDefaultValidator, _T("ID_ADD_SPLIT_POINT_BT"));
	timeSizer->Add(addSplitPointBt, 0, wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 8);
	mediaSizer->Add(timeSizer, 0, wxALL|wxEXPAND, 5);
	mainHSizer->Add(mediaSizer, 1, wxALL|wxEXPAND, 5);
	rightSizer = new wxBoxSizer(wxVERTICAL);
	BoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
	openBt = new wxButton(this, ID_OPEN_BT, _("Open Video"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_OPEN_BT"));
	BoxSizer1->Add(openBt, 0, wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	BoxSizer1->Add(-1,-1,1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	settingsBt = new wxBitmapButton(this, ID_SETTINGS_BT, wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_TICK_MARK")),wxART_TOOLBAR), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW, wxDefaultValidator, _T("ID_SETTINGS_BT"));
	BoxSizer1->Add(settingsBt, 0, wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	aboutBt = new wxBitmapButton(this, ID_ABOUT_BT, wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_INFORMATION")),wxART_TOOLBAR), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW, wxDefaultValidator, _T("ID_ABOUT_BT"));
	BoxSizer1->Add(aboutBt, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	rightSizer->Add(BoxSizer1, 0, wxEXPAND, 5);
	label1 = new wxStaticText(this, wxID_ANY, _("Split points:"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	rightSizer->Add(label1, 0, wxALL|wxALIGN_LEFT, 5);
	BoxSizer4 = new wxBoxSizer(wxHORIZONTAL);
	pointListCtrl = new CheckedListCtrl(this, ID_LISTCTRL1, wxDefaultPosition, wxDefaultSize, wxLC_REPORT|wxLC_SINGLE_SEL, wxDefaultValidator, _T("ID_LISTCTRL1"));
	BoxSizer4->Add(pointListCtrl, 1, wxEXPAND, 5);
	rightSizer->Add(BoxSizer4, 1, wxEXPAND, 5);
	startBt = new wxButton(this, ID_START_BT, _("Start splitting"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_START_BT"));
	rightSizer->Add(startBt, 0, wxALL|wxALIGN_RIGHT, 5);
	mainHSizer->Add(rightSizer, 0, wxEXPAND, 5);
	SetSizer(mainHSizer);
	mainHSizer->Fit(this);
	mainHSizer->SetSizeHints(this);
	Center();

	Connect(ID_SLIDER,wxEVT_COMMAND_SLIDER_UPDATED,(wxObjectEventFunction)&SplitterWin::OnSliderScroll);
	Connect(ID_PLAY_BT,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&SplitterWin::OnPlay);
	Connect(ID_TIME_CTRL,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&SplitterWin::OnChangeTime);
	Connect(ID_TIME_SPIN,wxEVT_SCROLL_THUMBTRACK,(wxObjectEventFunction)&SplitterWin::OnTimeSpin);
	Connect(ID_FRAME_SPIN,wxEVT_SCROLL_LINEUP,(wxObjectEventFunction)&SplitterWin::OnFrameSpin);
	Connect(ID_FRAME_SPIN,wxEVT_SCROLL_LINEDOWN,(wxObjectEventFunction)&SplitterWin::OnFrameSpinDown);
	Connect(ID_ADD_SPLIT_POINT_BT,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&SplitterWin::OnAddCutPoint);
	Connect(ID_OPEN_BT,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&SplitterWin::OnOpenFile);
	Connect(ID_SETTINGS_BT,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&SplitterWin::OnSettingsBt);
	Connect(ID_ABOUT_BT,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&SplitterWin::OnAboutBt);
	Connect(ID_LISTCTRL1,wxEVT_COMMAND_LIST_ITEM_RIGHT_CLICK,(wxObjectEventFunction)&SplitterWin::OnCutPointRClick);
	Connect(ID_START_BT,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&SplitterWin::OnRunBt);
	//*)
	Connect(ID_MEDIA_CTRL, wxEVT_MEDIA_STOP, (wxObjectEventFunction)(wxEventFunction)(wxMediaEventFunction) &SplitterWin::OnMediaStop);
	Connect(ID_MEDIA_CTRL, EVT_MEDIA_UPDATED, (wxObjectEventFunction)(wxEventFunction)(wxMediaEventFunction) &SplitterWin::OnMediaUpdate);
	Connect(REMOVE_POINT_ID, wxEVT_COMMAND_TOOL_CLICKED, (wxObjectEventFunction)&SplitterWin::OnRemovePoint);
	Connect(REMOVE_ALL_ID, wxEVT_COMMAND_TOOL_CLICKED, (wxObjectEventFunction)&SplitterWin::OnRemoveAll);
	Connect(wxEVT_CLOSE_WINDOW, (wxObjectEventFunction)&SplitterWin::OnClose);
	
	wxAcceleratorEntry entries[2];
	entries[0].Set(wxACCEL_ALT, (int) 'X', wxID_EXIT);
	entries[1].Set(wxACCEL_CTRL, (int) 'O', ID_OPEN_BT);
	wxAcceleratorTable accel(2, entries);
	SetAcceleratorTable(accel);

	addSplitPointBt->SetBitmap(wxArtProvider::GetBitmap(wxART_CUT, wxART_BUTTON));
	addSplitPointBt->SetLabel(addSplitPointBt->GetLabel() + " >");
	openBt->SetBitmap(wxArtProvider::GetBitmap(wxART_FILE_OPEN, wxART_TOOLBAR));
	settingsBt->SetBitmap(wxBITMAP_FROM_MEMORY(preferences));
	settingsBt->SetMinSize(aboutBt->GetSize());
	aboutBt->SetBitmap(wxBITMAP_FROM_MEMORY(info));
	aboutBt->SetMinSize(aboutBt->GetSize());
	startBt->SetBitmap(wxBITMAP_FROM_MEMORY(run));
	playBt->SetBitmap(wxBITMAP_FROM_MEMORY(play));
	playBt->Enable(false);
	
	// Restore frame size/position
	if (s_config.IsSplitterWinMaximized()) {
		Maximize(true);
	} else {
		wxRect rect = s_config.GetSplitterWinLocation();
		if (rect.width > 0 && rect.height > 0) {
			SetSize(rect);
			Center();
		} else {
			SetSize(wxSize(950,600));
			Center();
		}
	}

	pointListCtrl->AppendColumn("", wxLIST_FORMAT_LEFT, 20);
	pointListCtrl->AppendColumn(_("Start"));
	pointListCtrl->AppendColumn(_("End"));
	pointListCtrl->AppendColumn(_("Duration"));
	
	splitOnKeyFrame = false;
	mediaCtrl->SetPositionOnKeyFrame(splitOnKeyFrame);
	UpdateControls();

	pointListCtrl->SetFocus();
	SetDropTarget(new SplitterDnDFile(this));
}

SplitterWin::~SplitterWin() {
	//(*Destroy(SplitterWin)
	//*)
}

void SplitterWin::UpdateControls() {
	mediaCtrl->Enable(mediaFile.GetFileName().length() > 0);
	mediaSlider->Enable(mediaFile.GetFileName().length() > 0);
	timeCtrl->Enable(mediaFile.GetFileName().length() > 0);
	timeSpinBt->Enable(mediaFile.GetFileName().length() > 0);
	frameSpinBt->Enable(mediaFile.GetFileName().length() > 0);
	addSplitPointBt->Enable(mediaFile.GetFileName().length() > 0);
	pointListCtrl->Enable(mediaFile.GetFileName().length() > 0);
	startBt->Enable(cutPoints.size() > 0);

	// store checked times
	vector<long> unchecked;
	for (int i = 0; i < pointListCtrl->GetItemCount(); i++) {
		if (!pointListCtrl->IsChecked(i)) {
			unchecked.push_back(String2Time(pointListCtrl->GetItemText(i, 2)));
		}
	}

	pointListCtrl->DeleteAllItems();
	if (cutPoints.size() == 0)
		return;
	long endTime = 0;
	for (vector<long>::const_iterator it = cutPoints.begin(); ; it++) {
		long startTime = it == cutPoints.begin() ? 0 : endTime;
		endTime = it != cutPoints.end() ? *it : lround(mediaFile.GetDuration() * 1000);
		int idx = pointListCtrl->GetItemCount();
		pointListCtrl->InsertItem(idx, "",
				std::find(unchecked.begin(), unchecked.end(), endTime) != unchecked.end() ? 0 : 1);
		pointListCtrl->SetItem(idx, 1, Time2String(startTime, true));
		pointListCtrl->SetItem(idx, 2, Time2String(endTime, true));
		pointListCtrl->SetItem(idx, 3, Time2String(endTime - startTime, true));
		if (it == cutPoints.end())
			break;
	}
}

void SplitterWin::SeekVideo(long pos, bool updateTimeCtrl) {
	mediaCtrl->Seek((wxFileOffset) pos);
	if (splitOnKeyFrame)
		pos = mediaCtrl->GetKeyFramePosition() * 1000;
	videoPos = pos;
	mediaSlider->SetValue(lround(pos / 1000));
	timeSpinBt->SetValue(lround(pos / 1000));
	if (updateTimeCtrl)
		timeCtrl->ChangeValue(Time2String(pos, true));
}

void SplitterWin::OnSliderScroll(wxScrollEvent& event) {
	SeekVideo(((long)mediaSlider->GetValue())*1000);
}

void SplitterWin::OnOpenFile(wxCommandEvent& event) {
	wxString video = "*.avi;*.mkv;*.mov;*.mp4;*.mpeg;*.mpg;*.mts;*.ogg;*.ogm;*.webm;*.wmv;";
	wxFileDialog fileDlg(this, _("Choose a file"), wxT(""), wxT(""),
			wxString(_("MP4 Files")) + " (*.mp4)|*.mp4|"
			+ _("Video Files") + wxString::Format(" (%s)|%s|", video.c_str(), video.c_str())
			+ wxString(_("All Files")) + " (*.*)|*.*", wxFD_OPEN);
	fileDlg.SetDirectory(s_config.GetLastAddDir() + wxFILE_SEP_PATH);
	if (fileDlg.ShowModal() != wxID_OK)
		return;
	s_config.SetLastAddDir(fileDlg.GetDirectory());
	Open(fileDlg.GetPath());
}

bool SplitterWin::Open(wxString fileName) {
	if (!mediaCtrl->Load(fileName) || !mediaFile.Init(fileName)) {
		mediaFile.Init("");
		mediaCtrl->Load("");
		UpdateControls();
		return false;
	}
	mediaSlider->SetValue(0);
	mediaSlider->SetMax(mediaCtrl->Length()/1000);
	timeSpinBt->SetMax(mediaCtrl->Length()/1000);
	wxScrollEvent scrlEvt;
	OnSliderScroll(scrlEvt);
	playBt->Enable();

	cutPoints.clear();
	UpdateControls();
	return true;
}

void SplitterWin::OnMediaStop(const wxMediaEvent& evt) {
	playBt->SetBitmap(wxBITMAP_FROM_MEMORY(play));
}

void SplitterWin::OnMediaUpdate(const wxMediaEvent& evt) {
	videoPos = mediaCtrl->Tell();
	mediaSlider->SetValue(lround(videoPos / 1000));
	timeSpinBt->SetValue(lround(videoPos / 1000));
	timeCtrl->ChangeValue(Time2String(videoPos, true));
}

void SplitterWin::OnPlay(wxCommandEvent& event) {
	if (mediaCtrl->GetState() != wxMEDIASTATE_PLAYING) {
		mediaCtrl->Play();
		playBt->SetBitmap(wxBITMAP_FROM_MEMORY(pause));
	} else {
		mediaCtrl->Pause();
		playBt->SetBitmap(wxBITMAP_FROM_MEMORY(play));
	}
}

void SplitterWin::OnChangeTime(wxCommandEvent& event) {
	if (s_timeRE.Matches(timeCtrl->GetValue()))
		SeekVideo(String2Time(timeCtrl->GetValue()), false);
}

void SplitterWin::OnTimeSpin(wxSpinEvent& event) {
	SeekVideo(((long)timeSpinBt->GetValue())*1000);
}

void SplitterWin::OnFrameSpin(wxSpinEvent& event) {
    SeekVideo(String2Time(timeCtrl->GetValue()) + 1000 / mediaCtrl->GetFps());
}

void SplitterWin::OnFrameSpinDown(wxSpinEvent& event) {
    long pos = String2Time(timeCtrl->GetValue()) - 1000 / mediaCtrl->GetFps();
    SeekVideo(pos >= 0 ? pos : 0);
}

void SplitterWin::OnAddCutPoint(wxCommandEvent& event) {
	if (videoPos == 0 || videoPos == lround(mediaFile.GetDuration() * 1000)
			|| std::find(cutPoints.begin(), cutPoints.end(), videoPos) != cutPoints.end())
		return;
	cutPoints.push_back(videoPos);
	std::sort(cutPoints.begin(), cutPoints.end());
	UpdateControls();
}

void SplitterWin::OnRemovePoint(wxCommandEvent& event) {
	int sel = pointListCtrl->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
	if (sel >= 0) {
		cutPoints.erase(cutPoints.begin() + (sel < (int) cutPoints.size() ? sel : cutPoints.size() - 1));
		UpdateControls();
	}
}

void SplitterWin::OnRemoveAll(wxCommandEvent& event) {
	cutPoints.clear();
	UpdateControls();
}

void SplitterWin::OnCutPointRClick(wxListEvent &event) {
	wxMenu ctxMenu;
	ctxMenu.Append(REMOVE_POINT_ID, _("&Remove"));
	ctxMenu.Append(REMOVE_ALL_ID, _("Remove &all"));
	PopupMenu(&ctxMenu);
}

void SplitterWin::OnRunBt(wxCommandEvent& event) {
	// choose a output directory
	wxDirDialog dirDlg(this, _("Choose a output directory"), wxPathOnly(mediaFile.GetFileName()));
	if (dirDlg.ShowModal() != wxID_OK)
		return;
	wxString outputDir = dirDlg.GetPath();
	if (!wxDirExists(outputDir))
		return;

	// filename
	wxString fname;
	wxFileName::SplitPath(mediaFile.GetFileName(), NULL, &fname, NULL);

	// step count
	int stepCount = 0;
	for (int i = 0; i < pointListCtrl->GetItemCount(); i++) {
		if (pointListCtrl->IsChecked(i))
			stepCount++;
	}
	if (stepCount == 0)
		return;

	// show progress dialog
	ProgressDlg progDlg(this, _("MP4 Splitter"), _("Splitting the file"), stepCount*100, logFileName);
	progDlg.Show();
	int step = 0;

	// split
	int idx = 1;
	bool overwrite = false;
	for (int i = 0; i < pointListCtrl->GetItemCount(); i++) {
		if (!pointListCtrl->IsChecked(i))
			continue;

		long startTime = i == 0 ? 0 : cutPoints[i-1];
		long endTime = i < (int) cutPoints.size() ? cutPoints[i] : lround(mediaFile.GetDuration() * 1000);

		wxString ext = mediaFile.GetFileName().AfterLast('.').Lower();
		if (ext.length() > 5)
			ext = "mp4";
		wxString resultFile = outputDir + wxFILE_SEP_PATH + fname + wxString::Format("%02d", idx) + "." + ext;
		if (wxFileExists(resultFile)) {
			if (!overwrite && wxMessageBox(wxString::Format(
					_("File '%s' already exist. Do you want to overwrite it?"), resultFile.c_str()),
					_("Confirmation"), wxYES_NO|wxICON_QUESTION, this) == wxNO) {
				return;
			}
			overwrite = true;
			if (!wxRemoveFile(resultFile)) {
				return;
			}
		}

		progDlg.Update(step * 100, wxString::Format(_("Writting %s"), resultFile.c_str()));

		// build avconv/ffmpeg command
		wxString cmd = "ffmpeg";
#if defined(__WXMSW__) || defined(__WXMAC__)
		cmd = '"' + wxGetAppPath() + cmd + '"';
#endif
		cmd += " -i \"" + mediaFile.GetFileName() + '"';
		cmd += " -c:v copy";
		cmd += " -c:a copy";

		// cut video
		cmd += wxString::Format(wxT(" -ss %f"), (double) startTime / 1000);
		cmd += wxString::Format(wxT(" -t %f"), (double) (endTime - startTime) / 1000);

		// output file
		cmd += " \"" + resultFile + '"';
		progDlg.DoLogMessage(cmd);

		// execute avconv/ffmpeg
		AvConvProcess proc(&progDlg, step++, 100, (endTime - startTime) * mediaFile.GetVideoStream()->GetFps() / 1000);
		if (!proc.Execute(cmd)) {
			return;
		}
		progDlg.DoLogMessage("");

		idx++;
	}
	progDlg.Update(stepCount * 100, _("Successful"));
	while (progDlg.IsVisible() && !progDlg.WasCancelled()) {
		wxYield();
	}
}

void SplitterWin::OnSettingsBt(wxCommandEvent& event) {
	OptionsDlg dlg(this, true);
	dlg.SetSplitOnKeyFrame(splitOnKeyFrame);
	dlg.SetLogFile(logFileName);
	if (dlg.ShowModal() == wxID_OK) {
		logFileName = dlg.GetLogFile();
		splitOnKeyFrame = dlg.IsSplitOnKeyFrame();
		mediaCtrl->SetPositionOnKeyFrame(splitOnKeyFrame);
	}
}

wxString FixEmail(const wxString& str) {
	wxString result = str;
	result.Replace(" at ", "@");
	return result;
}

void SplitterWin::OnAboutBt(wxCommandEvent& event) {
	wxAboutDialogInfo info;
	info.SetName("MP4Splitter (MP4Tools)");
	info.SetVersion(APP_VERSION);
	info.SetDescription(_("Simple tool to split a MP4 file."));
	info.SetCopyright(APP_COPYRIGHT);
	info.SetWebSite(APP_COMPANYWEB);
#ifdef __WXMSW__
	info.SetIcon(wxICON(mp4splitter));
#else
	info.SetIcon(wxICON_FROM_MEMORY(mp4splitter));
#endif
	wxAboutBox(info);
}

void SplitterWin::OnClose(wxCloseEvent &event) {
	if (!IsIconized())
		s_config.SetSplitterWinLocation(GetRect(), IsMaximized());
	Destroy();
}
