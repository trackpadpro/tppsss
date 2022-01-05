if(-not(Test-Path -LiteralPath '.\data\')){
	try{
		New-Item -Path '.\data\' -ItemType Directory -ErrorAction Stop | Out-Null #-Force
	}
	catch{
		Write-Error -Message "Unable to create data directory. Error: $_" -ErrorAction Stop
	}
}

# If your CMake build cannot find a CURL library, 
# either set the paths within .\CMakeLists.txt or 
# uncomment the following block and use 
# .\scripts\buildDepsCMakeLists.txt to build:

<#
git submodule add https://github.com/ARMmbed/mbedtls.git .\deps\mbedtls\
git submodule add https://github.com/curl/curl.git .\deps\curl\

git submodule update --init --recursive

cmake -S .\deps\mbedtls\ -B .\deps\mbedtls\build\
cmake --build .\deps\mbedtls\build\
ctest --test-dir .\deps\mbedtls\build\
#>

cmake -S . -B .\build\
cmake --build .\build\