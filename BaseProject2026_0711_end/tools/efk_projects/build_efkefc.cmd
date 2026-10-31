@echo off
setlocal
set PROJ=%~dp0
set EFEKSEER=%PROJ%..\Effekseer\Effekseer.exe
set DATA=%PROJ%..\..\data\Sample\Effects
for %%N in (%*) do (
  "%EFEKSEER%" -cui -in "%DATA%\%%N.efkproj" -o "%DATA%\%%N.efkefc"
)
endlocal
