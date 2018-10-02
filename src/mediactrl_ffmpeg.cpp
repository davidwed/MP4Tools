/////////////////////////////////////////////////////////////////////////////
// Name:        mediactrl_ffmpeg.cpp
// Purpose:     Displays video
// Author:      Alex Thuering
// Created:     21.01.2011
// RCS-ID:      $Id: mediactrl_ffmpeg.cpp,v 1.11 2014/09/23 05:16:15 ntalex Exp $
// Copyright:   (c) Alex Thuering
// Licence:     GPL
/////////////////////////////////////////////////////////////////////////////

#include "mediactrl_ffmpeg.h"
#include <wx/wx.h>
#include <wx/dcclient.h>
#include <wxSVG/mediadec_ffmpeg.h>

//#include <SDL2/SDL.h>
//#include <SDL2/SDL_audio.h>

using namespace std;

class FFEvtHandler: public wxEvtHandler {
public:
	FFEvtHandler(wxMediaCtrl* ctrl): m_ctrl(ctrl) {
		m_frameAspectRatio = 1;
		redrawBitmap = true;
	}
	
	virtual ~FFEvtHandler() {
		// nothing to do
	}

	inline wxImage GetImage() {
		return m_image;
	}

	inline void SetImage(wxImage image) {
		m_image = image;
		redrawBitmap = true;
		m_ctrl->Refresh();
	}
	
	inline void SetFrameAspectRatio(float frameAspectRatio) {
		m_frameAspectRatio = frameAspectRatio;
	}

protected:
	virtual void OnPaint(wxPaintEvent &event) {
		wxPaintDC dc(m_ctrl);
		wxSize size = m_ctrl->GetClientSize();
		if (redrawBitmap) {
			if (!m_image.Ok())
				return;
			wxImage img = m_image;
			double aspect = m_frameAspectRatio;
			m_bmpRect = size;
			if (size.y >= size.x/aspect) {
				m_bmpRect.height = size.x/aspect;
				m_bmpRect.y = (size.y - m_bmpRect.height)/2;
			} else {
				m_bmpRect.width = size.y*aspect;
				m_bmpRect.x = (size.x - m_bmpRect.width)/2;
			}
			m_bitmap = wxBitmap(img.Scale(m_bmpRect.width, m_bmpRect.height));
			redrawBitmap = false;
		}
		
		dc.SetPen(*wxTRANSPARENT_PEN);
		dc.SetBrush(m_ctrl->GetBackgroundColour());
		dc.DrawRectangle(0, 0, m_bmpRect.x, size.GetHeight());
		dc.DrawRectangle(m_bmpRect.GetRight()+1, 0, size.GetWidth() - m_bmpRect.GetRight(), size.GetHeight());
		dc.DrawRectangle(m_bmpRect.x, 0, m_bmpRect.GetWidth(), m_bmpRect.y);
		dc.DrawRectangle(m_bmpRect.x, m_bmpRect.GetBottom()+1, m_bmpRect.GetWidth(), size.GetHeight() - m_bmpRect.GetBottom());
		
		dc.DrawBitmap(m_bitmap, m_bmpRect.GetPosition());
	}

	virtual void OnResize(wxSizeEvent &event) {
		m_bitmap = wxBitmap();
		m_ctrl->Refresh();
	}
	
	virtual void OnEraseBackground(wxEraseEvent &event) {
		if (!m_image.IsOk())
			event.Skip();
	}

private:
	wxMediaCtrl* m_ctrl;
	wxImage m_image;
	float m_frameAspectRatio;
	wxRect m_bmpRect;
	wxBitmap m_bitmap;
	bool redrawBitmap;
	DECLARE_EVENT_TABLE()
};

BEGIN_EVENT_TABLE(FFEvtHandler, wxEvtHandler)
  EVT_PAINT(FFEvtHandler::OnPaint)
  EVT_SIZE(FFEvtHandler::OnResize)
  EVT_ERASE_BACKGROUND(FFEvtHandler::OnEraseBackground)
END_EVENT_TABLE()

wxDEFINE_EVENT(EVT_MEDIA_UPDATED, wxMediaEvent);

class wxFFMediaBackend;

/** Playback class for ffmpeg */
class wxFFMediaPlaybackThread: public wxThread {
public:
	wxFFMediaPlaybackThread(wxFFMediaBackend* backend, float fps): m_backend(backend), m_lastTime(0) {
		m_frameTime = lroundf(1000/fps);
	}
	~wxFFMediaPlaybackThread();
	
	wxThread::ExitCode Entry();
private:
	wxFFMediaBackend* m_backend;
	int m_frameTime; // frame duration in sec
	wxLongLong m_lastTime;
};

