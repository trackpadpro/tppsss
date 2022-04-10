param($deps)

if($deps -eq 'on'){
	cmake -S . -B .\build\ -DBUILD_DEPS=ON
}
else{
	cmake -S . -B .\build\
}

cmake --build .\build\