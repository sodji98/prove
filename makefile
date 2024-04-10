W = -c --all-warnings --extra-warnings -W
test: ./src/*
	gcc ./src/* $(W)
