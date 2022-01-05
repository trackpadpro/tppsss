if(-not(Test-Path -LiteralPath '.\data\')){
	try{
		New-Item -Path '.\data\' -ItemType Directory -ErrorAction Stop | Out-Null #-Force
	}
	catch{
		Write-Error -Message "Unable to create data directory. Error: $_" -ErrorAction Stop
	}
}

cmake -S . -B .\build\
cmake --build .\build\