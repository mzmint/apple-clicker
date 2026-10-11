[Setup]
AppId={{6e7a1acc-6e56-44fe-9dfd-779a456f1a0b}
AppName=Apple Clicker
AppVersion=1.3.0
DefaultDirName={autopf}\Apple Clicker
DefaultGroupName=Apple Clicker
OutputDir=installer
OutputBaseFilename=AppleClicker-Setup-v1.3.0
Compression=lzma
SolidCompression=yes

[Files]
Source: "release v1.3.0\*"; DestDir: "{app}"; Flags: recursesubdirs

[Icons]
Name: "{group}\Apple Clicker"; Filename: "{app}\AppleClicker.exe"
Name: "{commondesktop}\Apple Clicker"; Filename: "{app}\AppleClicker.exe"

[Run]
Filename: "{app}\AppleClicker.exe"; Description: "Launch Apple Clicker"; Flags: nowait postinstall skipifsilent