//typedef struct VideoState {
//    uint8_t *audio_buf;
//    unsigned int audio_buf_size; /* in bytes */
//    int audio_buf_index; /* in bytes */
//    int audio_volume;
//    int paused;
//    int muted;
//};
//
///* prepare a new audio buffer */
//static void sdl_audio_callback(void *opaque, Uint8 *stream, int len) {
//    VideoState *is = opaque;
//    int audio_size, len1;
//
//    while (len > 0) {
//        if (is->audio_buf_index >= is->audio_buf_size) {
//           audio_size = audio_decode_frame(is);
//           if (audio_size < 0) {
//                /* if error, just output silence */
//               is->audio_buf = NULL;
//               is->audio_buf_size = SDL_AUDIO_MIN_BUFFER_SIZE / is->audio_tgt.frame_size * is->audio_tgt.frame_size;
//           } else {
//               is->audio_buf_size = audio_size;
//           }
//           is->audio_buf_index = 0;
//        }
//        len1 = is->audio_buf_size - is->audio_buf_index;
//        if (len1 > len)
//            len1 = len;
//        if (!is->muted && is->audio_buf && is->audio_volume == SDL_MIX_MAXVOLUME)
//            memcpy(stream, (uint8_t *)is->audio_buf + is->audio_buf_index, len1);
//        else {
//            memset(stream, 0, len1);
//            if (!is->muted && is->audio_buf)
//                SDL_MixAudioFormat(stream, (uint8_t *)is->audio_buf + is->audio_buf_index, AUDIO_S16SYS, len1, is->audio_volume);
//        }
//        len -= len1;
//        stream += len1;
//        is->audio_buf_index += len1;
//    }
//}


/////////////////////////////////////////////////////////////////////////////
/////////////////////////// wxFFMediaBackend ////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
class wxFFMediaBackend: public wxMediaBackendCommonBase {
public:
	wxFFMediaBackend(): m_loaded(false), m_duration(0), m_evtHandler(NULL) {
		m_pThread = NULL;
	}
	
	virtual ~wxFFMediaBackend() {
		if (m_evtHandler != NULL) {
			m_ctrl->PopEventHandler(true);
			m_evtHandler = NULL;
		}
	}
	
	virtual bool CreateControl(wxControl* ctrl, wxWindow* parent, wxWindowID id, const wxPoint& pos,
			const wxSize& size, long style, const wxValidator& validator, const wxString& name) {
		if (!ctrl->wxControl::Create(parent, id, pos, size, style, validator, name))
			return false;
		m_ctrl = wxStaticCast(ctrl, wxMediaCtrl);
		m_evtHandler = new FFEvtHandler(m_ctrl);
		m_ctrl->PushEventHandler(m_evtHandler);
		return true;
	}
	
	virtual bool Load(const wxString& fileName) {
		wxMutexLocker locker(m_decoderMutex);
		if (fileName.length() && m_decoder.Load(fileName)) {
			m_loaded = true;
			m_duration = m_decoder.GetDuration();
			if (m_duration < 0) {
				if (m_decoder.SetPosition(360000, false))
					m_duration = m_decoder.GetPosition();
				m_decoder.SetPosition(0, false);
			}
			NotifyMovieLoaded();
			m_evtHandler->SetImage(m_decoder.GetNextFrame());
			m_evtHandler->SetFrameAspectRatio(m_decoder.GetFrameAspectRatio());
			return true;
		}
		m_loaded = false;
		m_evtHandler->SetImage(wxImage());
		return false;
	}
	
	bool ShowNextFrame() {
		if (!m_loaded)
			return false;
		m_decoderMutex.Lock();
		wxImage img = m_decoder.GetNextFrame();
		m_decoderMutex.Unlock();
		m_evtHandler->SetImage(img);
		QueueEvent(EVT_MEDIA_UPDATED);
		return m_evtHandler->GetImage().Ok();
	}
	
	virtual bool SetPosition(wxLongLong where) {
		if (!m_loaded)
			return false;
		wxMutexLocker locker(m_decoderMutex);
		double dpos = where.ToDouble()/1000;
		if (dpos == 0 && m_decoder.GetPosition() != 0) {
			m_decoder.SetPosition(0, false);
			for (int i = 0; i < 8; i++) {
				m_decoder.GetNextFrame();
			}
		}
		m_decoder.SetPosition(dpos > 1.0 ? dpos - 1.0 : 0.0, dpos != 0);
		wxImage image;
		for (int i = 0; i < 60; i++) {
			image = m_decoder.GetNextFrame();
			if (m_decoder.GetPosition() >= dpos)
				break;
		}
		m_evtHandler->SetImage(image);
		return true;
	}
	
	virtual wxLongLong GetPosition() {
		wxLongLong res;
		if (m_loaded) {
			wxMutexLocker locker(m_decoderMutex);
			res.Assign(m_decoder.GetPosition()*1000);
		}
		return res;
	}
	
	virtual wxLongLong GetDuration() {
		wxLongLong res;
		if (m_loaded)
			res.Assign(m_duration*1000);
		return res;
	}
	
