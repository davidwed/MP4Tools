bin_PROGRAMS = mp4joiner mp4splitter
mp4joiner_SOURCES = Config.cpp Languages.cpp MediaStream.cpp MediaFile.cpp MediaListBox.cpp utils.cpp\
 ProgramProcess.cpp mediactrl_ffmpeg.cpp VideoPropDlg.cpp OptionsDlg.cpp MainWin.cpp mp4joiner.cpp ProgressDlg.cpp

mp4splitter_SOURCES = Config.cpp Languages.cpp MediaStream.cpp MediaFile.cpp MediaListBox.cpp utils.cpp\
 ProgramProcess.cpp mediactrl_ffmpeg.cpp VideoPropDlg.cpp CheckedListCtrl.cpp SplitterWin.cpp mp4splitter.cpp ProgressDlg.cpp OptionsDlg.cpp

run: $(bin_PROGRAMS)
	./mp4joiner
	./mp4splitter
