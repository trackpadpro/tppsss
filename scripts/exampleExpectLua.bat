@echo off

set PATH=%PATH%;%~dp0..\deps\expect\;

expect.exe %~dp0exampleExpect.lua %~1