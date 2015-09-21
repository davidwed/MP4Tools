/////////////////////////////////////////////////////////////////////////////
// Name:      MediaStream.h
// Author:    Alex Thuering
// Created:   18.02.2013
// Copyright: (c) Alex Thuering
// Licence:   GPL
/////////////////////////////////////////////////////////////////////////////
#ifndef MEIDA_STREAM_H_
#define MEIDA_STREAM_H_

#include <wx/wx.h>
#include <wxSVG/mediadec_ffmpeg.h>

/**
 * Stores information about media stream
 */
class MediaStream {
public:
	/** Constructor */
	MediaStream(StreamType type, wxString codecName, wxString codecTag);
	
	/** Returns stream type: audio, video or subtitle */
	StreamType GetType() const { return type; }
	
	/** Returns codec name */
	wxString GetCodecName() const { return codecName; }
	void SetCodecName(wxString codecName) { this->codecName = codecName; }
	
	/** Returns video codec tag (fourcc, e.g. DIVX or XVID) */
	wxString GetCodecTag() const { return codecTag; }
	void SetCodecTag(wxString codecName) { this->codecTag = codecTag; }
		
	/** Returns size of video */
	wxSize GetVideoSize() const { return videoSize; }
	void SetVideoSize(wxSize videoSize) { this->videoSize = videoSize; }
	
	/** Returns aspect ratio of video */
	float GetAspectRatio() const { return aspectRatio; }
	void SetAspectRatio(float aspectRatio) { this->aspectRatio = aspectRatio; }
	
	/** Returns count of frames per second of video */
	float GetFps() const { return fps; }
	void SetFps(float fps) { this->fps = fps; }
	
	/** Returns time base (tbc) of video */
	float GetTimebase() const { return timeBase; }
	void SetTimebase(float timeBase) { this->timeBase = timeBase; }
	
	/** Returns bitrate of the stream */
	int GetBitrate() const { return bitrate; }
	void SetBitrate(int bitrate) { this->bitrate = bitrate; }
	
	/** Returns channel number of audio stream */
	int GetChannelNumber() const { return channelNumber; }
	void SetChannelNumber(int channelNumber) { this->channelNumber = channelNumber; }
	
	/** Returns sample rate of audio stream */
	int GetSampleRate() const { return sampleRate; }
	void SetSampleRate(int sampleRate) { this->sampleRate = sampleRate; }
	
	/** Returns the stream format string */
	wxString GetFormat();
	
	/** Checks this stream compatible to given one */
	bool IsCompatibleTo(const MediaStream* mediaStream) const;
	
private:
	StreamType type; // audio/video/subtitle
	wxString codecName; // codec name
	wxString codecTag; // codec name
	int bitrate; // bitrate
	int channelNumber; // number of audio channels
	int sampleRate; // sample rate of audio
	wxSize videoSize; // frame size of video
	float aspectRatio; // aspect ratio of video
	float fps; // FPS of video
	float timeBase; // time base (tbc) of video
};

#endif /* MEIDA_STREAM_H_ */
