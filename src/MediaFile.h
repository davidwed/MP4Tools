/////////////////////////////////////////////////////////////////////////////
// Name:      MediaFile.h
// Author:    Alex Thuering
// Created:   7.07.2012
// Copyright: (c) Alex Thuering
// Licence:   GPL
/////////////////////////////////////////////////////////////////////////////
#ifndef MEDIAFILE_H_
#define MEDIAFILE_H_

#include "MediaStream.h"
#include <vector>

using namespace std;
extern vector<int> s_emptyVecInt;

/**
 * Stores information about media file
 */
class MediaFile {
public:
	/** Constructor */
	MediaFile();
	/** Destructor */
	~MediaFile();
	
	/** Reads file info */
	bool Init(const wxString& fileName);
	
	/** Returns filename */
	const wxString& GetFileName() const {
		return fileName;
	}
	
	/** Returns size of file in KB */
	long GetSize() const {
		return size;
	}
	
	/** Returns duration in sec */
	double GetDuration() const {
		return duration;
	}
	
	/** Returns result duration (cutted video) in sec */
	double GetResultDuration() const;
	
	/** Returns result size of file (cutted video) in KB */
	long GetResultSize() const;
	
	/** Returns video image */
	const wxImage& GetImage() const {
		return image;
	}
	
	/** Sets start time (sec) */
	void SetStartTime(double value) { startTime = value; }
	/** Returns start time (sec) */
	double GetStartTime() const { return startTime; }
	
	/** Sets recording time (sec) */
	void SetRecordingTime(double value) { recordingTime = value; }
	/** Returns recording time (sec) */
	double GetRecordingTime() const { return recordingTime; }
	
	/** Sets, if video must be reencoded */
	void SetReencodeVideo(bool value) { reencodeVideo = value; }
	/** Returns true, if video must be reencoded */
	bool IsReencodeVideo() const { return reencodeVideo; }
	
	/** Sets, if audio must be reencoded */
	void SetReencodeAudio(bool value) { reencodeAudio = value; }
	/** Returns true, if audio must be reencoded */
	bool IsReencodeAudio() const { return reencodeAudio; }
	
	/** Returns true, if video must be cutted */
	bool IsCutVideo() const { return startTime != 0 || recordingTime != -1; }

	/** Returns format name */
	const wxString& GetFormatName() const {
		return formatName;
	}

	/** Returns media streams of the file */
	const vector<MediaStream*>& GetStreams() const {
		return streams;
	}

	/** Returns video stream */
	MediaStream* GetVideoStream() const;
	
	/** Checks this file has video streams compatible to given one */
	bool HasCompatibleVideoStreams(const MediaFile* mediaFile);
	
	/** Checks this file has audio streams compatible to given one */
	bool HasCompatibleAudioStreams(const MediaFile* mediaFile);
	
	/** Calculates pad values to keep aspect ratio*/
	wxSize CalcPad(const wxSize frameSize, double aspect, const vector<int>& crop = s_emptyVecInt);
	/** Calculates crop value to keep aspect ratio*/
	wxSize CalcCrop(const wxSize frameSize, double aspect, const vector<int>& pad = s_emptyVecInt);

private:
	/** File name */
	wxString fileName;
	/** Size of file in KB */
	long size;
	/** Duration of video in sec. */
	double duration;
	/** Start time in sec */
	double startTime;
	/** Recording time in sec */
	double recordingTime;
	/** Video image */
	wxImage image;
	/** Format name */
	wxString formatName;
	/** Media streams of the file */
	vector<MediaStream*> streams;
	/** Shows, if audio must be reencoded*/
	bool reencodeAudio;
	/** Shows, if video must be reencoded*/
	bool reencodeVideo;
};

#endif
