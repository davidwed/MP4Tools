/////////////////////////////////////////////////////////////////////////////
// Name:      MediaStream.cpp
// Author:    Alex Thuering
// Created:   18.02.2012
// Copyright: (c) Alex Thuering
// Licence:   GPL
/////////////////////////////////////////////////////////////////////////////

#include "MediaStream.h"

/** Constructor */
MediaStream::MediaStream(StreamType type, wxString codecName, wxString codecTag) {
	this->type = type;
	this->codecName = codecName.BeforeFirst(wxT(' '));
	this->codecTag = codecTag;
	this->bitrate = 0;
	this->channelNumber = 0;
	this->sampleRate = 0;
	this->aspectRatio = 0;
	this->fps = 0;
	this->timeBase = 0;
}

/** Return the stream format string */
wxString MediaStream::GetFormat() {
	wxString result = codecName;
	if (result == wxT("liba52") || codecName == wxT("ac-3"))
		result = wxT("ac3");
	else if (result.StartsWith(wxT("mpeg2video")))
		result = wxT("mpeg2");
	if (type == stVIDEO && GetCodecTag().length())
		result += wxT(" (") + GetCodecTag() + wxT(")");
	switch (type) {
	case stVIDEO:
		if (videoSize.IsFullySpecified()) {
			result += wxString::Format(wxT(", %dx%d"), videoSize.GetWidth(), videoSize.GetHeight());
			int ifps = lroundf(fps);
			if (ifps == 25)
				result += wxT(" (PAL)");
			else if (ifps == 30)
				result += wxT(" (NTSC)");
			else if (ifps == 24)
				result += wxT(" (NTSC film)");
			else if (ifps > 0)
				result += wxString::Format(wxT(" (%0.1f fps)"), fps);
			if (round(aspectRatio*100) == 133 || round(aspectRatio*100) == 136)
				result += wxT(", 4:3");
			else if (round(aspectRatio*100) == 178)
				result += wxT(", 16:9");
			else
				result += wxString::Format(wxT(", 1:%0.2f"), aspectRatio);
		}
		break;
	case stAUDIO:
		if (channelNumber > 0) {
			result += wxT(", ");
			if (channelNumber == 1)
				result += _("mono");
			else if (channelNumber == 2)
				result += _("stereo");
			else if (channelNumber == 6)
				result += wxT("5.1");
			else
				result += wxString::Format(_("%d channels"), channelNumber);
		}
		if (sampleRate)
			result += wxT(", ") + wxString::Format(_("%d Hz"), sampleRate);
		if (bitrate >= 1000)
			result += wxT(", ") + wxString::Format(_("%d kbit/s"), bitrate/1000);
		break;
	default:
		break;
	}
	return result;
}

/** Checks this stream compatible to given one */
bool MediaStream::IsCompatibleTo(const MediaStream* mediaStream) const {
	switch (type) {
	case stVIDEO:
		return GetCodecName() == mediaStream->GetCodecName()
				&& GetVideoSize() == mediaStream->GetVideoSize()
				&& lround(GetFps()*10) == lround(mediaStream->GetFps()*10)
				&& lround(GetTimebase()*10) == lround(mediaStream->GetTimebase()*10)
				&& lround(GetAspectRatio()*10) == lround(mediaStream->GetAspectRatio()*10);
	case stAUDIO:
		return GetCodecName() == mediaStream->GetCodecName()
				&& GetChannelNumber() == mediaStream->GetChannelNumber()
				&& GetSampleRate() == mediaStream->GetSampleRate();
	default:
		break;
	}
	return true;
}
