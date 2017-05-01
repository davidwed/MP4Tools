/////////////////////////////////////////////////////////////////////////////
// Name:      MediaFile.h
// Author:    Alex Thuering
// Created:   7.07.2012
// Copyright: (c) Alex Thuering
// Licence:   GPL
/////////////////////////////////////////////////////////////////////////////

#include "MediaFile.h"
#include "utils.h"
#include <wx/file.h>
#include <wxSVG/mediadec_ffmpeg.h>
#include "../resources/video.png.h"
#include "../resources/audio.png.h"

vector<int> s_emptyVecInt;

/** Constructor */
MediaFile::MediaFile() {
	this->size = 0;
	this->duration = 0;
	this->startTime = 0;
	this->recordingTime = -1;
	this->reencodeAudio = false;
	this->reencodeVideo = false;
}

/** Destructor */
MediaFile::~MediaFile() {
	VECTOR_CLEAR(streams, MediaStream)
}

bool MediaFile::Init(const wxString& fileName) {
	this->fileName = fileName;
	
	if (!wxFileExists(fileName))
		return false;
	this->size = wxFile(fileName).Length()/1024;
	
	wxFfmpegMediaDecoder ffmpeg;
	if (!ffmpeg.Load(fileName))
		return false;
	this->duration = ffmpeg.GetDuration() > 0 ? ffmpeg.GetDuration() : 0;
	this->formatName = ffmpeg.GetFormatName();
	for (unsigned int stIdx = 0; stIdx < ffmpeg.GetStreamCount(); stIdx++) {
		MediaStream* stream = new MediaStream(ffmpeg.GetStreamType(stIdx), ffmpeg.GetCodecName(stIdx),
				ffmpeg.GetCodecTag(stIdx));
		if (stream->GetType() == stAUDIO) {
			stream->SetChannelNumber(ffmpeg.GetChannelNumber(stIdx));
			stream->SetSampleRate(ffmpeg.GetSampleRate(stIdx));
			stream->SetBitrate(ffmpeg.GetBitrate(stIdx));
		} else if (stream->GetType() == stVIDEO) {
			stream->SetVideoSize(ffmpeg.GetVideoSize());
			stream->SetBitrate(ffmpeg.GetBitrate(stIdx));
			stream->SetAspectRatio(ffmpeg.GetFrameAspectRatio());
			stream->SetFps(ffmpeg.GetFps());
			stream->SetTimebase(ffmpeg.GetCodecTimeBase());
		}
		streams.push_back(stream);
	}
	// load video frame
	if (GetVideoStream() != NULL) {
		if (duration > 0) {
			image = ffmpeg.GetNextFrame();
			double dpos = duration < 6000 ? duration * 0.05 : 300;
			ffmpeg.SetPosition(dpos - 1.0);
			for (int i = 0; i < 60; i++) {
				image = ffmpeg.GetNextFrame();
				if (ffmpeg.GetPosition() >= dpos)
					break;
			}
		} else {
			for (int i = 0; i < 30; i++)
				image = ffmpeg.GetNextFrame();
		}
		ffmpeg.Close();
		if (!image.IsOk())
			image = wxBITMAP_FROM_MEMORY(video).ConvertToImage();
	} else {
		image = wxBITMAP_FROM_MEMORY(audio).ConvertToImage();
	}
	return true;
}

/** Returns result duration (cutted video) in sec */
double MediaFile::GetResultDuration() const {
	if (GetRecordingTime() != -1)
		return GetRecordingTime();
	return GetDuration() - GetStartTime();
}

/** Returns result size of file (cutted video) in KB */
long MediaFile::GetResultSize() const {
	return GetDuration() > 0 ? size * GetResultDuration() / GetDuration() : 0;
}

/** Returns video stream */
MediaStream* MediaFile::GetVideoStream() const {
	for (vector<MediaStream*>::const_iterator it = streams.begin(); it != streams.end(); it++)
		if ((*it)->GetType() == stVIDEO)
			return (*it);
	return NULL;
}

