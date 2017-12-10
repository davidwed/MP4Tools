/////////////////////////////////////////////////////////////////////////////
// Name:      Config.cpp
// Author:    Alex Thuering
// Created:   13.07.2012
// Copyright: (c) Alex Thuering
// Licence:   GPL
/////////////////////////////////////////////////////////////////////////////

#ifndef CONFIG_H_
#define CONFIG_H_

#include <wx/wx.h>
#include <wx/config.h>
#include <wx/filename.h>

const wxString DEF_LANGUAGE_CODE= "EN";
#define DEF_LAST_ADD_DIR wxGetHomeDir()
const wxString DEF_MP4BOX_PARAMS = "-bs-switching merge -force-cat";

#define CONFIG_PROP(name, cfgName, defValue)\
  CONFIG_PROP_T(name, cfgName, defValue, wxString, wxString)

#define CONFIG_PROP_BOOL(name, cfgName, defValue)\
  CONFIG_PROP_T(name, cfgName, defValue, bool, long)

#define CONFIG_PROP_INT(name, cfgName, defValue)\
  CONFIG_PROP_T(name, cfgName, defValue, int, long)

#define CONFIG_PROP_T(name, cfgName, defValue, cfgType, cfgType2)\
  cfgType Get##name(bool def = false)\
  { return def ? (cfgType2) defValue : cfg->Read(cfgName, (cfgType2) defValue); }\
  void Set##name(cfgType value)\
  { wxLogNull log; if (value == defValue) cfg->DeleteEntry(cfgName); else cfg->Write(cfgName, value); }

class Config {
public:
    void Init();
    bool Flush() { return cfg->Flush(); }
    bool DeleteAll() { return cfg->DeleteAll(); }
    bool DeleteGroup(wxString group) { return cfg->DeleteGroup(group); }
	
    bool IsMainWinMaximized();
    wxRect GetMainWinLocation();
    void SetMainWinLocation(wxRect rect, bool isMaximized);
    
    bool IsSplitterWinMaximized();
    wxRect GetSplitterWinLocation();
    void SetSplitterWinLocation(wxRect rect, bool isMaximized);
    
    wxConfigBase* GetConfigBase() { return cfg; }
	
	CONFIG_PROP(LanguageCode, "Interface/LanguageCode", DEF_LANGUAGE_CODE)
	CONFIG_PROP(LastAddDir, "Interface/LastAddDir", DEF_LAST_ADD_DIR)
	CONFIG_PROP(MP4BoxParam, "Core/MP4BoxParams", DEF_MP4BOX_PARAMS)
	CONFIG_PROP(TempDir, "Core/TempDir", "")
	
  protected:
    wxConfigBase* cfg;
};

extern Config s_config;

#endif //CONFIG_H_
