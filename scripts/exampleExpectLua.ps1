param($STEAM_LOGIN_COOKIE)

$pathToScripts = $MyInvocation.MyCommand.Path

$env:Path += ";$pathToScripts\..\..\deps\expect\"

cd "$pathToScripts\..\..\deps\expect\"

ls

expect.exe $pathToScripts\..\exampleExpect.lua $STEAM_LOGIN_COOKIE