/** Checks this file has video streams compatible to given one */
bool MediaFile::HasCompatibleVideoStreams(const MediaFile* mediaFile) {
	MediaStream* st1 = GetVideoStream();
	MediaStream* st2 = mediaFile->GetVideoStream();
	if (st1 == NULL && st2 == NULL)
		return true;
	if (st1 == NULL || st2 == NULL)
		return false;
	return st1->IsCompatibleTo(st2);
}

/** Checks this file has audio streams compatible to given one */
bool MediaFile::HasCompatibleAudioStreams(const MediaFile* mediaFile) {
	unsigned int streamIdx1 = 0;
	for (unsigned int i = 0; i < GetStreams().size(); i++) {
		if (GetStreams()[i]->GetType() != stAUDIO)
			continue;
		// find corresponding audio stream in mediaFile
		while (streamIdx1 < mediaFile->GetStreams().size()
				&& mediaFile->GetStreams()[streamIdx1]->GetType() != stAUDIO)
			streamIdx1++;
		if (streamIdx1 >= mediaFile->GetStreams().size())
			return false;
		if (GetStreams()[i]->GetType() == stAUDIO && !GetStreams()[i]->IsCompatibleTo(mediaFile->GetStreams()[streamIdx1]))
			return false;
		streamIdx1++;
	}
	return true;
}

/** Calculates pad values to keep aspect ratio*/
wxSize MediaFile::CalcPad(const wxSize frameSize, double aspect, const vector<int>& crop) {
	MediaStream* videoSt = GetVideoStream();
	if (videoSt == NULL || videoSt->GetAspectRatio() <= 0)
		return wxSize();
	int padx = 0;
	int pady = 0;
	float oldAspect = videoSt->GetAspectRatio();
	wxSize oldFrameSize = videoSt->GetVideoSize();
	int cropX = crop.size() == 4 ? crop[0] + crop[1] : 0;
	int cropY = crop.size() == 4 ? crop[2] + crop[3] : 0;
	if (cropX + cropY > 0 && oldFrameSize.x > cropX && oldFrameSize.y > cropY)
		oldAspect *= ((double) oldFrameSize.y)/oldFrameSize.x*(oldFrameSize.x - cropX)/(oldFrameSize.y - cropY);
	if (aspect > oldAspect)
		padx = lround(((double)frameSize.x)*(1.0 - oldAspect/aspect)/2);
	else if (aspect < oldAspect)
		pady = lround(((double)frameSize.y)*(1.0 - aspect/oldAspect)/2);
	return wxSize(padx, pady);
}

/** Calculates crop value to keep aspect ratio*/
wxSize MediaFile::CalcCrop(const wxSize frameSize, double aspect, const vector<int>& pad) {
	MediaStream* videoSt = GetVideoStream();
	if (videoSt != NULL && videoSt->GetAspectRatio() <= 0)
		return wxSize();
	int cropx = 0;
	int cropy = 0;
	float oldAspect = videoSt->GetAspectRatio();
	wxSize oldFrameSize = videoSt->GetVideoSize();
	int padX = pad.size() == 4 ? pad[0] + pad[1] : 0;
	int padY = pad.size() == 4 ? pad[2] + pad[3] : 0;
	if (padX + padY > 0 && frameSize.x > padX && frameSize.y > padY)
		aspect *= ((double) frameSize.y)/frameSize.x*(frameSize.x - padX)/(frameSize.y - padY);
	if (aspect > oldAspect)
		cropy = lround(((double)oldFrameSize.y)*(1.0 - oldAspect/aspect)/2);
	else if (aspect < oldAspect)
		cropx = lround(((double)oldFrameSize.x)*(1.0 - aspect/oldAspect)/2);
	return wxSize(cropx, cropy);
}
