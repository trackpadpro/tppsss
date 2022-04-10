@echo off
powershell.exe -NoProfile -ExecutionPolicy Bypass "& {& '%~dp0exampleExpectLua.ps1' %*}"