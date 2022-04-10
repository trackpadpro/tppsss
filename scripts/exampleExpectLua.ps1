param($STEAM_LOGIN_COOKIE)

$pathToScripts = $MyInvocation.MyCommand.Path

(Get-Content $pathToScripts\..\exampleExpect.lua).replace('STEAM_LOGIN_COOKIE',$STEAM_LOGIN_COOKIE) |
	Set-Content $pathToScripts\..\exampleExpect.lua

expect.exe $pathToScripts\..\exampleExpect.lua

(Get-Content $pathToScripts\..\exampleExpect.lua).replace($STEAM_LOGIN_COOKIE,'STEAM_LOGIN_COOKIE') |
	Set-Content $pathToScripts\..\exampleExpect.lua
