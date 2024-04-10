W = -c --all-warnings --extra-warnings -W
SRC=$(wildcard ./src/*.c)

test: $(SRC)
    gcc $^ $(W) -o $@
