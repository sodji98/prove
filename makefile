W = -xc++  --all-warnings --extra-warnings -W
test: ./src/*
	g++ ./src/* $(W)