	inline double GetFps() {
		wxMutexLocker locker(m_decoderMutex);
		return m_decoder.GetFps();
	}
	
	inline wxFfmpegMediaDecoder& GetDecoder() {
		return m_decoder;
	}

	inline wxImage GetImage() {
		return m_evtHandler->GetImage();
	}
	
	virtual wxMediaState GetState() {
		wxCriticalSectionLocker enter(m_pThreadCS);
		if (m_pThread) {
			if (m_pThread->IsRunning())
				return wxMEDIASTATE_PLAYING;
			else if (m_pThread->IsPaused())
				return wxMEDIASTATE_PAUSED;
		}
		return wxMEDIASTATE_STOPPED;
	}
	
	virtual bool Play() {
		wxCriticalSectionLocker enter(m_pThreadCS);
		if (m_pThread == NULL) {
			m_pThread = new wxFFMediaPlaybackThread(this, m_decoder.GetFps());
			if (m_pThread->Run() != wxTHREAD_NO_ERROR) {
				wxLogError("Can't create the playback thread.");
				delete m_pThread;
				m_pThread = NULL;
			}
			return true;
		}
		return m_pThread->IsRunning();
	}
	
	virtual bool Pause() {
		wxCriticalSectionLocker enter(m_pThreadCS);
		if (m_pThread == NULL)
			return false;
		m_pThread->Delete();
		return true;
	}
	
protected:
	wxFFMediaPlaybackThread* m_pThread;
	wxCriticalSection m_pThreadCS; // protects the m_pThread pointer
	friend class wxFFMediaPlaybackThread;
	
//	void AudioOpen() {
//		SDL_AudioSpec want, have;
//		SDL_AudioDeviceID dev;
//
//		SDL_memset(&want, 0, sizeof(want)); /* or SDL_zero(want) */
//		want.freq = 48000;
//		want.format = AUDIO_S16SYS;
//		want.channels = 2;
//		want.samples = 4096;
//		want.callback = sdl_audio_callback;
//
//		dev = SDL_OpenAudioDevice(NULL, 0, &want, &have, SDL_AUDIO_ALLOW_FORMAT_CHANGE);
//		if (dev == 0) {
//			wxLogError("Failed to open audio: %s", SDL_GetError());
//		} else {
//		    if (have.format != want.format) { /* we let this one thing change. */
//		    	wxLogError("We didn't get Float32 audio format.");
//		    }
//		    SDL_PauseAudioDevice(dev, 0); /* start audio playing. */
//		    wxMilliSleep(5000); /* let the audio callback play some sound for 5 seconds. */
//		    SDL_CloseAudioDevice(dev);
//		}
//	}

	
private:
	bool m_loaded;
	wxFfmpegMediaDecoder m_decoder;
	wxMutex m_decoderMutex;
	double m_duration;
	FFEvtHandler* m_evtHandler;
	
	DECLARE_DYNAMIC_CLASS(wxFFMediaBackend)
};

IMPLEMENT_DYNAMIC_CLASS(wxFFMediaBackend, wxMediaBackend)

wxFFMediaPlaybackThread::~wxFFMediaPlaybackThread() {
	wxCriticalSectionLocker enter(m_backend->m_pThreadCS);
	m_backend->m_pThread = NULL;
}

wxThread::ExitCode wxFFMediaPlaybackThread::Entry() {
	while (m_backend->ShowNextFrame()) {
		if (TestDestroy())
			return 0;
		if (m_lastTime != 0) {
			wxLongLong t = wxGetUTCTimeMillis() - m_lastTime;
			int diff = t.ToLong();
			if (m_frameTime > diff)
				wxMilliSleep(m_frameTime - diff);
		}
		m_lastTime = wxGetUTCTimeMillis();
	}
	m_backend->QueueStopEvent();
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
////////////////////////////// MediaCtrlFF //////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
MediaCtrlFF::MediaCtrlFF(wxWindow* parent, wxWindowID id, const wxString& fileName, const wxPoint& pos,
		const wxSize& size, long style, const wxValidator& validator, const wxString& name):
        wxMediaCtrl(parent, id, fileName, pos, size, style, wxT("wxFFMediaBackend"), validator, name) {
	if (m_imp == NULL) {
		m_imp = new wxFFMediaBackend();
		if (!m_imp->CreateControl(this, parent, id, pos, size, style, validator, name)) {
		    delete m_imp;
		    m_imp = NULL;
		}
	}
}

MediaCtrlFF::~MediaCtrlFF() {
	// nothing to do
}

double MediaCtrlFF::GetFps() {
	return ((wxFFMediaBackend*) m_imp)->GetFps();
}

wxFfmpegMediaDecoder& MediaCtrlFF::GetDecoder() {
	return ((wxFFMediaBackend*) m_imp)->GetDecoder();
}

wxImage MediaCtrlFF::GetImage() {
	return ((wxFFMediaBackend*) m_imp)->GetImage();
}

