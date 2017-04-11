#define MyAppName "OpenALPR Training Utility"
#define MyAppPublisher "OpenALPR"
#define MyAppVersion "1.0.0"
#define MyAppTemplate "plates"
#define MyAppExecutable MyAppTemplate + ".exe"

[Setup]
AppId={{2B5FE8FF-0856-4433-9FBD-57318237B5D4}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
AppPublisher={#MyAppPublisher}
DefaultDirName={pf}\{#MyAppName}
DefaultGroupName={#MyAppName}
OutputDir=.
OutputBaseFilename={#MyAppTemplate}-setup-{#MyAppVersion}
SetupIconFile={#MyAppTemplate}.ico
Compression=lzma
SolidCompression=yes
UninstallDisplayName={#MyAppName}
UninstallDisplayIcon={app}\{#MyAppExecutable}
MinVersion=0,5.1

[Languages]
Name: "AAAenglish"; MessagesFile: "compiler:Default.isl"
Name: "BrazilianPortuguesexisl"; MessagesFile: "compiler:Languages\BrazilianPortuguese.isl"
Name: "Catalanxisl"; MessagesFile: "compiler:Languages\Catalan.isl"
Name: "Corsicanxisl"; MessagesFile: "compiler:Languages\Corsican.isl"
Name: "Czechxisl"; MessagesFile: "compiler:Languages\Czech.isl"
Name: "Danishxisl"; MessagesFile: "compiler:Languages\Danish.isl"
Name: "Dutchxisl"; MessagesFile: "compiler:Languages\Dutch.isl"
Name: "Finnishxisl"; MessagesFile: "compiler:Languages\Finnish.isl"
Name: "Frenchxisl"; MessagesFile: "compiler:Languages\French.isl"
Name: "Germanxisl"; MessagesFile: "compiler:Languages\German.isl"
Name: "Greekxisl"; MessagesFile: "compiler:Languages\Greek.isl"
Name: "Hebrewxisl"; MessagesFile: "compiler:Languages\Hebrew.isl"
Name: "Hungarianxisl"; MessagesFile: "compiler:Languages\Hungarian.isl"
Name: "Italianxisl"; MessagesFile: "compiler:Languages\Italian.isl"
Name: "Japanesexisl"; MessagesFile: "compiler:Languages\Japanese.isl"
Name: "Norwegianxisl"; MessagesFile: "compiler:Languages\Norwegian.isl"
Name: "Polishxisl"; MessagesFile: "compiler:Languages\Polish.isl"
Name: "Portuguesexisl"; MessagesFile: "compiler:Languages\Portuguese.isl"
Name: "Russianxisl"; MessagesFile: "compiler:Languages\Russian.isl"
Name: "ScottishGaelicxisl"; MessagesFile: "compiler:Languages\ScottishGaelic.isl"
Name: "SerbianCyrillicxisl"; MessagesFile: "compiler:Languages\SerbianCyrillic.isl"
Name: "SerbianLatinxisl"; MessagesFile: "compiler:Languages\SerbianLatin.isl"
Name: "Slovenianxisl"; MessagesFile: "compiler:Languages\Slovenian.isl"
Name: "Spanishxisl"; MessagesFile: "compiler:Languages\Spanish.isl"
Name: "Turkishxisl"; MessagesFile: "compiler:Languages\Turkish.isl"
Name: "Ukrainianxisl"; MessagesFile: "compiler:Languages\Ukrainian.isl"

[Files]
Source: "COPYING-runtime.txt"; DestDir: "{app}"; Flags: ignoreversion
Source: "COPYING-winpthreads.txt"; DestDir: "{app}"; Flags: ignoreversion
Source: "libgcc_s_dw2-1.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "libstdc++-6.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "libwinpthread-1.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "Qt5Core.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "Qt5Gui.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "Qt5Widgets.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "imageformats/qico.dll"; DestDir: "{app}/imageformats"; Flags: ignoreversion
Source: "imageformats/qjpeg.dll"; DestDir: "{app}/imageformats"; Flags: ignoreversion
Source: "platforms/qwindows.dll"; DestDir: "{app}/platforms"; Flags: ignoreversion
Source: "{#MyAppExecutable}"; DestDir: "{app}"; Flags: ignoreversion

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"
Name: "quicklaunchicon"; Description: "{cm:CreateQuickLaunchIcon}"; GroupDescription: "{cm:AdditionalIcons}"; OnlyBelowVersion: 0,6.1

[Run]
Filename: {app}\{#MyAppExecutable}; Description: {cm:LaunchProgram,{#MyAppName}}; Flags: nowait postinstall skipifsilent

[Icons]
Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyAppExecutable}"
Name: "{group}\{cm:UninstallProgram,{#MyAppName}}"; Filename: "{uninstallexe}"
Name: "{commondesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExecutable}"; Tasks: desktopicon
Name: "{userappdata}\Microsoft\Internet Explorer\Quick Launch\{#MyAppName}"; Filename: "{app}\{#MyAppExecutable}"; Tasks: quicklaunchicon

[Code]
procedure CurStepChanged(CurStep: TSetupStep);
var
ResultCode: Integer;
Uninstall: String;
UninstallQuery : String;
begin
UninstallQuery := ExpandConstant('Software\Microsoft\Windows\CurrentVersion\Uninstall\{#emit SetupSetting("AppId")}_is1');
  if (CurStep = ssInstall) then begin
    if RegQueryStringValue(HKLM, UninstallQuery, 'UninstallString', Uninstall)
        or RegQueryStringValue(HKCU, UninstallQuery, 'UninstallString', Uninstall) then begin
      Uninstall := RemoveQuotes(Uninstall)
      if (FileExists(Uninstall)) AND (not Exec(RemoveQuotes(Uninstall), '/VERYSILENT', '', SW_SHOWNORMAL, ewWaitUntilTerminated, ResultCode)) then begin
        MsgBox(SysErrorMessage(ResultCode), mbCriticalError, MB_OK);
        Abort();
      end;
    end;
  end;
end;

procedure CurUninstallStepChanged(CurUninstallStep: TUninstallStep);
  var ErrorCode: Integer;
begin
  if (CurUninstallStep = usUninstall) then begin
    ShellExec('open','taskkill.exe','/f /im {#MyAppExecutable}','',SW_HIDE,ewNoWait,ErrorCode);
    ShellExec('open','tskill.exe',' {#MyAppName}','',SW_HIDE,ewNoWait,ErrorCode);
  end;
end;
