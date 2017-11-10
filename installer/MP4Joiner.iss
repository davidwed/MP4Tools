[Setup]          
AppName=MP4Tools
AppVerName=MP4Tools v3.6 beta 1
AppPublisher=Thüring IT-Consulting
AppPublisherURL=http://www.mp4joiner.org
AppSupportURL=http://www.mp4joiner.org
AppUpdatesURL=http://www.mp4joiner.org
DefaultDirName={pf}\MP4Tools
DefaultGroupName=MP4Tools
OutputBaseFilename=MP4Tools-3.6b1-win32
Compression=lzma
SolidCompression=yes
UninstallDisplayIcon={app}\bin\MP4Joiner.exe
PrivilegesRequired=admin
WizardSmallImageFile=MP4Joiner.bmp
DisableWelcomePage=no
ChangesAssociations=yes
AllowNoIcons=yes
InfoBeforeFile=Info.rtf

[Languages]
Name: "de"; MessagesFile: "compiler:Languages\German.isl"
Name: "en"; MessagesFile: "compiler:Default.isl"
Name: "es"; MessagesFile: "compiler:Languages\Spanish.isl"   
Name: "fr"; MessagesFile: "compiler:Languages\French.isl"
Name: "pl"; MessagesFile: "compiler:Languages\Polish.isl"
Name: "pt"; MessagesFile: "compiler:Languages\Portuguese.isl"
Name: "pt_BR"; MessagesFile: "compiler:Languages\BrazilianPortuguese.isl"
Name: "ru"; MessagesFile: "compiler:Languages\Russian.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"
Name: "quicklaunchicon"; Description: "{cm:CreateQuickLaunchIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "..\src\*.exe"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "..\src\*.dll"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "..\COPYING"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\ChangeLog"; DestDir: "{app}"; Flags: ignoreversion

Source: "..\locale\de\*"; DestDir: "{app}\locale\de"; Flags: ignoreversion
Source: "..\locale\es\*"; DestDir: "{app}\locale\es"; Flags: ignoreversion  
Source: "..\locale\fr\*"; DestDir: "{app}\locale\es"; Flags: ignoreversion 
Source: "..\locale\pl\*"; DestDir: "{app}\locale\pl"; Flags: ignoreversion
Source: "..\locale\pt\*"; DestDir: "{app}\locale\pt"; Flags: ignoreversion
Source: "..\locale\pt_BR\*"; DestDir: "{app}\locale\pt"; Flags: ignoreversion
Source: "..\locale\ru\*"; DestDir: "{app}\locale\ru"; Flags: ignoreversion

[Icons]
Name: "{group}\MP4Joiner"; Filename: "{app}\bin\MP4Joiner.exe"
Name: "{group}\MP4Splitter"; Filename: "{app}\bin\MP4Splitter.exe"
Name: "{group}\{cm:ProgramOnTheWeb,MP4Tools}"; Filename: "http://www.mp4joiner.org/"
Name: "{group}\{cm:UninstallProgram,MP4Tools}"; Filename: "{uninstallexe}"
Name: "{userdesktop}\MP4Joiner"; Filename: "{app}\bin\MP4Joiner.exe"; Tasks: desktopicon
Name: "{userdesktop}\MP4Splitter"; Filename: "{app}\bin\MP4Splitter.exe"; Tasks: desktopicon
Name: "{userappdata}\Microsoft\Internet Explorer\Quick Launch\MP4Joiner"; Filename: "{app}\bin\MP4Joiner.exe"; Tasks: quicklaunchicon
Name: "{userappdata}\Microsoft\Internet Explorer\Quick Launch\MP4Splitter"; Filename: "{app}\bin\MP4Splitter.exe"; Tasks: quicklaunchicon

[Registry]
Root: HKCU; Subkey: "Software\MP4Joiner"; Flags: uninsdeletekey
Root: HKCU; Subkey: "Software\MP4Joiner\Interface"; Flags: uninsdeletekey
Root: HKCU; Subkey: "Software\MP4Joiner\Interface"; ValueType: string; ValueName: "LanguageCode"; ValueData: {language} 
Root: HKCU; Subkey: "Software\MP4Splitter"; Flags: uninsdeletekey
Root: HKCU; Subkey: "Software\MP4Splitter\Interface"; Flags: uninsdeletekey
Root: HKCU; Subkey: "Software\MP4Splitter\Interface"; ValueType: string; ValueName: "LanguageCode"; ValueData: {language}

[Run]              
Filename: "{app}\bin\MP4Joiner.exe"; Description: "{cm:LaunchProgram,MP4Joiner}"; Flags: nowait postinstall skipifsilent 
Filename: "{app}\bin\MP4Splitter.exe"; Description: "{cm:LaunchProgram,MP4Splitter}"; Flags: nowait postinstall skipifsilent
