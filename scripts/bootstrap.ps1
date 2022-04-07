if(-not(Test-Path -LiteralPath '.\data\')){
	try{
		New-Item -Path '.\data\' -ItemType Directory -ErrorAction Stop | Out-Null #-Force
	}
	catch{
		Write-Error -Message "Unable to create data directory. Error: $_" -ErrorAction Stop
	}
}

if(-not(Test-Path -LiteralPath '.\data\auth')){
	try{
		New-Item -Path '.\data\auth' -ItemType Directory -ErrorAction Stop | Out-Null #-Force
	}
	catch{
		Write-Error -Message "Unable to create authentication directory. Error: $_" -ErrorAction Stop
	}
}

cmake -S . -B .\build\
cmake --build .\build\