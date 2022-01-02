if(-not(Test-Path -LiteralPath '.\data\')){
	try{
		New-Item -Path '.\data\' -ItemType Directory -ErrorAction Stop | Out-Null #-Force
	}
	catch{
		Write-Error -Message "Unable to create data directory. Error: $_" -ErrorAction Stop
	}
}

cmake -S .\deps\mbedtls\ -B .\deps\mbedtls\build\
cmake --build .\deps\mbedtls\build\
ctest --test-dir .\deps\mbedtls\build\

cmake -S . -B .\build\
make -C .\build\